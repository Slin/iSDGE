//
//	Events.cpp
//	Engine
//
//	Created by Nils Daumann on 01.05.10.
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

#include "Events.h"
#include "sgResourceManager.h"

#if defined __IOS__
	//Has to be included, if you want to enable multitouch
	#import "sgView.h"
#endif

#include "sgDebug.h"
#include "CameraFree.h"
#include "FPSDisplay.h"

#include "AnimTest.h"

//This message will be called before creating the renderer and has to be used to set the paths to the standard shaders
void Events::onPreInit(sgMain *m)
{
#if !defined __IOS__
	//path to the default shaders
	sgResourceManager::addPath("../../../../Engine/Graphics/");

	sgResourceManager::addPath("../../../../Media/Models/lasse/scifiroom/");
	sgResourceManager::addPath("../../../../Media/Textures/");
	sgResourceManager::addPath("../../../../Engine/Templates/Shaders/");
#endif
}

//This method will be called directly after initializing the engine and is meant to be used to initialize your project in
void Events::onInit(sgMain *m)
{
	sgmain = m;

#if defined __IOS__
	//Activate multitouch support
	[sgView view].multipleTouchEnabled = true;

	//Set device orientation
	sgmain->setOrientation(2);
#endif

	//Activate multisampling
	sgmain->renderer->setMultisampling(4);

	//Create sun at default position
	sgmain->renderer->first_light->createLight();

	//create the FPS display
	sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);

	//Create the camera
	sgEntity *cam = sgmain->first_ent->createCamEntity((sgAction*)new CameraFree);
	cam->cam->position = sgVector3(0.0f, 5.0f, 5.0f);
	cam->cam->rotation = sgVector3(0.0f, 0.0f, -45.0f);

	//Create skycube
	sgmain->first_ent->createSkyCubeEntity("sky_right.png", "sky_back.png", "sky_left.png", "sky_front.png", "sky_down.png", "sky_up.png");

	sgEntity *ent = sgmain->first_ent->createObjEntity("sponza.sgm");
	ent->obj->scale = 0.1f;
	ent->obj->rotation = sgVector3(0.0f, 0.0f, -90.0f);
	ent->obj->updateObject();
	ent->obj->body->materials[5]->alphatest = true;
	ent->obj->body->materials[5]->culling = false;
	ent->obj->body->materials[5]->alphatestvalue = 0.9f;
	ent->obj->body->materials[5]->setShader(sgShader::BIS_TEXTURE_DISCARD);
	ent->obj->body->materials[6]->alphatest = true;
	ent->obj->body->materials[6]->culling = false;
	ent->obj->body->materials[6]->alphatestvalue = 0.9f;
	ent->obj->body->materials[6]->setShader(sgShader::BIS_TEXTURE_DISCARD);
	ent->obj->body->materials[17]->alphatest = true;
	ent->obj->body->materials[17]->culling = false;
	ent->obj->body->materials[17]->alphatestvalue = 0.9f;
	ent->obj->body->materials[17]->setShader(sgShader::BIS_TEXTURE_DISCARD);

	ent = sgmain->first_ent->createObjEntity("simplegirl.sgm");
	ent->obj->position = sgVector3(5.0, -1.7, 0.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent->obj->body->materials[0]->setShader("anim.vsh", "anim.fsh");
	ent->obj->skeleton->init();
	ent->obj->skeleton = ent->obj->skeleton;
	ent->obj->skeleton->setAnimation(std::string("cammina"));
	skeleton = ent->obj->skeleton;
	
	//Create a zombie
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(0.0, -1.7, 0.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(2.0, -1.7, 0.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(-2.0, -1.7, 0.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(4.0, -1.7, 0.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(-4.0, -1.7, 0.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
/*	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(0.0, -1.7, 2.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(0.0, -1.7, -2.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(0.0, -1.7, 4.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(0.0, -1.7, -4.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(2.0, -1.7, 2.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();
	
	ent = sgmain->first_ent->createObjEntity("new_thin_zombie.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(2.0, -1.7, -2.0);
	ent->obj->scale = 1.0f;
	ent->obj->updateObject();*/
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{
	skeleton->update(timestep*24.0f);
}
