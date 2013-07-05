//
//	sgSkeleton.h
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

#ifndef __SGSKELETON_H__
#define __SGSKELETON_H__

#include "sgBase.h"
#include "sgVector4.h"
#include "sgVector3.h"
#include "sgQuaternion.h"
#include "sgMatrix4x4.h"
#include <vector>
#include <string>
#include <map>


class sgAnimationBone
{
public:
	sgAnimationBone(sgAnimationBone *prev, sgAnimationBone *next, const float frametime, const sgVector3 &pos, const sgVector3 &scal, const sgQuaternion &rot);
	
	float time;
	
	sgVector3 position;
	sgVector3 scale;
	sgQuaternion rotation;
	
	sgAnimationBone *nextframe;
	sgAnimationBone *prevframe;
};

class sgAnimation : public sgBase
{
	public:
		sgAnimation(const std::string &animname);
		~sgAnimation();
	
		std::string name;
		std::map<int, sgAnimationBone*> bones;
};

class sgBone
{
	public:
		sgBone(sgVector3 &pos, std::string bonename, bool root);
		sgBone(const sgBone &other);
		
		void init(sgBone *parent = 0);
		void update(sgBone *parent = 0, float timestep = 0.01f);
	
		void setAnimation(sgAnimationBone *anim);
		
		sgMatrix4x4 relbasematrix;
		sgMatrix4x4 invbasematrix;
		
		sgVector3 position;
		sgQuaternion rotation;
		sgVector3 scale;
		
		sgMatrix4x4 finalmatrix;
		
		std::string name;
		bool isroot;
	
		std::vector<sgBone*> children;
		std::vector<int> tempchildren;
	
		sgAnimationBone *currframe;
		sgAnimationBone *nextframe;
	
		float currtime;
		float timediff;
};

/**
 * Skeleton class. Responsible for skeletal animations.
 */
class sgSkeleton : public sgBase
{
	public:
		/**
		 *	Constructor.
		 *	Creates a skeleton.
		 */
		sgSkeleton();
		sgSkeleton(sgSkeleton *skeleton);
	
		~sgSkeleton();
	
		void init();
		void update(float timestep);
		void setAnimation(const std::string &animname);
	
		std::vector<sgBone> bones;
		std::map<std::string, sgAnimation*> animations;
		float *matrices;
};

#endif
