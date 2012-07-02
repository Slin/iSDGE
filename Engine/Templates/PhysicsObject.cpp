/*
 *  PhysicsObject.cpp
 *  iSDGE
 *
 *  Created by Nils Daumann on 30.06.12.
 *  Copyright 2012 SlinDev. All rights reserved.
 *
 */

#include "PhysicsObject.h"

void PhysicsObject::onInit(sgEntity *e)
{
	ent = e;
}

//This method is called every frame, just before drawing for each object with this action attached
void PhysicsObject::onDrawLate(float timestep)
{
	ent->obj->updateObject();
}
