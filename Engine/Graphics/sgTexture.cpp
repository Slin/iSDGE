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

#if !defined __IOS__
	#include <cstring>
#endif

sgTexture::sgTexture()
{
	loaded = false;
	fbo = -1;
	texid = -1;
	texdata = NULL;
	textype = GL_TEXTURE_2D;
	format = GL_RGBA;
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
#if defined __IOS__
			glDeleteFramebuffersOES(1, &fbo);
			glDeleteRenderbuffersOES(1, &fbo_depth);
#endif
		}
	}

	if(texid != -1)
		glDeleteTextures(1, &texid);

	if(texdata != NULL)
		delete[] texdata;
}

void sgTexture::createTexture(const char *filename, bool mipmaps, bool lock)
{
	if(loaded)
		return;

	sgTextureFiles::sgUncompressedTexture *tex = 0;
	if(!sgTextureFiles::loadPNG(&tex, filename))
	{
		if(tex)
		{
			if(tex->bytes)
				delete[] tex->bytes;
			delete tex;
		}

		sgLog("Can´t load texture file: %s", filename);
		return;
	}

	width = tex->width;
	height = tex->height;

/*	if(!((width != 0) && !(width & (width - 1))))
	{
		if(!((height != 0) && !(height & (height - 1))))
		{
			sgLog("Texture resolution is not a power of two: %s", filename);
			width = 0;
			height = 0;
			return;
		}
	}*/

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);

	if(mipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#if defined __IOS__
		if(sgRenderer::oglversion <= 1)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		}
#elif !defined __ANDROID__
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#endif
	}else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	texdata = tex->bytes;
	if(tex->hasalpha)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		format = GL_RGBA;
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);
		format = GL_RGB;
	}

	if(mipmaps && sgRenderer::oglversion > 1)
	{
#if defined __IOS__ || defined __ANDROID__
		glGenerateMipmap(GL_TEXTURE_2D);
#endif
	}

	if(!lock)
	{
		if(tex->bytes)
			delete[] tex->bytes;
		texdata = NULL;
	}
	delete tex;

	sgResourceManager::addResource(filename, this);

	loaded = true;
}

void sgTexture::createPVRTexture(const char *filename)
{
	if(loaded)
		return;

	sgTextureFiles::sgPVRTexture *tex = 0;
	if(!sgTextureFiles::loadPVR(&tex, filename) || tex->mipsizes.size() < 1)
	{
		if(tex)
		{
			if(tex->bytes)
				delete[] tex->bytes;
			delete tex;
		}

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	unsigned long offset = 0;
	for(int i = 0; i < tex->mipsizes.size(); i++)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, i, tex->glformat, w, h, 0, tex->mipsizes[i], &tex->bytes[offset]);
		offset += tex->mipsizes[i];

		w = fmaxf((unsigned int)(w >> 1), 1.0f);
		h = fmaxf((unsigned int)(h >> 1), 1.0f);
	}

	format = tex->glformat;

	if(tex->bytes)
		delete[] tex->bytes;
	delete tex;

	sgResourceManager::addResource(filename, this);

	loaded = true;
}

