//
//	sgEntity.cpp
//	Engine
//
//	Created by Nils Daumann on 30.04.10.
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

#include "sgEntity.h"

#include "sgMain.h"
#include "sgAction.h"
#include "sgDebug.h"

sgEntity::sgEntity(sgEntity* p, sgEntity *n, sgMain *m)
{
	prev = p;
	next = n;
	if(next)
	{
		next->prev = this;
	}
	
	sgmain = m;
	
	obj = NULL;
	cam = NULL;
	light = NULL;
	act = NULL;
	pan = NULL;
	emitt = NULL;
}

sgEntity *sgEntity::createEmptyEntity(sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createEmptyObjEntity(sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createEmptyObj();
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createObjEntity(const char *name, unsigned long flags, sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createObj(name, flags);
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createTerrainEntity(const char *hmp, unsigned int xverts, unsigned int zverts, unsigned char xchunks, unsigned char zchunks, unsigned int lodsteps, sgVector4 hmpscale, sgAction *a)
{
	if(xverts == 0 || zverts == 0)
		return NULL;
	
	next = new sgEntity(this, next, sgmain);
	next->createTerrain(hmp, xverts, zverts, xchunks, zchunks, lodsteps, hmpscale);
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createSkyCubeEntity(const char *right, const char *back, const char *left, const char *front, const char *down, const char *up, sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createSkyCube(right, back, left, front, down, up);
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createCamEntity(sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createCam();
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createLightEntity(sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createLight();
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createPanEntity(sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createPan();
	next->createAction(a);
	return next;
}

sgEntity *sgEntity::createEmitterEntity(const char *texfile, sgAction *a)
{
	next = new sgEntity(this, next, sgmain);
	next->createEmitter(texfile);
	next->createAction(a);
	return next;
}

void sgEntity::createEmptyObj()
{
	if(obj != NULL)
		return;
	
	obj = sgmain->renderer->first_solid->createObject();
}

void sgEntity::createObj(const char *name, unsigned long flags)
{
	if(obj != NULL)
		return;
	
	obj = sgmain->renderer->first_solid->createObject(name, flags);
}

void sgEntity::createTerrain(const char *hmp, unsigned int xverts, unsigned int zverts, unsigned char xchunks, unsigned char zchunks, unsigned int lodsteps, sgVector4 hmpscale)
{
	if(obj != NULL)
		return;
	
	if(xverts == 0 || zverts == 0)
		return;
	
	obj = sgmain->renderer->first_solid->createTerrain(xverts, zverts, xchunks, zchunks, lodsteps, hmp, hmpscale);
}

void sgEntity::createSkyCube(const char *right, const char *back, const char *left, const char *front, const char *down, const char *up)
{
	if(obj != NULL)
		return;
	
	obj = sgmain->renderer->first_sky->createObject("skycube");
	
	obj->body->materials[0]->setTexture2D(-1, right, false);
	obj->body->materials[0]->depthtest = false;
	obj->body->materials[0]->textures[0]->setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	obj->body->materials[0]->textures[0]->setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	obj->body->materials[1]->setTexture2D(-1, back, false);
	obj->body->materials[1]->depthtest = false;
	obj->body->materials[1]->textures[0]->setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	obj->body->materials[1]->textures[0]->setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	obj->body->materials[2]->setTexture2D(-1, left, false);
	obj->body->materials[2]->textures[0]->setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	obj->body->materials[2]->textures[0]->setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	obj->body->materials[2]->depthtest = false;
	obj->body->materials[3]->setTexture2D(-1, front, false);
	obj->body->materials[3]->textures[0]->setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	obj->body->materials[3]->textures[0]->setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	obj->body->materials[3]->depthtest = false;
	obj->body->materials[4]->setTexture2D(-1, down, false);
	obj->body->materials[4]->textures[0]->setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	obj->body->materials[4]->textures[0]->setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	obj->body->materials[4]->depthtest = false;
	obj->body->materials[5]->setTexture2D(-1, up, false);
	obj->body->materials[5]->textures[0]->setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	obj->body->materials[5]->textures[0]->setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	obj->body->materials[5]->depthtest = false;
	obj->sky = true;
}

void sgEntity::createCam()
{
	if(cam != NULL)
		return;
	
	cam = sgmain->renderer->first_cam->createCamera();
	
	cam->size = sgVector2(sgRenderer::backingWidth, sgRenderer::backingHeight);
	cam->aspect = cam->size.x/cam->size.y;
	cam->updateProj();
}

void sgEntity::createLight()
{
	if(light != NULL)
		return;
	
	light = sgmain->renderer->first_light->createLight();
}

void sgEntity::createPan()
{
	if(pan != NULL)
		return;
	
	pan = sgmain->renderer->first_panel->createPanel();
}

void sgEntity::createEmitter(const char *texfile)
{
	if(emitt != NULL)
		return;
	
	emitt = sgmain->renderer->first_partemitter->createEmitter(texfile);
}

void sgEntity::createAction(sgAction *a)
{
	if(act != NULL)
		return;
	
	act = a;
	if(act != NULL)
		act->onInit(this);
}

void sgEntity::destroy()
{
	if(act != NULL)
		act->onDestroy();
	
	if(obj != NULL)
		obj->destroy();
	
	if(cam != NULL)
		cam->destroy();
	
	if(light != NULL)
		light->destroy();
	
	if(pan != NULL)
		pan->destroy();
	
	if(emitt != NULL)
		emitt->destroy();
	
	act = NULL;
	obj = NULL;
	cam = NULL;
	light = NULL;
	pan = NULL;
	emitt = NULL;
	
	if(prev)
		prev->next = next;
	if(next)
		next->prev = prev;
	
	delete this;
}

void sgEntity::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
}