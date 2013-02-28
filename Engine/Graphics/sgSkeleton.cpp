//
//	sgSkeleton.cpp
//	iSDGE
//
//	Created by Nils Daumann on 21.09.11.
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

#include "sgSkeleton.h"
#include <cstring>

sgAnimationBone::sgAnimationBone(sgAnimationBone *prev, sgAnimationBone *next, const float frametime, const sgVector3 &pos, const sgVector3 &scal, const sgQuaternion &rot)
{
	time = frametime;
	position = pos;
	scale = scal;
	rotation = rot;
	
	prevframe = prev;
	nextframe = next;
}

sgAnimation::sgAnimation(const std::string &animname)
{
	name = animname;
}

sgAnimation::~sgAnimation()
{
	for (std::map<int, sgAnimationBone*>::iterator it = bones.begin(); it!=bones.end(); ++it)
	{
		sgAnimationBone *bone = it->second;
		sgAnimationBone *first = bone;
		while(bone)
		{
			bone = bone->nextframe;
			delete bone->prevframe;
			if(bone == first)
				break;
		}
		if(bone != first)
			delete bone;
	}
}

sgBone::sgBone(sgVector3 &pos, std::string bonename, bool root)
{
	invbasematrix.makeTranslate(pos*(-1.0f));
	
	name = bonename;
	isroot = root;
	
	position = pos;
	rotation.makeIdentity();
	scale = sgVector3(1.0, 1.0, 1.0);
	
	currframe = 0;
	nextframe = 0;
	currtime = 0.0f;
}

void sgBone::init(sgBone *parent)
{
	for(int i = 0; i < children.size(); i++)
	{
		children[i]->init(this);
	}
	
	if(parent != 0)
		position -= parent->position;
	relbasematrix.makeTranslate(position);
	position = 0.0f;
}

void sgBone::update(sgBone *parent, float timestep)
{
	if(currframe != 0 && nextframe != 0)
	{
		currtime += timestep;
		while(currtime > nextframe->time)
		{
			if(currframe->time > nextframe->time)
				currtime = nextframe->time;
			currframe = nextframe;
			nextframe = nextframe->nextframe;
			timediff = nextframe->time-currframe->time;
		}
		
		float blend = (currtime-currframe->time)/timediff;
		position.makeLerp(currframe->position, nextframe->position, blend);
		scale.makeLerp(currframe->scale, nextframe->scale, blend);
		rotation.makeLerpS(currframe->rotation, nextframe->rotation, blend);
	}
	
	finalmatrix = relbasematrix;
	finalmatrix.translate(position);
	finalmatrix.scale(scale);
	finalmatrix.rotate(rotation);
	if(parent != 0)
	{
		finalmatrix = parent->finalmatrix*finalmatrix;
	}
	
	for(int i = 0; i < children.size(); i++)
	{
		children[i]->update(this, timestep);
	}
	
	finalmatrix = finalmatrix*invbasematrix;
}

void sgBone::setAnimation(sgAnimationBone *animbone)
{
	currtime = 0.0f;
	currframe = animbone;
	nextframe = animbone->nextframe;
	timediff = nextframe->time-currframe->time;
}



sgSkeleton::sgSkeleton()
{

}

void sgSkeleton::init()
{
	for(int i = 0; i < bones.size(); i++)
	{
		if(bones[i].isroot)
		{
			bones[i].init();
		}
	}
}

void sgSkeleton::update(float timestep)
{
	for(int i = 0; i < bones.size(); i++)
	{
		if(bones[i].isroot)
		{
			bones[i].update(0, timestep);
		}
	}
	
	for(int i = 0; i < bones.size(); i++)
	{
		memcpy(matrices+16*i, &bones[i].finalmatrix, 16*sizeof(float));
//		printf("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", matrices[i*16+0], matrices[i*16+1], matrices[i*16+2], matrices[i*16+3], matrices[i*16+4], matrices[i*16+5], matrices[i*16+6], matrices[i*16+7], matrices[i*16+8], matrices[i*16+9], matrices[i*16+10], matrices[i*16+11], matrices[i*16+12], matrices[i*16+13], matrices[i*16+14], matrices[i*16+15]);
	}
}

void sgSkeleton::setAnimation(const std::string &animname)
{
	sgAnimation *anim = animations[animname];
	if(!anim)
	{
		return;
	}
	for(int i = 0; i < bones.size(); i++)
	{
		sgAnimationBone *temp = anim->bones[i];
		if(temp)
			bones[i].setAnimation(temp);
	}
}
