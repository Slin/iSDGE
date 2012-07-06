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

#include "procTexture.h"
#include "procBuilding.h"

//Has to be included, if you want to enable multitouch
#import "sgView.h"

//This method will be called directly after initializing the engine and is meant to be used to initialize your project in
void Events::onInit(sgMain *m)
{
	sgmain = m;
    
	//Activate multitouch support
	[sgView view].multipleTouchEnabled = true;
	
	//Set device orientation
	sgmain->setOrientation(2);
	
	//Activate multisampling
	sgmain->renderer->setMultisampling(4);
	
	//Create sun at default position
	sgmain->renderer->first_light->createLight();
	
	//create the FPS display
	sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);
	
	//Create the camera
	sgEntity *cam = sgmain->first_ent->createCamEntity((sgAction*)new CameraFree);
	cam->cam->position = sgVector3(0.0f, 10.0f, 0.0f);
	cam->cam->rotation = sgVector3(0.0f, 0.0f, -90.0f);
	
	//Create skycube
	sgmain->first_ent->createSkyCubeEntity("sky_right.png", "sky_back.png", "sky_left.png", "sky_front.png", "sky_down.png", "sky_up.png");
	
	//Generate ground texture
	sgTexture *ground = sgTexture::getTexture(128, 128);
	ground->lockPixels();
	ground->setColor(sgColorA(120.0, 130.0, 140.0, 255.0));
	procTexture::genNoise(ground, sgColorA(255, 255, 255, 0), procTexture::BM_ADD, 0.2);
	ground->unlockPixels();
	ground->setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	ground->setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	ground->generateMipmaps();
	
	//Create the ground
	sgEntity *ent = sgmain->first_ent->createObjEntity("box");
	ent->obj->scale.x *= 50;
	ent->obj->scale.y *= 0.1;
	ent->obj->scale.z *= 50;
	ent->obj->position.y = -0.1;
	ent->obj->body->materials[0]->setTexture(-1, ground);
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(50, 50, 50));
	ent->obj->updateObject();
	
	//Generate brick texture
	sgTexture *bricks = sgTexture::getTexture(128, 128);
	bricks->lockPixels();
	bricks->setColor(sgColorA(140.0, 31.0, 16.0, 255.0));
	procTexture::genBricks(bricks, sgColorA(64, 70, 80, 0), procTexture::BM_MULTIPLY, 1.0, 64, 32, 2, 2);
	procTexture::genNoise(bricks, sgColorA(255, 255, 255, 0), procTexture::BM_ADD, 0.05);
	bricks->unlockPixels();
	bricks->setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	bricks->setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	bricks->generateMipmaps();
	
	//Create a building
	ent = sgmain->first_ent->createEmptyObjEntity();
	ent->obj->position.y += 10.0;
	procBuilding::genBuilding(ent->obj, sgVector3(25.0, 10.0, 5.0), 1, 1, 1, procBuilding::ET_RECTANGLE);
	ent->obj->calcCullSphere();
	ent->obj->body->cloneMaterial(0);
	ent->obj->body->materials[0]->setTexture(0, bricks);
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(100, 40, 20));
	ent->obj->updateObject();
	
	//Generate window texture
	sgTexture *windows = sgTexture::getTexture(128, 128);
	windows->lockPixels();
	windows->setColor(sgColorA(255.0, 255.0, 255.0, 255.0));
	procTexture::genBricks(windows, sgColorA(50, 50, 50, 255.0), procTexture::BM_MULTIPLY, 1.0, 32, 64, 2, 2);
	windows->unlockPixels();
	windows->setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	windows->setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
	windows->generateMipmaps();
	
	//Create another building
	ent = sgmain->first_ent->createEmptyObjEntity();
	ent->obj->position.y += 15.0;
	ent->obj->position.z += 15.0;
	procBuilding::genBuilding(ent->obj, sgVector3(5.0, 15.0, 5.0), 1, 1, 1, procBuilding::ET_RECTANGLE);
	ent->obj->calcCullSphere();
	ent->obj->body->cloneMaterial(0);
	ent->obj->body->materials[0]->setTexture(0, windows);
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(8, 20, 8));
	ent->obj->updateObject();
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{
	
}
