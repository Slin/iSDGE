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

	//Create the ground
/*	sgEntity *ent = sgmain->first_ent->createObjEntity("box");
	ent->obj->scale.x *= 10;
	ent->obj->scale.y *= 0.1;
	ent->obj->scale.z *= 10;
	ent->obj->position.y = -0.1;
	ent->obj->body->materials[0]->setTexture(-1, "sand.png");
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(10, 10, 10));*/

	sgEntity *ent = sgmain->first_ent->createObjEntity("sponza.sgm");
	ent->obj->scale = 0.1f;
	ent->obj->rotation = sgVector3(0.0f, 0.0f, -90.0f);
	ent->obj->updateObject();
	ent->obj->body->materials[4]->alphatest = true;
	ent->obj->body->materials[4]->culling = false;
	ent->obj->body->materials[4]->alphatestvalue = 0.9f;
	ent->obj->body->materials[4]->setShader(sgShader::BIS_TEXTURE_DISCARD);
	ent->obj->body->materials[10]->alphatest = true;
	ent->obj->body->materials[10]->culling = false;
	ent->obj->body->materials[10]->alphatestvalue = 0.9f;
	ent->obj->body->materials[10]->setShader(sgShader::BIS_TEXTURE_DISCARD);
	ent->obj->body->materials[19]->alphatest = true;
	ent->obj->body->materials[19]->culling = false;
	ent->obj->body->materials[19]->alphatestvalue = 0.9f;
	ent->obj->body->materials[19]->setShader(sgShader::BIS_TEXTURE_DISCARD);
/*	ent->obj->body->materials[0]->setShader(sgShader::BIS_LIGHTMAP);
	ent->obj->body->materials[1]->setShader(sgShader::BIS_LIGHTMAP);*/

//	ent = sgmain->first_ent->createObjEntity("test.sgm");
//	ent->obj->body->materials[0]->setShader(sgShader::BIS_LIGHTMAP);

	//Create a moving car
	ent = sgmain->first_ent->createObjEntity("mini_knight.sgm", (sgAction*)new AnimTest);
	ent->obj->position = sgVector3(0.0, 10.0, 0.0);
	ent->obj->updateObject();
	ent->obj->body->materials[0]->setShader("anim.vsh", "anim.fsh");
	ent->obj->skeleton->init();
	ent->obj->skeleton->update(0.0f);
	skeleton = ent->obj->skeleton;
	
	
	skeleton->animations.insert(std::pair<std::string, sgAnimation*>(std::string("hard"), new sgAnimation(std::string("hard"))));
	
	sgAnimationBone *animbone = new sgAnimationBone(0, 0, 0.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector3(0.0f, 0.0f, 0.0f)));
	animbone->nextframe = new sgAnimationBone(animbone, 0, 1.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector4(1.0f, 0.0f, 0.0f, 45.0f)));
	animbone->nextframe->nextframe = new sgAnimationBone(animbone->nextframe, animbone, 2.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector3(0.0f, 0.0f, 0.0f)));
	animbone->nextframe->prevframe = animbone->nextframe->nextframe;
	animbone->prevframe = animbone->nextframe->nextframe;
	skeleton->animations[std::string("hard")]->bones.insert(std::pair<int, sgAnimationBone*>(0, animbone));
	
	animbone = new sgAnimationBone(0, 0, 0.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector3(0.0f, 0.0f, 0.0f)));
	animbone->nextframe = new sgAnimationBone(animbone, 0, 1.0f, sgVector3(0.0f, 1.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector4(1.0f, 0.0f, 0.0f, 45.0f)));
	animbone->nextframe->nextframe = new sgAnimationBone(animbone->nextframe, animbone, 2.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector3(0.0f, 0.0f, 0.0f)));
	animbone->nextframe->prevframe = animbone->nextframe->nextframe;
	animbone->prevframe = animbone->nextframe->nextframe;
	skeleton->animations[std::string("hard")]->bones.insert(std::pair<int, sgAnimationBone*>(1, animbone));
	
	animbone = new sgAnimationBone(0, 0, 0.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector3(0.0f, 0.0f, 0.0f)));
	animbone->nextframe = new sgAnimationBone(animbone, 0, 1.0f, sgVector3(0.0f, 1.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector4(1.0f, 0.0f, 0.0f, 45.0f)));
	animbone->nextframe->nextframe = new sgAnimationBone(animbone->nextframe, animbone, 2.0f, sgVector3(0.0f, 0.0f, 0.0f), sgVector3(1.0f, 1.0f, 1.0f), sgQuaternion(sgVector3(0.0f, 0.0f, 0.0f)));
	animbone->nextframe->prevframe = animbone->nextframe->nextframe;
	animbone->prevframe = animbone->nextframe->nextframe;
	skeleton->animations[std::string("hard")]->bones.insert(std::pair<int, sgAnimationBone*>(2, animbone));
	
	
	skeleton->setAnimation(std::string("test"));
	
	
	sgAnimationBone *bone = skeleton->animations[std::string("test")]->bones[2];
	sgAnimationBone *temp = bone;
	while(temp != bone->prevframe)
	{
		printf("%f, %f, %f\n", temp->position.x, temp->position.y, temp->position.z);
		temp = temp->nextframe;
	}
//	temp->position.makeIdentity();
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{
/*	static float rot1 = 0.0f;
	static float rot2 = 0.0f;
	rot1 += timestep*20.0f;
	rot2 = sin(rot1*0.1f)*30.0f;
	skeleton->bones[0].rotation = sgVector3(rot1, 0.0, 0.0);
	skeleton->bones[1].rotation = sgVector3(0.0, rot2, 0.0);*/
	skeleton->update(timestep*24.0f);
}
