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

#include "CameraFreePhys.h"
#include "FPSDisplay.h"
#include "AutoDoor.h"

//This method will be called directly after initializing the engine and is meant to be used to initialize your project in
void Events::onInit(sgMain *m)
{
	sgmain = m;
	
	//Activate multitouch support
	[sgView view].multipleTouchEnabled = true;
	
	//Set device orientation
	sgmain->setOrientation(2);
	
	//Activate multisampling
//	if(![[[UIDevice currentDevice] model] rangeOfString:@"iPad"].location != NSNotFound)
		sgmain->renderer->setMultisampling(2);
	
	sgEntity *te = sgmain->first_ent->createObjEntity("kiste.sgm");
	te->obj->body->materials[0]->setTexture(0, "kiste_diffuse.png");
	te->obj->body->materials[0]->setTexture(1, "kiste_normal.png");
	te->obj->body->materials[0]->setShader("NormDiffSpecS.vsh", "NormDiffSpecS.fsh");
	te->createPhysBody(sgPhysBody::ES_BOX, 1.0);
	
	//Create the camera
	cam = sgmain->first_ent->createCamEntity((sgAction*)new CameraFreePhys);
	
	//Create sun at default position
//	sgmain->renderer->first_light->createLight();
	
	sgEntity *ent = sgmain->first_ent->createObjEntity("space_room.sgm");
	ent->obj->body->materials[1]->setTexture(2, "hauptmodul_normal.png");
	ent->obj->body->materials[0]->setTexture(2, "wandmodul_normal.png");
	ent->obj->body->materials[0]->setShader("NormDiffSpecSLm.vsh", "NormDiffSpecSLm.fsh");
	ent->obj->body->materials[1]->setShader("NormDiffSpecSLm.vsh", "NormDiffSpecSLm.fsh");
	ent->createPhysBody(sgPhysBody::ES_MESH, 0.0);
	ent->body->setFriction(0.7);
	
	ent = sgmain->first_ent->createObjEntity("door1.sgm", new AutoDoor);
	ent->obj->body->materials[0]->setTexture(2, "wandmodul_normal.png");
	ent->obj->body->materials[0]->setShader("NormDiffSpecSLm.vsh", "NormDiffSpecSLm.fsh");
	ent->obj->position = sgVector3(19.1, 1.0, -0.1);
	ent->createPhysBody(sgPhysBody::ES_BOX, 0, sgVector3(0.5, 3.0, 10.0));
	ent->body->setKinematic();
	ent->body->setAlwaysActive();
	
	ent = sgmain->first_ent->createObjEntity("door2.sgm");
	ent->obj->body->materials[0]->setTexture(1, "wandmodul_normal.png");
	ent->obj->body->materials[0]->setShader("NormDiffSpecS.vsh", "NormDiffSpecS.fsh");
	ent->obj->position = sgVector3(35.0, -0.275, 18.5);
	ent->createPhysBody(sgPhysBody::ES_BOX, 1, sgVector3(1.0, 2.0, 0.2));
	sgPhysConstraint *constr = sgmain->physworld->getConstraint(sgPhysConstraint::ET_HINGE);
	constr->initHinge(ent->body, sgVector3(1.0, 0.0, 0.0), sgVector3(0.0, 1.0, 0.0));
	constr->setHingeLimit(270.0, 360.0);
	
	//create the FPS display
	sgmain->first_ent->createPanEntity((sgAction*)new FPSDisplay);
	
	[[sgView viewcontroller] startAccelerometer];
}

//Called every frame, just before drawing
void Events::onDraw(float timestep)
{
	
}
