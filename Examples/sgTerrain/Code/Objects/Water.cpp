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
#include "sgObjectBody.h"
#include "sgTexture.h"

void Water::onInit(sgEntity *e)
{
	ent = e;
	
	//Create the reflection camera and create and assign a rendertarget
	mirror = ent->createCamEntity();
	mirror->cam->size = sgVector2(128, 128);
	mirror->cam->rendertarget = sgTexture::getTexture2D(128, 128);
	mirror->cam->rendertarget->makeRendertarget();
	mirror->cam->tag = 1;
	
	ent->obj->body->materials[0]->setTexture2D(-1, "waterbump.png");
	ent->obj->body->materials[0]->setTexture2D(-1, mirror->cam->rendertarget);
	ent->obj->body->materials[0]->setShader("Water", "Water");
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(100, 100, 1));
}

//This method is called every frame, just before drawing for each object with this action attached
void Water::onDrawLate(float timestep)
{
	ent->obj->body->materials[0]->mattex.mat[12] += 0.1*timestep;
	
	mirror->cam->position = ent->cam->position;
	mirror->cam->position.y = 2.0*ent->obj->position.y-mirror->cam->position.y;
	sgVector3 rot = ent->cam->rotation;
	rot.y *= -1;
	rot.z *= -1;
	mirror->cam->rotation = rot;
}
