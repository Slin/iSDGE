//
//	sgPhysWorldBullet.cpp
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

#include "sgPhysWorldBullet.h"
#include "sgPhysBodyBullet.h"
#include "sgPhysConstraintBullet.h"

sgPhysWorldBullet::sgPhysWorldBullet()
{
	// Build the broadphase
    broadphase = new btDbvtBroadphase();
	
    // Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
	
    // The actual physics solver
    solver = new btSequentialImpulseConstraintSolver;
	
    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
}

sgPhysWorldBullet::~sgPhysWorldBullet()
{
	for(int i = bodies.size()-1; i >= 0; i--)
		delete bodies[i];
	
	for(int i = constraints.size()-1; i >= 0; i--)
		delete constraints[i];
	
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}

void sgPhysWorldBullet::update(float timestep)
{
	dynamicsWorld->stepSimulation(timestep, 10);
}

sgPhysBody *sgPhysWorldBullet::getRigidBody(sgPhysBody::eShape shp, sgVector3 &pos, sgQuaternion &rot, sgVector3 &sz)
{
	sgPhysBody *bod = new sgPhysBodyBullet(shp, pos, rot, sz, this);
	bodies.push_back(bod);
	return bod;
}

sgPhysConstraint *sgPhysWorldBullet::getConstraint(sgPhysConstraint::eType tp)
{
	sgPhysConstraint *constraint = new sgPhysConstraintBullet(tp, this);
	constraints.push_back(constraint);
	return constraint;
}

void sgPhysWorldBullet::destroyRigidBody(sgPhysBody *body)
{
	for(int i = 0; i < bodies.size(); i++)
	{
		if(bodies[i] == body)
		{
			bodies.erase(bodies.begin()+i);
			delete body;
		}
	}
}

void sgPhysWorldBullet::destroyConstraint(sgPhysConstraint *cnstrt)
{
	for(int i = 0; i < bodies.size(); i++)
	{
		if(constraints[i] == cnstrt)
		{
			constraints.erase(constraints.begin()+i);
			delete cnstrt;
		}
	}
}

bool sgPhysWorldBullet::traceRay(sgVector3 from, sgVector3 to, sgTraceResult &res)
{
	btVector3 btfrom(from.x, from.y, from.z);
	btVector3 btto(to.x, to.y, to.z);
	btCollisionWorld::ClosestRayResultCallback cb(btfrom, btto);
	dynamicsWorld->rayTest(btfrom, btto, cb);
	
	if(cb.hasHit())
	{
		res.position = sgVector3(cb.m_hitPointWorld.x(), cb.m_hitPointWorld.y(), cb.m_hitPointWorld.z());
		res.normal = sgVector3(cb.m_hitNormalWorld.x(), cb.m_hitNormalWorld.y(), cb.m_hitNormalWorld.z());
		if(cb.m_collisionObject)
			res.entity = cb.m_collisionObject->getUserPointer();
		else
			res.entity = 0;
		
		return true;
	}else
	{
		res.entity = 0;
		return false;
	}
}

bool sgPhysWorldBullet::traceShape(sgPhysBody *bdy, sgVector3 from, sgVector3 to, sgTraceResult &res)
{	
	btTransform btfrom;
	btfrom.setRotation(btQuaternion(bdy->rotation.x, bdy->rotation.y, bdy->rotation.z, bdy->rotation.w));
	btfrom.setOrigin(btVector3(from.x, from.y, from.z));

	btTransform btto;
	btto.setRotation(btQuaternion(bdy->rotation.x, bdy->rotation.y, bdy->rotation.z, bdy->rotation.w));
	btto.setOrigin(btVector3(to.x, to.y, to.z));
	
	btCollisionWorld::ClosestConvexResultCallback cb(btVector3(from.x, from.y, from.z), btVector3(to.x, to.y, to.z));
	dynamicsWorld->convexSweepTest((btConvexShape*)bdy->getShape(), btfrom, btto, cb);
	
	if(cb.hasHit())
	{
		res.position = sgVector3(cb.m_hitPointWorld.x(), cb.m_hitPointWorld.y(), cb.m_hitPointWorld.z());
		res.normal = sgVector3(cb.m_hitNormalWorld.x(), cb.m_hitNormalWorld.y(), cb.m_hitNormalWorld.z());
		if(cb.m_hitCollisionObject)
			res.entity = cb.m_hitCollisionObject->getUserPointer();
			else
				res.entity = 0;
				
				return true;
	}else
	{
		res.entity = 0;
		return false;
	}
}
