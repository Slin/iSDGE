//
//	sgTexture.cpp
//	iSDGE
//
//	Created by Nils Daumann on 17.04.10.
//	Copyright (c) 2010 Nils Daumann

//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:

//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.

//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include "sgTexture.h"

#include <string>
#include <cmath>
#include "sgTextureFiles.h"
#include "sgResourceManager.h"
#include "sgRenderer.h"
#include "sgColor.h"
#include "sgDebug.h"

sgTexture::sgTexture()
{
	loaded = false;
	fbo = -1;
	texid = -1;
	texdata = NULL;
}

sgTexture::~sgTexture()
{
	if(fbo != -1)
	{
		if(sgRenderer::oglversion > 1)
		{
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(1, &fbo_depth);
		}else
		{
			glDeleteFramebuffersOES(1, &fbo);
			glDeleteRenderbuffersOES(1, &fbo_depth);
		}
	}
	
	if(texid != -1)
		glDeleteTextures(1, &texid);
	
	if(texdata != NULL)
		delete[] texdata;
}

void sgTexture::createTexture2D(const char *filename, bool mipmaps, bool lock)
{
	if(loaded)
		return;
	
	const char *filepath = sgResourceManager::getPath(filename);
	CGDataProviderRef texturefiledata = CGDataProviderCreateWithFilename(filepath);
	if(!texturefiledata)
	{
		sgLog("Can´t load texture file: %s", filename);
		return;
	}
	delete[] filepath;
	CGImageRef textureImage = CGImageCreateWithPNGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
	CGDataProviderRelease(texturefiledata);
	

	width = CGImageGetWidth(textureImage);     
	height = CGImageGetHeight(textureImage);

	if(!((width != 0) && !(width & (width - 1))))
	{
		if(!((height != 0) && !(height & (height - 1))))
		{
			sgLog("Texture resolution is not a power of two: %s", filename);
			width = 0;
			height = 0;
			return;
		}
	}

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	
	if(mipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		if(sgRenderer::oglversion <= 1)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		}
	}else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	
	texdata = (unsigned char *)calloc(width*height*4, 1);
	CGContextRef textureContext = CGBitmapContextCreate(texdata, width, height, 8, width*4, CGImageGetColorSpace(textureImage), kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(textureContext, CGRectMake(0.0, 0.0, (float)width, (float)height), textureImage);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);

	if(mipmaps && sgRenderer::oglversion > 1)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	if(!lock)
	{
		free(texdata);
		texdata = NULL;
	}
	CFRelease(textureContext);
	CGImageRelease(textureImage);
	
	sgResourceManager::addResource(filename, this);
	
	loaded = true;
}

void sgTexture::createPVRTexture2D(const char *filename)
{
	if(loaded)
		return;
	
	sgTextureFiles::sgPVRTexture *tex;
	if(!sgTextureFiles::loadPVR(&tex, filename) || tex->mipsizes.size() < 1)
	{
		if(tex->bytes)
			delete[] tex->bytes;
		delete tex;
		
		sgLog("Can´t load texture file: %s", filename);
		return;
	}
	
	width = tex->width;
	height = tex->height;
	
	unsigned int w = width;
	unsigned int h = height;
	
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	
	if(tex->mipsizes.size() > 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	
	unsigned long offset = 0;
	for(int i = 0; i < tex->mipsizes.size(); i++)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, i, tex->glformat, w, h, 0, tex->mipsizes[i], &tex->bytes[offset]);
		offset += tex->mipsizes[i];
		
		w = fmax(w >> 1, 1);
		h = fmax(h >> 1, 1);
	}	
	
	if(tex->bytes)
		delete[] tex->bytes;
	delete tex;
	
	sgResourceManager::addResource(filename, this);
	
	loaded = TRUE;
}

void sgTexture::createTexture2D(float width_, float height_)
{
	if(loaded)
		return;
	
	width = width_;
	height = height_;
	
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	sgResourceManager::addResource(this);
	
	loaded = true;
}

void sgTexture::makeRendertarget()
{
	if(fbo != -1)
		return;
	
	if(sgRenderer::oglversion > 1)
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glGenRenderbuffers(1, &fbo_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
	}else
	{
		glGenFramebuffersOES(1, &fbo);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texid, 0);
		
		glGenRenderbuffersOES(1, &fbo_depth);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, fbo_depth);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, fbo_depth);
	}
}

void sgTexture::setParameteri(unsigned int pname, unsigned int param)
{
	if(!loaded)
		return;
	
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, pname, param);
}

