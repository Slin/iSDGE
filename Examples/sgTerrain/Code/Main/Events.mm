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
#import "sgView.h"

//This method will be called directly after initializing the engine and is meant to be used to initialize your project in
void Events::onInit(sgMain *m)
{
	sgmain = m;
	
	[sgView view].multipleTouchEnabled = true;
	
	//Set device orientation
	sgmain->setOrientation(2);
	
	//Create sun at default position
	sgEntity *light = sgmain->first_ent->createLightEntity();
	light->light->position = sgVector4(-1, -1, 0, 0);
	
	//create the FPS display
	sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);
	
	//Create the camera
	sgEntity *cam = sgmain->first_ent->createCamEntity((sgAction*)new CameraFree);
	cam->cam->position = sgVector3(0.0f, 50.0f, 0.0f);
	cam->cam->rotation = sgVector3(0.0f, 0.0f, -45.0f);
	
	//Create skycube
	sgmain->first_ent->createSkyCubeEntity("sky_right.png", "sky_back.png", "sky_left.png", "sky_front.png", "sky_down.png", "sky_up.png");
	
	//Create a terrain entity
	sgEntity *ent = sgmain->first_ent->createTerrainEntity("czTerrHMP.png", 512, 512, 8, 8, 4, sgVector4(0.1, 0.1, 0.1, 0.0));
	ent->obj->body->materials[0]->setTexture2D(-1, "czTerrTex.png");
	ent->obj->body->materials[0]->setTexture2D(-1, "czTerrDet.png");
	ent->obj->body->materials[0]->setShader("TerrDetCl", "TerrDetCl");
	
	//Create water entity
	ent = sgmain->first_ent->createTerrainEntity(NULL, 2, 2, 2, 2, 0, sgVector4(0.0, 0.0, 0.0, 0.0), (sgAction*)new Water);
	ent->cam = cam->cam;
	ent->obj->position.y = 23.0;
	ent->obj->scale *= 1000.0;
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{
	
}
