//
//	TestPart.cpp
//	Engine
//
//	Created by Nils Daumann on 15.02.11.
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

#include "BlubPart.h"
#include "sgVector3.h"
#include "sgCamera.h"

#include "stdlib.h"

BlubPart::BlubPart(sgCamera *obj)
{
	spawncam = obj;
	position = spawncam->position+sgVector3((rand()%600)*0.01-3.0, -3, (rand()%600)*0.01-3);
	scale = 0.1;
	color.w = 0.3;
	speed = 0;
}

//This method is called every frame, for the particle
void BlubPart::onDraw(float timestep)
{
	speed += timestep*2.0;
	position.y += timestep*speed;
	scale += 0.1*timestep;

	if(position.y >= 23.0)
		destroy = true;
}
