//
//	sgObjectFiles.h
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

#ifndef __SGTEXTUREFILES_H__
#define __SGTEXTUREFILES_H__

#include <vector>
#include <string>

/**
 * Object file namespace. Contains functionality to loat object files.
 */
namespace sgTextureFiles
{
	struct sgPVRTexture
	{
		unsigned long headerLength;
		unsigned long height;
		unsigned long width;
		unsigned long numMipmaps;
		unsigned long flags;
		unsigned long dataLength;
		unsigned long bpp;
		unsigned long bitmaskRed;
		unsigned long bitmaskGreen;
		unsigned long bitmaskBlue;
		unsigned long bitmaskAlpha;
		unsigned long pvrTag;
		unsigned long numSurfs;

		unsigned long glformat;
		unsigned char* bytes;
		std::vector<unsigned long> mipsizes;
	};

	struct sgUncompressedTexture
	{
		bool hasalpha;
		unsigned long height;
		unsigned long width;
		unsigned long dataLength;
		unsigned char* bytes;
	};

	/**
	 *	Load png.
	 *	Loads a png texture file.
	 * @param tex pointer to the datastructure which will contain the image information
	 * @param filename the textures filename
	 * @return true if it was successfull, false otherwise
	 */
	bool loadPNG(sgUncompressedTexture **tex, const char *filename);

	/**
	 *	Load pvr.
	 *	Loads a pvr compressed texture file.
	 * @param tex pointer to the datastructure which will contain the image information
	 * @param filename the textures filename
	 * @return true if it was successfull, false otherwise
	 */
	bool loadPVR(sgPVRTexture **tex, const char *filename);
}

#endif
