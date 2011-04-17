//
//	sgPhysWorldBullet.h
//	Engine
//
//	Created by Nils Daumann on 30.03.11.
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

#ifndef __SGPHYSWORLDBULLET_H__
#define __SGPHYSWORLDBULLET_H__

#include "btBulletDynamicsCommon.h"
#include "sgPhysWorld.h"
#include <vector>

class sgPhysBodyBullet;

/**
 *	Physics world. Represents a phyiscs world handled by Bullet.
 */
class sgPhysWorldBullet : public sgPhysWorld
{
	public:
		/**
		 *	Constructor.
		 *	Initializes the physics world.
		 */
		sgPhysWorldBullet();
	
		/**
		 *	Destructor.
		 *	Cleans everything up.
		 */
		~sgPhysWorldBullet();
		
		/**
		 *	Update.
		 *	Updates the world by the given amount of time.
		 *	@param timestep the time since the last update to update the world with.
		 */
		void update(float timestep);
	
		/**
		 *	Get rigid body.
		 *	Creates a rigid body, adds it to the world and returns a pointer to it. Has to be initialized afterwards by calling its init method!
		 *	@param shp the shape to use (ES_MESH is always static)
		 *	@param pos the position vector to effect
		 *	@param rot the rotation quaternion to effect
		 *	@param sz the size of the shape (ignored for ES_MESH)
		 *	@return pointer to the new rigid body
		 */
		sgPhysBody *getRigidBody(sgPhysBody::eShape shp, sgVector3 &pos, sgQuaternion &rot, sgVector3 &sz);
	
		/**
		 *	Get constraint.
		 *	Creates a constraint, adds it to the world and returns a pointer to it. Has to be initialized afterwards!
		 *	@param tp the type of the constraint
		 *	@return pointer to the new constraint
		 */
		sgPhysConstraint *getConstraint(sgPhysConstraint::eType tp);
	
		/**
		 *	Destroy rigid body.
		 *	Completely destroys a rigid body.
		 *	@param body pointer to the body to destroy.
		 */
		void destroyRigidBody(sgPhysBody *body);
	
		/**
		 *	Destroy constraint.
		 *	Completely destroys a constraint.
		 *	@param cnstrt pointer to the constraint to destroy.
		 */
		void destroyConstraint(sgPhysConstraint *cnstrt);
	
		/**
		 *	Trace ray
		 *	Gets the surfaces the given ray intersects.
		 *	@param from the start position of the ray
		 *	@param to the end position of the ray
		 *	@return true if something was hit
		 */
		bool traceRay(sgVector3 from, sgVector3 to, sgTraceResult &res);
	
		/**
		 *	Bullet world.
		 *	The physics world.
		 */
		btDiscreteDynamicsWorld* dynamicsWorld;
	
	private:
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
	
		std::vector<sgPhysBody *> bodies;
		std::vector<sgPhysConstraint *> constraints;
};

#endif
