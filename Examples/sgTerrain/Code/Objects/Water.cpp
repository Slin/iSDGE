//
//	Bob.cpp
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

#include "Water.h"

void Water::onInit(sgEntity *e)
{
	ent = e;
	
	ent->obj->materials[0]->setTexture2D(-1, "waterbump.png");
	ent->obj->materials[0]->setTexture2D(-1, "sky.png");
	ent->obj->materials[0]->setShader("Water", "Water");
	ent->obj->materials[0]->mattex.makeScale(sgVector3(100, 100, 1));
}

//This method is called every frame, just before drawing for each object with this action attached
void Water::onDraw(float timestep)
{
	ent->obj->materials[0]->mattex.mat[12] += 0.1*timestep;
}