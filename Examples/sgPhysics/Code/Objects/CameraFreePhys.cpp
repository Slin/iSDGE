//
//	Camera.cpp
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

#include "CameraFreePhys.h"
#include "sgMain.h"
#include "sgDebug.h"
#include "PhysicsObject.h"

CameraFreePhys::CameraFreePhys(float sp)
{
	speed = sp;
	counter = 0;
	picked = NULL;
}

void CameraFreePhys::onDraw(float timestep)
{
	timestep *= 16.0f;

	sgVector2 toll;
#if defined __IOS__ || defined __ANDROID__
#if defined(__OUYA__)
	toll.y = -sgGamepad::leftjoy.y;
	toll.x = sgGamepad::leftjoy.x;
	
	if(toll.length() < 0.2f)
		toll = 0.0f;
#else
	if(sgAccelerometer::curracc.x > 0.1f || sgAccelerometer::curracc.x < -0.1f)
	{
		toll.x = sgAccelerometer::curracc.x;
	}else
	{
		toll.x = 0.0;
	}
	if(sgAccelerometer::curracc.y > 0.1f || sgAccelerometer::curracc.y < -0.1f)
	{
		toll.y = sgAccelerometer::curracc.y;
	}else
	{
		toll.y = 0.0;
	}
#endif
#if defined(__OUYA__)
	if(sgGamepad::lefttrigger > 0.5f)
	{
		if(!picked)
		{
			sgTraceResult res;
			sgVector3 topos(0, 0, 1.0);
			topos = ent->cam->camToWorld(topos);
			ent->sgmain->physworld->traceRay(ent->cam->position, topos, res);
			picked = (sgEntity*)res.entity;
			if(!picked || (picked && picked->body->isStatic()))
			{
				picked = NULL;
			}else
			{
				sgPhysConstraint *costrnt = ent->sgmain->physworld->getConstraint(sgPhysConstraint::ET_POINT);
				costrnt->initPoint(picked->body, res.position);
				costrnt->setPointTau(0.0001f);
				costrnt->setPointClamp(10.0f);
				picked->body->setAlwaysActive(true);
				pickdist = (picked->obj->position-ent->cam->position).length();
			}
		}else
		{
			sgVector3 topos(0, 0, 1.0);
			topos = ent->cam->camToWorld(topos);
			topos -= ent->cam->position;
			topos.normalize();
			topos *= pickdist;
			topos += ent->cam->position;
			picked->body->constraint->setPointPivotB(topos);
		}
	}
	else
	{
		if(picked)
		{
			picked->body->setAlwaysActive(false);
			picked->body->constraint->destroy();
			picked = NULL;
		}
	}

#else
	if(sgTouches::touches.size() == 0)
	{
		if(picked)
		{
			picked->body->setAlwaysActive(false);
			picked->body->constraint->destroy();
			picked = NULL;
		}
	}else if(sgTouches::touches.size() == 1)
	{
		if(!picked)
		{
			sgTraceResult res;
			sgVector3 topos(sgTouches::touches[0]->position.y/(ent->sgmain->renderer->backingWidth/sgRenderer::scaleFactor)*2.0-1.0, -(sgTouches::touches[0]->position.x/(ent->sgmain->renderer->backingHeight/sgRenderer::scaleFactor)*2.0-1.0), 1.0);
			topos = ent->cam->camToWorld(topos);
			ent->sgmain->physworld->traceRay(ent->cam->position, topos, res);
			picked = (sgEntity*)res.entity;
			if(!picked || (picked && picked->body->isStatic()))
			{
				picked = NULL;
			}else
			{
				sgPhysConstraint *costrnt = ent->sgmain->physworld->getConstraint(sgPhysConstraint::ET_POINT);
				costrnt->initPoint(picked->body, res.position);
				costrnt->setPointTau(0.0001f);
				costrnt->setPointClamp(10.0f);
				picked->body->setAlwaysActive(true);
				pickdist = (picked->obj->position-ent->cam->position).length();
			}
		}else
		{
			sgVector3 topos(sgTouches::touches[0]->position.y/(ent->sgmain->renderer->backingWidth/sgRenderer::scaleFactor)*2.0-1.0, -(sgTouches::touches[0]->position.x/(ent->sgmain->renderer->backingHeight/sgRenderer::scaleFactor)*2.0-1.0), 1.0);
			topos = ent->cam->camToWorld(topos);
			topos -= ent->cam->position;
			topos.normalize();
			topos *= pickdist;
			topos += ent->cam->position;
			picked->body->constraint->setPointPivotB(topos);
		}
	}
#endif

#if defined(__OUYA__)
	sgVector3 rot(sgGamepad::rightjoy.x*10.0, 0.0f, sgGamepad::rightjoy.y*10.0);
	
	if(rot.length() < 1.0f)
		rot = 0.0f;
	
	ent->cam->rotation -= rot*timestep;
#else
	else if(sgTouches::touches.size() == 2)
	{
		sgVector3 rot(sgTouches::touches[0]->direction.x, 0.0f, -sgTouches::touches[0]->direction.y);
		ent->cam->rotation += rot;
		sgTouches::touches[0]->direction.x = 0;
		sgTouches::touches[0]->direction.y = 0;
	}
#endif

	shake = 0.0f;//shake*0.5+(fabs(toll.x)+fabs(toll.y))*0.5;
#else
	toll.y = glfwGetKey('W')-glfwGetKey('S');
	toll.x = glfwGetKey('D')-glfwGetKey('A');

	sgVector3 rot(sgMouse::currdir.x, 0.0f, sgMouse::currdir.y);
	ent->cam->rotation -= rot*timestep;

	if(glfwGetKey(GLFW_KEY_SPACE))
	{
		shake = 10.0;
	}
	else
	{
		shake = 0.0;
	}

	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
	{
		if(!picked)
		{
			sgTraceResult res;
			sgVector3 topos(0, 0, 1.0);
			topos = ent->cam->camToWorld(topos);
			ent->sgmain->physworld->traceRay(ent->cam->position, topos, res);
			picked = (sgEntity*)res.entity;
			if(!picked || (picked && picked->body->isStatic()))
			{
				picked = NULL;
			}else
			{
				sgPhysConstraint *costrnt = ent->sgmain->physworld->getConstraint(sgPhysConstraint::ET_POINT);
				costrnt->initPoint(picked->body, res.position);
				costrnt->setPointTau(0.0001f);
				costrnt->setPointClamp(10.0f);
				picked->body->setAlwaysActive(true);
				pickdist = (picked->obj->position-ent->cam->position).length();
			}
		}else
		{
			sgVector3 topos(0, 0, 1.0);
			topos = ent->cam->camToWorld(topos);
			topos -= ent->cam->position;
			topos.normalize();
			topos *= pickdist;
			topos += ent->cam->position;
			picked->body->constraint->setPointPivotB(topos);
		}
	}
	else
	{
		if(picked)
		{
			picked->body->setAlwaysActive(false);
			picked->body->constraint->destroy();
			picked = NULL;
		}
	}
#endif

#if (defined(__IOS__) || defined(__ANDROID__)) && !defined(__OUYA__)
	if(sgTouches::touches.size() == 3 && counter < 35)
#elif defined(__OUYA__)
	if(sgGamepad::keys[0] == true)
#else
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) && counter < 500)
#endif
	{
		if(!thrown)
		{
			sgEntity *te = ent->createObjEntity("kiste.sgm", new PhysicsObject);
			te->obj->position = ent->cam->position;
	//		te->obj->initShadowVolume();
	//		te->obj->shadow = true;
			te->createPhysBody(sgPhysBody::ES_BOX, 1.0);
			te->body->setFriction(0.7);

			sgVector3 tdir(0.0f, 0.0f, -15.0f);
			tdir = ent->cam->rotation.rotate(tdir);
			te->body->applyCentralImpulse(tdir);
			thrown = true;
			counter += 1;
		}
	}else
	{
		thrown = false;
	}

	if(shake > 1.2)
	{
		if(picked)
		{
			picked->body->setAlwaysActive(false);
			picked->body->constraint->destroy();
			picked = NULL;
		}

		sgEntity *ne;
		for(sgEntity *te = ent->next; te != NULL; te = ne)
		{
			ne = te->next;
			te->destroy();
		}
		counter = 0;
	}

	sgVector3 dir(toll.x, 0.0f, -toll.y);
	dir = ent->cam->rotation.rotate(dir);
	ent->cam->position += dir*speed*timestep;
	ent->cam->updateCamera();
}
