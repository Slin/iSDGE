//
//	sgPhysConstraint.h
//	Engine
//
//	Created by Nils Daumann on 04.04.11.
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

#ifndef __SGPHYSCONSTRAINT_H__
#define __SGPHYSCONSTRAINT_H__

#include "sgVector3.h"
#include "sgQuaternion.h"

class sgPhysBody;

/**
 *	Physics body. Represents a body within a physics world handled by a physics engine.
 */
class sgPhysConstraint
{
	public:
		/**
		 *	Body shapes. The supported shapes.
		 */
		enum eType
		{
			ET_POINT,
			ET_HINGE,
			ET_SLIDER
		};
	
		/**
		 *	Constructor.
		 *	Initializes a point constraint.
		 *	@param tp the type of the constraint
		 */
		sgPhysConstraint(eType tp);
	
		/**
		 *	Destructor.
		 *	Cleans everything up.
		 */
		~sgPhysConstraint();
	
		/**
		 *	Initialize.
		 *	Initializes a point constraint.
		 *	@param tp the type of the constraint
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 */
		virtual void initPoint(sgPhysBody *bdA, sgVector3 pivposA){}
	
		/**
		 *	Initialize.
		 *	Initializes a point constraint.
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 *	@param bdB pointer to the second physics body the constraint is meant to be attached to
		 *	@param pivBpos position within the second body to attach the constraint to
		 */
		virtual void initPoint(sgPhysBody *bdA, sgVector3 pivApos, sgPhysBody *bdB, sgVector3 pivBpos){}
	
		/**
		 *	Initialize.
		 *	Initializes a hinge constraint.
		 *	@param bdA pointer to the physics body the constraint is meant to be attached to
		 *	@param pivApos position within the body to attach the constraint to
		 *	@param axisA axis the body is meant to turn around
		 */
		virtual void initHinge(sgPhysBody *bdA, sgVector3 pivApos, sgVector3 axisA){}
	
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
		virtual void initHinge(sgPhysBody *bdA, sgVector3 pivApos, sgVector3 axisA, sgPhysBody *bdB, sgVector3 pivBpos, sgVector3 axisB){}
	
		/**
		 *	Destroy.
		 *	Destroys the constraint.
		 */
		virtual void destroy(){}
	
	
		virtual void setPointTau(float val){}
	
	
		virtual void setPointDamping(float val){}
	
	
		virtual void setPointClamp(float val){}
	
	
		virtual void setPointPivotA(sgVector3 pos){}
	
	
		virtual void setPointPivotB(sgVector3 pos){}
	
	
		virtual void setHingeLimit(float low, float high){}
		
	protected:
		eType type;
};

#endif
