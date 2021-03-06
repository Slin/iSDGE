/*
 *  Water.cpp
 *  sgWavefield
 *
 *  Created by Nils Daumann on 29.12.10.
 *  Copyright 2010 SlinDev. All rights reserved.
 *
 */

#include "Water.h"
#include "sgObjectBody.h"
#include "sgTexture.h"
#include "sgMain.h"
#include "math.h"

Water::Water(sgMain *m, const char *bf, sgCamera *cam)
{
	bumpfile = bf;
	camera = cam;

	m->first_ent->createTerrainEntity(NULL, 2, 2, 1, 1, 0, sgVector4(), this);
}

void Water::onInit(sgEntity *e)
{
	ent = e;
	ent->obj->scale *= 1200.0;

	//Create the reflection camera and create and assign a rendertarget
	mirror = ent->createCamEntity();
	mirror->cam->size = sgVector2(128, 128);
	mirror->cam->rendertarget = sgTexture::getTexture(128, 128);
	mirror->cam->rendertarget->makeRendertarget();
	mirror->cam->tag = 1;
	mirror->cam->lodshift = 150.0;

	ent->obj->body->materials[0]->setTexture(-1, bumpfile);
	ent->obj->body->materials[0]->setTexture(-1, mirror->cam->rendertarget);
#if defined __IOS__
	ent->obj->body->materials[0]->setShader("iSDGE.bundle/Water.vsh", "iSDGE.bundle/Water.fsh");
#else
	ent->obj->body->materials[0]->setShader("Water.vsh", "Water.fsh");
#endif
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(0.1, 0.1, 1));

	ent->obj->body->materials[0]->blending = true;
	ent->obj->body->materials[0]->depthwrite = false;
	ent->obj->body->materials[0]->depthtest = true;
	ent->obj->body->materials[0]->culling = false;

	ent->obj->tag = 1;
}

void Water::onDraw(float timestep)
{
	ent->obj->body->materials[0]->mattex.mat[12] += 0.1*timestep;
}

//This method is called every frame, just before drawing for each object with this action attached
void Water::onDrawLate(float timestep)
{
	ent->obj->position.x = camera->position.x;
	ent->obj->position.z = camera->position.z;

	mirror->cam->position = camera->position;
	mirror->cam->position.y = 2.0*ent->obj->position.y-mirror->cam->position.y;
	sgVector3 rot = camera->rotation;
	rot.y *= -1;
	rot.z *= -1;
	mirror->cam->rotation = rot;

	ent->obj->updateObject();
	mirror->cam->updateCamera();
}
