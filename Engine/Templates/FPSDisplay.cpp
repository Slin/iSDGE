//
//	FPSDisplay.cpp
//	Engine
//
//	Created by Nils Daumann on 03.06.10.
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

#include "FPSDisplay.h"

void FPSDisplay::onInit(sgEntity *e)
{
	ent = e;
	ent->pan->addText("FPS: 600", sgVector2(12, 18), sgVector2(0, 0), sgVector2(12, 18), "font1.png");
}

void FPSDisplay::onDraw(float timestep)
{
	//Update the displayed fps
	char *fpsstr = new char[5];
	sprintf(fpsstr, "%.0f0", (1.0f/timestep));
	((sgText*)ent->pan->elements[0])->str.str = std::string("FPS: ")+std::string(fpsstr);
	((sgText*)ent->pan->elements[0])->str.translate();
	delete[] fpsstr;
}
