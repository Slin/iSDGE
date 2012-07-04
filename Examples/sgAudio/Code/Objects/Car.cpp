//
//	Car.cpp
//	Engine
//
//	Created by Nils Daumann on 30.07.11.
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

#include "Car.h"
#include "sgVector3.h"
#include "sgSound.h"

void Car::onInit(sgEntity *e)
{
	ent = e;

	ent->createSndSource();
#if defined __IOS__
	unsigned int hndl = ent->sndsrc->registerSound(sgSound::getSound("engine.caf"));
#else
	unsigned int hndl = ent->sndsrc->registerSound(sgSound::getSound("engine.wav"));
#endif
	ent->sndsrc->playSound(hndl, true, 5.0, 1.5);
}

//This method is called every frame, just before drawing for each object with this action attached
void Car::onDraw(float timestep)
{
	if(ent->obj->position.z > 50.0f)
		ent->obj->position.z = -50.0f;
	ent->obj->position.z += timestep*20.0f;
	ent->sndsrc->position = ent->obj->position;
	ent->obj->updateObject();
}
