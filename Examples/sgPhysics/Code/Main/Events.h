//
//	Events.h
//	Engine
//
//	Created by Nils Daumann on 01.05.10.
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

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "sgEvents.h"
#include "sgMain.h"
#include "sgDebug.h"

#include "CameraFree.h"
#include "FPSDisplay.h"

#include "AutoDoor.h"


class Events : public sgEvents
{
	public:
		void onInit(sgMain *m);
		void onDraw(float timestep);
		
	private:
		sgMain *sgmain;
		sgEntity *cam;
};

#endif
