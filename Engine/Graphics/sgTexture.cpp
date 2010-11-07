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
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <CoreGraphics/CoreGraphics.h>
#include "sgResourceManager.h"
#include "sgRenderer.h"
#include "sgColor.h"

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
	
	filename = sgResourceManager::getPath(filename);
	CGDataProviderRef texturefiledata = CGDataProviderCreateWithFilename(filename);
	delete[] filename;
	CGImageRef textureImage = CGImageCreateWithPNGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
	CGDataProviderRelease(texturefiledata);
	

	width = CGImageGetWidth(textureImage);     
	height = CGImageGetHeight(textureImage);

	if(!((width != 0) && !(width & (width - 1))))
	{
		if(!((height != 0) && !(height & (height - 1))))
		{
//			sgDebug::writeString2("Texture has not a power of two: ", filename);
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

/*void sgTexture::createPVRTCTexture2D(const char *name, unsigned int w, unsigned int h, unsigned int type, BOOL mipmaps)
{
	if(loaded)
		return;
	
	width = w;     
	height = h;
	
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	
	mipmaps = FALSE;
	
	if(mipmaps)
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
	
	const char *path = sgRessourceManager::getPath(name, "pvrtc");
	NSData *texData = [[NSData alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path]];
	delete[] path;

	// This assumes that source PVRTC image is 4 bits per pixel and RGB not RGBA
	// If you use the default settings in texturetool, e.g.:
	//
	//      texturetool -e PVRTC -o texture.pvrtc texture.png
	//
	// then this code should work fine for you.
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, [texData length], [texData bytes]);*/
	
/*	if(mipmaps && sgRenderer::oglversion > 1)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}*/
	
/*	std::string na(name);
	na += std::string(".pvrtc");
	sgRessourceManager::addRessource(na.c_str(), this);
	
	loaded = TRUE;
	
}*/

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
//		glBindTexture(GL_TEXTURE_2D, texid);
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
//		glBindTexture(GL_TEXTURE_2D, texid);
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texid, 0);
		
		glGenRenderbuffersOES(1, &fbo_depth);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, fbo_depth);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, width, height);
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
	tex->createTexture2D(filename, mipmaps, lock);
	return tex;
}

sgTexture *sgTexture::getTexture2D(float width_, float height_)
{
	sgTexture *tex = new sgTexture();
	tex->createTexture2D(width_, height_);
	return tex;
}

/*sgTexture *sgTexture::getPVRTCTexture2D(const char *name, unsigned int w, unsigned int h, unsigned int type, BOOL mipmaps)
{
	std::string str(name);
	str += std::string(".pvrtc");
	sgTexture *tex = (sgTexture*)sgRessourceManager::getRessource(str.c_str());
	if(tex != NULL)
		return tex;
	
	tex = new sgTexture();
	tex->createPVRTCTexture2D(name, w, h, type, mipmaps);
	return tex;
}*/

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

void sgTexture::setPixel(unsigned int x, unsigned int y, sgColorA color)
{
	y = height-y-1;
	texdata[y*width*4+x*4+0] = color.r;
	texdata[y*width*4+x*4+1] = color.g;
	texdata[y*width*4+x*4+2] = color.b;
	texdata[y*width*4+x*4+3] = color.a;
}

sgColorA sgTexture::getPixel(unsigned int x, unsigned int y)
{
	y = height-y-1;
	return sgColorA(texdata[y*width*4+x*4+0], texdata[y*width*4+x*4+1], texdata[y*width*4+x*4+2], texdata[y*width*4+x*4+3]);
}

void sgTexture::destroy()
{
	sgResourceManager::removeResource(this);
	delete this;
}
