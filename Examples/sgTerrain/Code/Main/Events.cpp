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
#include "RenderEvent.h"
#include "BlubPart.h"

#include "sgResourceManager.h"

#if defined __IOS__
	#import "sgView.h"
#endif

//This message will be called before creating the renderer and has to be used to set the paths to the standard shaders
void Events::onPreInit(sgMain *m)
{
#if !defined __IOS__
	//path to the default shaders
	sgResourceManager::addPath("../../../../Engine/Graphics/");

	sgResourceManager::addPath("../../../../Media/Textures/");
	sgResourceManager::addPath("../../../../Media/Shaders/");
	sgResourceManager::addPath("../../../../Engine/Templates/Shaders/");
#endif
}

//This method will be called directly after initializing the engine and is meant to be used to initialize your project in
void Events::onInit(sgMain *m)
{
	sgmain = m;

#if defined __IOS__
	[sgView view].multipleTouchEnabled = true;

	//Set device orientation
	sgmain->setOrientation(2);
#endif

	//Create sun at default position
	sgEntity *light = sgmain->first_ent->createLightEntity();
	light->light->position = sgVector4(-1, -1, 0, 0);

	//create the FPS display
	sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);

	//Create the camera
	cam = sgmain->first_ent->createCamEntity((sgAction*)new CameraFree);
	cam->cam->position = sgVector3(0.0f, 50.0f, 0.0f);
	cam->cam->rotation = sgVector3(0.0f, 0.0f, -45.0f);
	cam->createEmitter("blub.png");
	cam->emitt->material->blendsource = GL_SRC_ALPHA;
	cam->emitt->material->blenddestination = GL_ONE;

	//Create skycube
	sgmain->first_ent->createSkyCubeEntity("sky_right.png", "sky_back.png", "sky_left.png", "sky_front.png", "sky_down.png", "sky_up.png");

	//Create water entity
	Water *water = new Water(m, "waterbump.png", cam->cam);
	water->ent->obj->position.y = 23.0;

	//Create a terrain entity
	sgEntity *terr = sgmain->first_ent->createTerrainEntity("czTerrHMP.png", 512, 512, 8, 8, 4, sgVector4(0.1, 0.1, 0.1, 0.0));
	terr->obj->body->materials[0]->setTexture(-1, "czTerrTex.png");
	terr->obj->body->materials[0]->setTexture(-1, "czTerrDet.png");

	//Set render event
	sgmain->renderer->event = (sgRenderEvent*)new RenderEvent(terr, "TerrDetail.vsh", "TerrDetail.fsh", "TerrCl.vsh", "TerrCl.fsh");
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{
	static float currtime = 0;
	static float lastparttime = 0;
	currtime += timestep;
	if((currtime-lastparttime) > (0.1) && cam->cam != NULL)
	{
		cam->emitt->emitPart((sgParticle*)new BlubPart(cam->cam));
		lastparttime = currtime;
	}
}
