//
//	sgCharacterControllerBullet.cpp
//	Engine
//
//	Created by Nils Daumann on 09.04.11.
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

#include "sgCharacterControllerBullet.h"
#include "sgPhysWorldBullet.h"

sgCharacterControllerBullet::sgCharacterControllerBullet(eShape shp, sgVector3 &pos, sgQuaternion &rot, sgVector3 &sz, sgPhysWorldBullet *wld)
: sgCharacterController(shp, pos, rot, sz), world(wld), shape(0), body(0), trimesh(0)
{
	switch(shp)
	{
		case ES_BOX:
			shape = new btBoxShape(btVector3(sz.x, sz.y, sz.z));
			break;
			
		case ES_SPHERE:
			shape = new btSphereShape(sz.length());
			break;
			
		case ES_MESH:
			trimesh = new btTriangleMesh();
			break;
	}
}

sgCharacterControllerBullet::~sgCharacterControllerBullet()
{
	if(body)
	{
//		world->dynamicsWorld->removeRigidBody(body);
		delete body;
	}
	
	if(shape)
		delete shape;
	
	if(trimesh)
		delete trimesh;
}

void sgCharacterControllerBullet::addFace(sgVector3 vert1, sgVector3 vert2, sgVector3 vert3)
{
	if(!trimesh || initialized)
		return;
	
	trimesh->addTriangle(btVector3(vert1.x, vert1.y, vert1.z), btVector3(vert2.x, vert2.y, vert2.z), btVector3(vert3.x, vert3.y, vert3.z));
}

void sgCharacterControllerBullet::init(float mass, void *userdata)
{
	if(trimesh)
		shape = new btBvhTriangleMeshShape(trimesh, true);
	
	btVector3 inertia(0, 0, 0);
	if(!trimesh)
		shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo bodyci(mass, this, shape, inertia);
	body = new btRigidBody(bodyci);
	body->setUserPointer(userdata);
	world->dynamicsWorld->addRigidBody(body);
	
	initialized = true;
}

void sgCharacterControllerBullet::destroy()
{
	world->dynamicsWorld->removeRigidBody(body);
//	world->destroyRigidBody(this);
}

bool sgCharacterControllerBullet::isStatic()
{
	return body->isStaticObject();
}

void sgCharacterControllerBullet::applyCentralImpulse(sgVector3 impulse)
{
	body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

void sgCharacterControllerBullet::setFriction(float friction)
{
	body->setFriction(friction);
}

void sgCharacterControllerBullet::setAlwaysActive(bool active)
{
	if(active)
		body->forceActivationState(DISABLE_DEACTIVATION);
	else
		body->forceActivationState(ACTIVE_TAG);
}

void sgCharacterControllerBullet::setKinematic(bool enable)
{
	if(enable)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
}

void *sgCharacterControllerBullet::getBody()
{
	return (void*)body;
}

void sgCharacterControllerBullet::getWorldTransform(btTransform &worldTrans) const
{
	worldTrans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
	worldTrans.setOrigin(btVector3(position.x, position.y, position.z));
}

void sgCharacterControllerBullet::setWorldTransform(const btTransform &worldTrans)
{
	btQuaternion rot = worldTrans.getRotation();
	rotation = sgQuaternion(rot.x(), rot.y(), rot.z(), rot.w());
	btVector3 pos = worldTrans.getOrigin();
	position = sgVector3(pos.x(), pos.y(), pos.z());
}
