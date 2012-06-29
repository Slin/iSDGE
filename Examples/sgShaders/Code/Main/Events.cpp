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

//This message will be called before creating the renderer and has to be used to set the paths to the standard shaders
void Events::onPreInit(sgMain *m)
{
#if !defined __IOS__
	//path to the default shaders
	sgResourceManager::addPath("../../../../Engine/Graphics/");

	sgResourceManager::addPath("../../../../Media/Models/modelcheap/");
	sgResourceManager::addPath("../../../../Media/Textures/");
	sgResourceManager::addPath("../../../../Media/Logos/");
	sgResourceManager::addPath("../../../../Media/Shaders/");
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

	//Create a rotating space ship
	sgEntity *ent = sgmain->first_ent->createObjEntity("spacecraft.sgm", (sgAction*)new Rotate);
	ent->obj->scale *= 4;
	ent->obj->updateObject();

	//Create the ground from box primitive
	ent = sgmain->first_ent->createObjEntity("box", (sgAction*)new ShaderBox);
	ent->obj->scale = sgVector3(10.0f, 1.0f, 10.0f);
	ent->obj->position.y = -5.0f;
	ent->obj->updateObject();

	//Clone its material (which is shared between primitives) and assign a texture and set its tiling
	ent->obj->body->cloneMaterial(0);
	ent->obj->body->materials[0]->setTexture(-1, "sand.png");
	ent->obj->body->materials[0]->mattex.makeScale(sgVector3(10.0f, 10.0f, 10.0f));

	//Create a rendertarget texture for the camera
	cam->cam->size.x = sgRenderer::backingWidth;
	cam->cam->size.y = sgRenderer::backingHeight;
	cam->cam->rendertarget = sgTexture::getTexture(sgRenderer::backingWidth, sgRenderer::backingHeight);
	cam->cam->rendertarget->makeRendertarget();

	//Create a horizontal blur effect
#if defined __IOS__
	sgMaterial *mat = sgMaterial::getMaterial("iSDGE.bundle/sgsPPBase.vsh", "PPBlurX.fsh");
#else
	sgMaterial *mat = sgMaterial::getMaterial("Shaders/sgsPPBase.vsh", "PPBlurX.fsh");
#endif
	mat->setTexture(-1, cam->cam->rendertarget);

	//Render it as a panel into the same texture
	sgEntity *pan = ent->sgmain->first_ent->createPanEntity();
	pan->pan->fixorientation = true;
	pan->pan->addImage(mat, sgVector2(0, 0), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor, sgRenderer::backingHeight/sgRenderer::scaleFactor));
//	pan->pan->rendertarget = cam->cam->rendertarget;
	pan->pan->rendertarget = sgTexture::getTexture(sgRenderer::backingWidth, sgRenderer::backingHeight);
	pan->pan->rendertarget->makeRendertarget();

	//Create a vertical blur effect
#if defined __IOS__
	mat = sgMaterial::getMaterial("iSDGE.bundle/sgsPPBase.vsh", "PPBlurY.fsh");
#else
	mat = sgMaterial::getMaterial("Shaders/sgsPPBase.vsh", "PPBlurY.fsh");
#endif
	mat->setTexture(-1, pan->pan->rendertarget);

	//Render it as a panel to the screen
	pan = ent->sgmain->first_ent->createPanEntity();
	pan->pan->fixorientation = true;
	pan->pan->addImage(mat, sgVector2(0, 0), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor, sgRenderer::backingHeight/sgRenderer::scaleFactor));
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{

}
