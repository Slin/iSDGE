//
//	sgPhysConstraintBullet.h
//	Engine
//
//	Created by Nils Daumann on 01.04.11.
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

#ifndef __SGPHYSCONSTRAINTBULLET_H__
#define __SGPHYSCONSTRAINTBULLET_H__

#include "btBulletDynamicsCommon.h"
#include "sgPhysConstraint.h"

class sgPhysWorldBullet;

/**
 *	Physics body. Represents a body within a physics world handled by Bullet.
 */
class sgPhysConstraintBullet : public sgPhysConstraint
{
	public:
		/**
		 *	Constructor.
		 *	Constructs a constraint. Init() has to be called before the constraint can be used.
		 *	@param tp the type of the constraint
		 *	@param wld pointer to the physics world this body is meant to be handled by
		 */
		sgPhysConstraintBullet(eType tp, sgPhysWorldBullet *wld);
	
		/**
		 *	Destructor.
		 *	Cleans everything up.
		 */
		~sgPhysConstraintBullet();
	
		/**
		 *	Initialize.
		 *	Initializes a point constraint.
		 *	@param tp the type of the constraint
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 */
		void initPoint(sgPhysBody *bdA, sgVector3 pivposA);
		
		/**
		 *	Initialize.
		 *	Initializes a point constraint.
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 *	@param bdB pointer to the second physics body the constraint is meant to be attached to
		 *	@param pivBpos position within the second body to attach the constraint to
		 */
		void initPoint(sgPhysBody *bdA, sgVector3 pivApos, sgPhysBody *bdB, sgVector3 pivBpos);
		
		/**
		 *	Initialize.
		 *	Initializes a hinge constraint.
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 *	@param axisA axis the body is meant to turn around
		 */
		void initHinge(sgPhysBody *bdA, sgVector3 pivApos, sgVector3 axisA);
	
		/**
		 *	Initialize.
		 *	Initializes a hinge constraint.
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 *	@param axisA axis the body is meant to turn around
		 *	@param bdB pointer to the second physics body the constraint is meant to be attached to
		 *	@param pivBpos position within the second body to attach the constraint to
		 *	@param axisB axis the body is meant to turn around
		 */
		void initHinge(sgPhysBody *bdA, sgVector3 pivApos, sgVector3 axisA, sgPhysBody *bdB, sgVector3 pivBpos, sgVector3 axisB);
	
		/**
		 *	Destroy.
		 *	Destroys the constraint.
		 */
		void destroy();
	
	
		void setPointTau(float val);
	
	
		void setPointDamping(float val);
	
	
		void setPointClamp(float val);
	
	
		void setPointPivotA(sgVector3 pos);
	
	
		void setPointPivotB(sgVector3 pos);
	
	
		void setHingeLimit(float low, float high);
	
	private:
		sgPhysWorldBullet *world;
		btTypedConstraint *constraint;
};

#endif
