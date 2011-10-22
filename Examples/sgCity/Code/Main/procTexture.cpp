//
//	procTexture.cpp
//	proc framework
//
//	Created by Nils Daumann on 18.10.11.
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

#include "procTexture.h"
#include "sgColor.h"
#include <cmath>
#include <cstdlib>

namespace procTexture
{
	void genBricks(sgTexture *tex, sgColorA color, BlendMode blendmode, float blendfactor, int xsize, int ysize, int widthx, int widthy)
	{
		float invfac = 1.0-blendfactor;
		for(int y = 0; y < tex->height; y++)
		{
			for(int x = 0; x < tex->width; x++)
			{
				int edgex = (x+((y/ysize)%2)*xsize/2)%xsize;
				int edgey = y%ysize;
				if(edgex >= xsize-(int)(widthx*0.5f) || edgex <= (int)(widthx*0.5f) || edgey >= ysize-(int)(widthy*0.5f) || edgey <= (int)(widthy*0.5f))
				{
					sgColorA col = tex->getPixel(x, y);
					switch(blendmode)
					{
						case BM_ADD:
							col.r = fmin(int(col.r+color.r*blendfactor), 255);
							col.g = fmin(int(col.g+color.g*blendfactor), 255);
							col.b = fmin(int(col.b+color.b*blendfactor), 255);
							col.a = fmin(int(col.a+color.a*blendfactor), 255);
							break;
						case BM_LINEAR:
							col.r = col.r*invfac+color.r*blendfactor;
							col.g = col.g*invfac+color.g*blendfactor;
							col.b = col.b*invfac+color.b*blendfactor;
							col.a = col.a*invfac+color.a*blendfactor;
							break;
						case BM_MULTIPLY:
							col.r *= color.r/255.0f*blendfactor;
							col.g *= color.g/255.0f*blendfactor;
							col.b *= color.b/255.0f*blendfactor;
							col.a *= color.a/255.0f*blendfactor;
							break;
					}
					tex->setPixel(x, y, col);
				}
			}
		}
	}
	
	void genNoise(sgTexture *tex, sgColorA color, BlendMode blendmode, float blendfactor)
	{
		float *noise = new float[tex->width*tex->height];
		for(int y = 0; y < tex->height; y++)
		{
			for(int x = 0; x < tex->width; x++)
			{
				noise[y*tex->width+x] = (float)rand()/(float)RAND_MAX;
				float invfac = 1.0-noise[y*tex->width+x]*blendfactor;
				
				sgColorA col = tex->getPixel(x, y);
				switch(blendmode)
				{
					case BM_ADD:
						col.r = fmin(int(col.r+color.r*noise[y*tex->width+x]*blendfactor), 255);
						col.g = fmin(int(col.g+color.g*noise[y*tex->width+x]*blendfactor), 255);
						col.b = fmin(int(col.b+color.b*noise[y*tex->width+x]*blendfactor), 255);
						col.a = fmin(int(col.a+color.a*noise[y*tex->width+x]*blendfactor), 255);
						break;
					case BM_LINEAR:
						col.r = col.r*invfac+color.r*noise[y*tex->width+x]*blendfactor;
						col.g = col.g*invfac+color.g*noise[y*tex->width+x]*blendfactor;
						col.b = col.b*invfac+color.b*noise[y*tex->width+x]*blendfactor;
						col.a = col.a*invfac+color.a*noise[y*tex->width+x]*blendfactor;
						break;
					case BM_MULTIPLY:
						col.r *= color.r/255.0f*noise[y*tex->width+x]*blendfactor;
						col.g *= color.g/255.0f*noise[y*tex->width+x]*blendfactor;
						col.b *= color.b/255.0f*noise[y*tex->width+x]*blendfactor;
						col.a *= color.a/255.0f*noise[y*tex->width+x]*blendfactor;
						break;
				}
				tex->setPixel(x, y, col);
			}
		}
		delete[] noise;
	}
}
