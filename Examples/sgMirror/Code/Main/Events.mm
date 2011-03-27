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

//Has to be included, if you want to enable multitouch
#import "sgView.h"

//This method will be called directly after initializing the engine and is meant to be used to initialize your project in
void Events::onInit(sgMain *m)
{
	sgmain = m;
	
	[sgView view].multipleTouchEnabled = YES;
	
	//Set device orientation
	sgmain->setOrientation(2);
	
	//Create sun at default position
	sgmain->renderer->first_light->createLight();
	
	//create the FPS display
	sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);
	
	//Create the camera
	camera = sgmain->first_ent->createCamEntity((sgAction*)new CameraFree);
	camera->cam->position = sgVector3(0.0f, 5.0f, 5.0f);
	camera->cam->rotation = sgVector3(0.0f, 0.0f, -45.0f);
	
	//Create the reflection camera and create and assign a rendertarget
	mirror = sgmain->first_ent->createCamEntity();
	mirror->cam->size = sgVector2(128, 128);
	mirror->cam->rendertarget = sgTexture::getTexture2D(128, 128);
	mirror->cam->rendertarget->makeRendertarget();
	mirror->cam->tag = 1;
	
	//Create skycube
	sgmain->first_ent->createSkyCubeEntity("sky_right.png", "sky_back.png", "sky_left.png", "sky_front.png", "sky_down.png", "sky_up.png");
	
	//Create a rotating space ship
	sgEntity *ent = sgmain->first_ent->createObjEntity("f360.sgm", (sgAction*)new Rotate);
	
/*	ent->obj->initShadowVolume();
	ent->obj->shadow = true;*/
	
	//Create reflecting ground and assign texture and render target to it as well as the reflection shader
	ent = sgmain->first_ent->createObjEntity("box");
	ent->obj->scale.x *= 10;
	ent->obj->scale.y *= 0.1;
	ent->obj->scale.z *= 10;
	ent->obj->position.y = -0.1;
	ent->obj->body->materials[0]->setTexture2D(-1, "marble.png");
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(10, 10, 10));
	ent->obj->body->materials[0]->setTexture2D(-1, mirror->cam->rendertarget);
	ent->obj->body->materials[0]->setShader("Reflect.vsh", "Reflect.fsh");
	ent->obj->tag = 1;
}

//Called every frame, just before drawing
void Events::onDrawLate(float timestep)
{
	mirror->cam->position = camera->cam->position;
	mirror->cam->position.y = -mirror->cam->position.y;
	sgVector3 rot = camera->cam->rotation;
	rot.y *= -1;
	rot.z *= -1;
	mirror->cam->rotation = rot;
}
