//
//	AnimTest.cpp
//	Engine
//
//	Created by Nils Daumann on 20.09.11.
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

#include "AnimTest.h"
#include "sgVector3.h"

void AnimTest::onInit(sgEntity *e)
{
	ent = e;
	time = (rand()%100)/20.0f;
	curr = rand()%4;
	
	ent->obj->body->materials[0]->setShader("anim.vsh", "anim.fsh");
	ent->obj->skeleton->init();
	ent->obj->skeleton = ent->obj->skeleton;
	
	if(curr == 0)
		ent->obj->skeleton->setAnimation(std::string("idle"));
	if(curr == 1)
		ent->obj->skeleton->setAnimation(std::string("walk"));
	if(curr == 2)
		ent->obj->skeleton->setAnimation(std::string("attack3"));
	if(curr == 3)
		ent->obj->skeleton->setAnimation(std::string("run"));
}

//This method is called every frame, just before drawing for each object with this action attached
void AnimTest::onDraw(float timestep)
{
	time += timestep;
	if(time > 5.0f)
	{
		curr += 1;
		if(curr > 3)
			curr = 0;
		time = 0.0f;
		if(curr == 0)
			ent->obj->skeleton->setAnimation(std::string("idle"));
		if(curr == 1)
			ent->obj->skeleton->setAnimation(std::string("walk"));
		if(curr == 2)
			ent->obj->skeleton->setAnimation(std::string("attack3"));
		if(curr == 3)
			ent->obj->skeleton->setAnimation(std::string("run"));
	}
	ent->obj->skeleton->update(timestep*24.0f);
}
