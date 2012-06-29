//
//	sgTextureFiles.cpp
//	iSDGE
//
//	Created by Nils Daumann on 23.03.10.
//	Copyright (c) 2011 Nils Daumann

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

#include "sgTextureFiles.h"

#include "sgXML.h"
#include "sgResourceManager.h"
#include "sgTexture.h"
#include "sgDebug.h"

#if defined __IOS__
	#include "CoreFoundation/CoreFoundation.h"
#else
	#include <png.h>
	#include <cstdlib>
	#include <cstring>
#endif

#define PVR_TEXTURE_FLAG_TYPE_MASK  0xff

namespace sgTextureFiles
{
	static char gPVRTexIdentifier[] = "PVR!";

	enum
	{
		kPVRTextureFlagTypePVRTC_2 = 11,
		kPVRTextureFlagTypePVRTC_4,

		kPVRTextureFlagTypePVRTC_2_GL = 24,
		kPVRTextureFlagTypePVRTC_4_GL
	};

	bool loadPNG(sgUncompressedTexture **tex, const char *filename)
	{
#if defined __IOS__
		const char *filepath = sgResourceManager::getPath(filename);
		CGDataProviderRef texturefiledata = CGDataProviderCreateWithFilename(filepath);
		if(!texturefiledata)
		{
			return false;
		}
		delete[] filepath;
		CGImageRef textureImage = CGImageCreateWithPNGDataProvider(texturefiledata, NULL, true, kCGRenderingIntentDefault);
		CGDataProviderRelease(texturefiledata);

		*tex = new sgUncompressedTexture;
		sgUncompressedTexture *texture = *tex;
		texture->hasalpha = true;
		texture->bytes = NULL;

		texture->width = CGImageGetWidth(textureImage);
		texture->height = CGImageGetHeight(textureImage);

		texture->bytes = new unsigned char[texture->width*texture->height*4];
		CGContextRef textureContext = CGBitmapContextCreate(texture->bytes, texture->width, texture->height, 8, texture->width*4, CGImageGetColorSpace(textureImage), kCGImageAlphaPremultipliedLast);
		CGContextDrawImage(textureContext, CGRectMake(0.0, 0.0, (float)texture->width, (float)texture->height), textureImage);

		CFRelease(textureContext);
		CGImageRelease(textureImage);

		return true;
#else
		png_structp png_ptr;
		png_infop info_ptr;
		unsigned int sig_read = 0;
		int color_type, interlace_type;
		FILE *fp;

		const char *filepath = sgResourceManager::getPath(filename);
		if ((fp = fopen(filepath, "rb")) == NULL)
		{
			delete[] filepath;
			return false;
		}
		delete[] filepath;

		/* Create and initialize the png_struct
		 * with the desired error handler
		 * functions.  If you want to use the
		 * default stderr and longjump method,
		 * you can supply NULL for the last
		 * three parameters.  We also supply the
		 * the compiler header file version, so
		 * that we know if the application
		 * was compiled with a compatible version
		 * of the library.  REQUIRED
		 */
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				NULL, NULL, NULL);

		if (png_ptr == NULL)
		{
			fclose(fp);
			return false;
		}

		/* Allocate/initialize the memory
		 * for image information.  REQUIRED. */
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return false;
		}

		/* Set error handling if you are
		 * using the setjmp/longjmp method
		 * (this is the normal method of
		 * doing things with libpng).
		 * REQUIRED unless you  set up
		 * your own error handlers in
		 * the png_create_read_struct()
		 * earlier.
		 */
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			/* Free all of the memory associated
			 * with the png_ptr and info_ptr */
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fp);
			/* If we get here, we had a
			 * problem reading the file */
			return false;
		}

		/* Set up the output control if
		 * you are using standard C streams */
		png_init_io(png_ptr, fp);

		/* If we have already
		 * read some of the signature */
		png_set_sig_bytes(png_ptr, sig_read);

		/*
		 * If you have enough memory to read
		 * in the entire image at once, and
		 * you need to specify only
		 * transforms that can be controlled
		 * with one of the PNG_TRANSFORM_*
		 * bits (this presently excludes
		 * dithering, filling, setting
		 * background, and doing gamma
		 * adjustment), then you can read the
		 * entire image (including pixels)
		 * into the info structure with this
		 * call
		 *
		 * PNG_TRANSFORM_STRIP_16 |
		 * PNG_TRANSFORM_PACKING  forces 8 bit
		 * PNG_TRANSFORM_EXPAND forces to
		 *  expand a palette into RGB
		 */
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

		*tex = new sgUncompressedTexture;
		sgUncompressedTexture *texture = *tex;
		texture->bytes = NULL;

		texture->width = png_get_image_width(png_ptr, info_ptr);
		texture->height = png_get_image_height(png_ptr, info_ptr);
		switch(png_get_color_type(png_ptr, info_ptr))
		{
			case PNG_COLOR_TYPE_RGBA:
				texture->hasalpha = true;
				break;
			case PNG_COLOR_TYPE_RGB:
				texture->hasalpha = false;
				break;
			default:
//				std::cout << "Color type " << info_ptr->color_type << " not supported" << std::endl;
				delete tex;
				png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				fclose(fp);
				return false;
		}
		unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
		texture->bytes = (unsigned char*) malloc(row_bytes * texture->height);

		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

		for(int i = 0; i < texture->height; i++)
		{
			// note that png is ordered top to
			// bottom, but OpenGL expect it bottom to top
			// so the order or swapped
			memcpy(texture->bytes+(row_bytes * i/*(texture->height-1-i)*/), row_pointers[i], row_bytes);
		}

		/* Clean up after the read,
		 * and free any memory allocated */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		/* Close the file */
		fclose(fp);

		return true;
