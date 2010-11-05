//
//	sgRendererES2.h
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

#ifndef __SGRENDERERES2_H__
#define __SGRENDERERES2_H__

#include "sgRenderer.h"

/**
 * OpenGL ES 2.0 renderer class. This is the renderer class which will render the scene if only OpenGL ES 2.0 is supported. Don´t touch this and use sgRenderer instead.
 */
class sgRendererES2 : public sgRenderer
{
	public:
		sgRendererES2();
		~sgRendererES2();
	
		void render();
		bool resizeBuffers();
		void setMultisampling(unsigned short samples);
	
		void setMaterial(sgMaterial *mat);
		void renderObjects(sgCamera *cam, sgObject *first);
		void renderShadowVolumes(sgObject *first);
		void renderShadows(sgObject *first);
		void renderPanels(sgPanel *first);
	
		sgMaterial *shadowvolume;
		sgMaterial *shadowquad;
};

#endif