void sgTexture::createTexture(float width_, float height_, bool cubemap)
{
	if(loaded)
		return;

	if(cubemap)
	{
		textype = GL_TEXTURE_CUBE_MAP;
	}else
	{
		textype = GL_TEXTURE_2D;
	}
	width = width_;
	height = height_;

	texdata = new unsigned char[width*height*4];

	glGenTextures(1, &texid);
	glBindTexture(textype, texid);

	if(cubemap)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
	}else
	{
		glTexImage2D(textype, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
	}

	format = GL_RGBA;

	glTexParameterf(textype, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textype, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(textype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(textype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	sgResourceManager::addResource(this);

	delete[] texdata;
	texdata = 0;

	loaded = true;
}

void sgTexture::makeRendertarget()
{
	if(textype != GL_TEXTURE_2D || fbo != -1)
		return;

	if(sgRenderer::oglversion > 1)
	{
#if defined __IOS__ || defined __ANDROID__
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(textype, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glGenRenderbuffers(1, &fbo_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
#else
		glGenFramebuffersEXT(1, &fbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);
		glBindTexture(textype, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glGenRenderbuffersEXT(1, &fbo_depth);
		glBindRenderbufferEXT(GL_RENDERBUFFER, fbo_depth);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
#endif
	}else
	{
#if defined __IOS__
		glGenFramebuffersOES(1, &fbo);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texid, 0);

		glGenRenderbuffersOES(1, &fbo_depth);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, fbo_depth);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, fbo_depth);
#endif
	}
}

void sgTexture::generateMipmaps()
{
	glBindTexture(textype, texid);
	glTexParameteri(textype, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(textype);
}

void sgTexture::setParameteri(unsigned int pname, unsigned int param)
{
	if(!loaded)
		return;

	glBindTexture(textype, texid);
	glTexParameteri(textype, pname, param);
}

sgTexture *sgTexture::getTexture(const char *filename, bool mipmaps, bool lock)
{
	sgTexture *tex = (sgTexture*)sgResourceManager::getResource(filename);
	if(tex != NULL)
		return tex;

	tex = new sgTexture();
	std::string fnm(filename);
	if(fnm.rfind(".png") != std::string::npos || fnm.rfind(".jpg") != std::string::npos || fnm.rfind(".PNG") != std::string::npos || fnm.rfind(".JPG") != std::string::npos)
	{
		tex->createTexture(filename, mipmaps, lock);
	}else if(fnm.rfind(".pvr") != std::string::npos || fnm.rfind(".PVR") != std::string::npos)
	{
		tex->createPVRTexture(filename);
	}else
	{
		delete tex;
		tex = NULL;
	}
	return tex;
}

sgTexture *sgTexture::getTexture(float width_, float height_, bool cubemap)
{
	sgTexture *tex = new sgTexture();
	tex->createTexture(width_, height_, cubemap);
	return tex;
}

void sgTexture::lockPixels()
{
	if(texdata != NULL)
		return;

	glBindTexture(GL_TEXTURE_2D, texid);
	texdata = new unsigned char[width*height*((format == GL_RGBA)?4:3)];
	for(int i = 0; i < width*height*((format == GL_RGBA)?4:3); i++)
		texdata[i] = 0;

/*	bool newfbo = false;
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
	}*/
}

void sgTexture::updatePixels(bool swapped)
{
	glBindTexture(GL_TEXTURE_2D, texid);
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, (swapped?GL_BGRA:GL_RGBA), GL_UNSIGNED_BYTE, texdata);
//	glTexImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, (swapped?GL_BGRA:GL_RGBA), GL_UNSIGNED_BYTE, texdata);
#if !defined __ANDROID__
	glTexImage2D(textype, 0, format, width, height, 0, (swapped?GL_BGRA:format), GL_UNSIGNED_BYTE, texdata);
#else
	glTexImage2D(textype, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texdata);
#endif
}


void sgTexture::unlockPixels()
{
	updatePixels();
	delete[] texdata;
	texdata = NULL;
}

void sgTexture::setColor(sgColorA color)
{
	for(int i = 0; i < width*height; i++)
	{
		memcpy(&texdata[i*((format == GL_RGBA)?4:3)], &color.r, 4*sizeof(char));
	}
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

	int bytes = ((format == GL_RGBA)?4:3);
	texdata[y*width*bytes+x*bytes+0] = color.r;
	texdata[y*width*bytes+x*bytes+1] = color.g;
	texdata[y*width*bytes+x*bytes+2] = color.b;
	if(format = GL_RGBA)
		texdata[y*width*bytes+x*bytes+3] = color.a;
}

void sgTexture::setPixels(unsigned char *data, unsigned int offset, unsigned int size)
{
	memcpy(texdata, data, size);
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

	int bytes = ((format == GL_RGBA)?4:3);
	if(bytes == 4)
		return sgColorA(texdata[y*width*bytes+x*bytes+0], texdata[y*width*bytes+x*bytes+1], texdata[y*width*bytes+x*bytes+2], texdata[y*width*bytes+x*bytes+3]);
	else
		return sgColorA(texdata[y*width*bytes+x*bytes+0], texdata[y*width*bytes+x*bytes+1], texdata[y*width*bytes+x*bytes+2], 1.0);
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
