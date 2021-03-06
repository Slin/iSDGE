//
//	Camera.h
//	Engine
//
//	Created by Nils Daumann on 10.05.10.
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

#ifndef __CAMERAFREE_H__
#define __CAMERAFREE_H__

#include <cmath>

#include "sgAction.h"
#include "sgTouches.h"
#include "sgAccelerometer.h"
#include "sgMouse.h"

class CameraFree : public sgAction
{
	public:
		CameraFree(float sp = 1.0, bool acc = false);

		void makeStereo(float dist, float xoffset = 0.0f);
		void makeAnaglyph(float dist, const char *fs);
		void onDraw(float timestep);

	private:
		float speed;
		float stereodist;
        bool acccontrol;
		sgCamera *stereo_cam;
        sgVector2 lefttouchstart;
        sgVector2 righttouchstart;
};

#endif