#endif
	}

	bool loadPVR(sgPVRTexture **tex, const char *filename)
	{
#if defined __IOS__
		bool success = false;

		unsigned long dataOffset = 0;
		unsigned long dataSize = 0;
		unsigned long blockSize = 0;
		unsigned long widthBlocks = 0;
		unsigned long heightBlocks = 0;
		unsigned long width = 0;
		unsigned long height = 0;
		unsigned long bpp = 4;

		*tex = new sgPVRTexture;
		sgPVRTexture *texture = *tex;
		texture->bytes = NULL;

		const char *filepath = sgResourceManager::getPath(filename);
		FILE *file = fopen(filepath, "rb");
		delete[] filepath;
		fread(texture, 4, 13, file);
		texture->pvrTag = CFSwapInt32LittleToHost(texture->pvrTag);

		if(gPVRTexIdentifier[0] != ((texture->pvrTag >>  0) & 0xff) ||
			gPVRTexIdentifier[1] != ((texture->pvrTag >>  8) & 0xff) ||
			gPVRTexIdentifier[2] != ((texture->pvrTag >> 16) & 0xff) ||
			gPVRTexIdentifier[3] != ((texture->pvrTag >> 24) & 0xff))
		{
			return false;
		}

		texture->flags = CFSwapInt32LittleToHost(texture->flags);
		texture->flags = texture->flags & PVR_TEXTURE_FLAG_TYPE_MASK;

		if(texture->flags == kPVRTextureFlagTypePVRTC_4 || texture->flags == kPVRTextureFlagTypePVRTC_2)
		{
			if(texture->flags == kPVRTextureFlagTypePVRTC_4 || texture->flags == kPVRTextureFlagTypePVRTC_4_GL)
				texture->glformat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
			else if(texture->flags == kPVRTextureFlagTypePVRTC_2 || texture->flags == kPVRTextureFlagTypePVRTC_2_GL)
				texture->glformat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;

			width = texture->width = CFSwapInt32LittleToHost(texture->width);
			height = texture->height = CFSwapInt32LittleToHost(texture->height);
			texture->bitmaskAlpha = CFSwapInt32LittleToHost(texture->bitmaskAlpha);

			texture->dataLength = CFSwapInt32LittleToHost(texture->dataLength);

			texture->bytes = new unsigned char[texture->dataLength];
			fread(texture->bytes, texture->dataLength, 1, file);

			// Calculate the data size for each texture level and respect the minimum number of blocks
			while(dataOffset < texture->dataLength)
			{
				if(texture->flags == kPVRTextureFlagTypePVRTC_4)
				{
					blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
					widthBlocks = width / 4;
					heightBlocks = height / 4;
					bpp = 4;
				}else
				{
					blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
					widthBlocks = width / 8;
					heightBlocks = height / 4;
					bpp = 2;
				}

				// Clamp to minimum number of blocks
				if(widthBlocks < 2)
					widthBlocks = 2;
				if(heightBlocks < 2)
					heightBlocks = 2;

				dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
				texture->mipsizes.push_back(dataSize);

				dataOffset += dataSize;

				width = fmax(width >> 1, 1);
				height = fmax(height >> 1, 1);
			}

			success = true;
		}

		return success;
#else
		return false;
#endif
	}
}
