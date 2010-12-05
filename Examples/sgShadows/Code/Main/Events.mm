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

void Events::onInit(sgMain *m)
{
	sgmain = m;
	
	//Set device orientation
	sgmain->setOrientation(2);
	
//	sgmain->renderer->setMultisampling(2);
	
	//Create a light source
	sgmain->renderer->first_light->createLight();
	sgmain->renderer->first_light->next->attenuation = 0.0001;
	
	//create the "HUD"
	sgEntity *hud = sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);
	hud->pan->addImage("logo_64.png", sgVector2(400, 0), sgVector2(64, 64));
	
	//Create a camera
	sgEntity *cam = sgmain->first_ent->createCamEntity((sgAction*)new CameraFree);
	cam->cam->position = sgVector3(0.0f, 5.0f, 5.0f);
	cam->cam->rotation = sgVector3(0.0f, 0.0f, -45.0f);
	
	//Create a skycube
	sgmain->first_ent->createSkyCubeEntity("sky_right.png", "sky_back.png", "sky_left.png", "sky_front.png", "sky_down.png", "sky_up.png");
	
	//Create objects
	sgEntity *ent;
	ent = sgmain->first_ent->createObjEntity("haus.sgm");
	ent->obj->initShadowVolume();	//Initialize shadow volume (can be done at any time, but there will be no shadow before)
	ent->obj->shadow = true;		//Activates the shadow (has no effect, if the volume wasn´t initialized)
	
	ent = sgmain->first_ent->createObjEntity("f360.sgm", (sgAction*)new Car);
	ent->obj->initShadowVolume();
	ent->obj->shadow = true;
	
	ent = sgmain->first_ent->createObjEntity("jeep2.sgm", (sgAction*)new Jeep);
	ent->obj->position = sgVector3(-8, 0, 0);
	ent->obj->initShadowVolume();
	ent->obj->shadow = true;
}

void Events::onDraw(float timestep)
{
	
}