sgTexture *sgTexture::getTexture2D(const char *filename, bool mipmaps, bool lock)
{
	sgTexture *tex = (sgTexture*)sgResourceManager::getResource(filename);
	if(tex != NULL)
		return tex;
	
	tex = new sgTexture();
	std::string fnm(filename);
	if(fnm.rfind(".png") != std::string::npos)
	{
		tex->createTexture2D(filename, mipmaps, lock);
	}else if(fnm.rfind(".pvr") != std::string::npos)
	{
		tex->createPVRTexture2D(filename);
	}else
	{
		delete tex;
		tex = NULL;
	}
	return tex;
}

sgTexture *sgTexture::getTexture2D(float width_, float height_)
{
	sgTexture *tex = new sgTexture();
	tex->createTexture2D(width_, height_);
	return tex;
}

void sgTexture::lockPixels()
{
	if(texdata != NULL)
		return;
	
	glBindTexture(GL_TEXTURE_2D, texid);
	texdata = new unsigned char[width*height*4];
	for(int i = 0; i < width*height*4; i++)
		texdata[i] = 0;
	
	bool newfbo = false;
	if(fbo == -1)
	{
		newfbo = true;
		if(sgRenderer::oglversion > 1)
		{
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
		}else
		{
			glGenFramebuffersOES(1, &fbo);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texid, 0);
		}
	}else
	{
		if(sgRenderer::oglversion > 1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		}else
		{
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
		}
	}
	
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
	
	if(newfbo)
	{
		if(sgRenderer::oglversion > 1)
		{
			glDeleteFramebuffers(1, &fbo);
		}else
		{
			glDeleteFramebuffersOES(1, &fbo);
		}
		fbo = -1;
	}
}

void sgTexture::updatePixels()
{
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
}


void sgTexture::unlockPixels()
{
	updatePixels();
	delete[] texdata;
	texdata = NULL;
}

void sgTexture::setPixel(int x, int y, sgColorA color)
{
	if(x < 0)
	{
		int facx = -x/width;
		x += facx*width;
	}
	x %= width;
	
	if(y < 0)
	{
		int facy = -y/height;
		y += facy*height;
		sgLog("%i, %i", facy, y);
	}
	y %= height;
	
	texdata[y*width*4+x*4+0] = color.r;
	texdata[y*width*4+x*4+1] = color.g;
	texdata[y*width*4+x*4+2] = color.b;
	texdata[y*width*4+x*4+3] = color.a;
}

sgColorA sgTexture::getPixel(int x, int y)
{
	if(x < 0)
	{
		int facx = -x/width+1;
		x += facx*width;
	}
	x %= width;
	
	if(y < 0)
	{
		int facy = -y/height+1;
		y += facy*height;
	}
	y %= height;
	
	return sgColorA(texdata[y*width*4+x*4+0], texdata[y*width*4+x*4+1], texdata[y*width*4+x*4+2], texdata[y*width*4+x*4+3]);
}

sgColorA sgTexture::getPixel(float x, float y)
{
	int posx = (int)x;
	int posy = (int)y;
	float facx = x-posx;
	float facy = y-posy;
	sgColorA col0 = getPixel(posx, posy);
	sgColorA col1 = getPixel(posx+1, posy);
	sgColorA col2 = getPixel(posx, posy+1);
	sgColorA col3 = getPixel(posx+1, posy+1);
	
	sgColorA middlex0;
	middlex0.r = col0.r*facx+col1.r*(1.0-facx);
	middlex0.g = col0.g*facx+col1.g*(1.0-facx);
	middlex0.b = col0.b*facx+col1.b*(1.0-facx);
	middlex0.a = col0.a*facx+col1.a*(1.0-facx);
	
	sgColorA middlex1;
	middlex1.r = col2.r*facx+col3.r*(1.0-facx);
	middlex1.g = col2.g*facx+col3.g*(1.0-facx);
	middlex1.b = col2.b*facx+col3.b*(1.0-facx);
	middlex1.a = col2.a*facx+col3.a*(1.0-facx);
	
	sgColorA middle;
	middle.r = middlex0.r*facy+middlex1.r*(1.0-facy);
	middle.g = middlex0.g*facy+middlex1.g*(1.0-facy);
	middle.b = middlex0.b*facy+middlex1.b*(1.0-facy);
	middle.a = middlex0.a*facy+middlex1.a*(1.0-facy);
	
	return middle;
}

void sgTexture::destroy()
{
	sgResourceManager::removeResource(this);
	delete this;
}
