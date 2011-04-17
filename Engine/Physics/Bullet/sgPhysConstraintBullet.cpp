//
//	sgPhysConstraintBullet.cpp
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

#include "sgPhysConstraintBullet.h"
#include "sgPhysWorldBullet.h"

sgPhysConstraintBullet::sgPhysConstraintBullet(eType tp, sgPhysWorldBullet *wld)
	:sgPhysConstraint(tp), world(wld)
{
	
}

sgPhysConstraintBullet::~sgPhysConstraintBullet()
{
	if(constraint)
	{
		delete constraint;
	}
}

void sgPhysConstraintBullet::initPoint(sgPhysBody *bdA, sgVector3 pivApos)
{
	if(type != ET_POINT)
		return;
	
	btRigidBody *body = (btRigidBody *)bdA->getBody();
	btVector3 lpiv = body->getCenterOfMassTransform().inverse()*btVector3(pivApos.x, pivApos.y, pivApos.z);
	
	constraint = new btPoint2PointConstraint(*body, lpiv);
	world->dynamicsWorld->addConstraint(constraint, false);
	bdA->constraint = this;
}

void sgPhysConstraintBullet::initPoint(sgPhysBody *bdA, sgVector3 pivApos, sgPhysBody *bdB, sgVector3 pivBpos)
{
	if(type != ET_POINT)
		return;
	
	btRigidBody *body = (btRigidBody *)bdA->getBody();
	btRigidBody *bodyb = (btRigidBody *)bdB->getBody();
	btVector3 lpiv = body->getCenterOfMassTransform().inverse()*btVector3(pivApos.x, pivApos.y, pivApos.z);
	btVector3 lpivb = bodyb->getCenterOfMassTransform().inverse()*btVector3(pivBpos.x, pivBpos.y, pivBpos.z);

	constraint = new btPoint2PointConstraint(*body, *bodyb, lpiv, lpivb);
	world->dynamicsWorld->addConstraint(constraint, false);
	bdA->constraint = this;
}

void sgPhysConstraintBullet::initHinge(sgPhysBody *bdA, sgVector3 pivApos, sgVector3 axisA)
{
	if(type != ET_HINGE)
		return;
	
	btRigidBody *body = (btRigidBody *)bdA->getBody();
	btVector3 lpiv = /*body->getCenterOfMassTransform().inverse()**/btVector3(pivApos.x, pivApos.y, pivApos.z);
	btVector3 laxis = /*body->getCenterOfMassTransform().inverse()**/btVector3(axisA.x, axisA.y, axisA.z);
	
	constraint = new btHingeConstraint(*body, lpiv, laxis);
	world->dynamicsWorld->addConstraint(constraint, false);
	bdA->constraint = this;
}

void sgPhysConstraintBullet::initHinge(sgPhysBody *bdA, sgVector3 pivApos, sgVector3 axisA, sgPhysBody *bdB, sgVector3 pivBpos, sgVector3 axisB)
{
	if(type != ET_HINGE)
		return;
	
	btRigidBody *body = (btRigidBody *)bdA->getBody();
	btRigidBody *bodyb = (btRigidBody *)bdB->getBody();
	btVector3 lpiv = body->getCenterOfMassTransform().inverse()*btVector3(pivApos.x, pivApos.y, pivApos.z);
	btVector3 lpivb = bodyb->getCenterOfMassTransform().inverse()*btVector3(pivBpos.x, pivBpos.y, pivBpos.z);
	btVector3 laxis = body->getCenterOfMassTransform().inverse()*btVector3(axisA.x, axisA.y, axisA.z);
	btVector3 laxisb = bodyb->getCenterOfMassTransform().inverse()*btVector3(axisB.x, axisB.y, axisB.z);
	
	constraint = new btHingeConstraint(*body, *bodyb, lpiv, lpivb, laxis, laxisb);
	world->dynamicsWorld->addConstraint(constraint, false);
	bdA->constraint = this;
}

void sgPhysConstraintBullet::destroy()
{
	world->dynamicsWorld->removeConstraint(constraint);
	world->destroyConstraint(this);
}

void sgPhysConstraintBullet::setPointTau(float val)
{
	if(type != ET_POINT)
		return;
	
	((btPoint2PointConstraint *)constraint)->m_setting.m_tau = val;
}

void sgPhysConstraintBullet::setPointDamping(float val)
{
	if(type != ET_POINT)
		return;
	
	((btPoint2PointConstraint *)constraint)->m_setting.m_damping = val;
}

void sgPhysConstraintBullet::setPointClamp(float val)
{
	if(type != ET_POINT)
		return;
	
	((btPoint2PointConstraint *)constraint)->m_setting.m_impulseClamp = val;
}

void sgPhysConstraintBullet::setPointPivotA(sgVector3 pos)
{
	if(type != ET_POINT)
		return;
	
	((btPoint2PointConstraint *)constraint)->setPivotA(btVector3(pos.x, pos.y, pos.z));
}

void sgPhysConstraintBullet::setPointPivotB(sgVector3 pos)
{
	if(type != ET_POINT)
		return;
	
	((btPoint2PointConstraint *)constraint)->setPivotB(btVector3(pos.x, pos.y, pos.z));
}

void sgPhysConstraintBullet::setHingeLimit(float low, float high)
{
	if(type != ET_HINGE)
		return;
	
	((btHingeConstraint *)constraint)->setLimit(low*SIMD_RADS_PER_DEG, high*SIMD_RADS_PER_DEG);
}
