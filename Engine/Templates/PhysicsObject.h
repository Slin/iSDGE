/*
 *  PhysicsObject.h
 *  iSDGE
 *
 *  Created by Nils Daumann on 30.06.12.
 *  Copyright 2012 SlinDev. All rights reserved.
 *
 */

#ifndef __PhysicsObject_H__
#define __PhysicsObject_H__

#include "sgAction.h"

class PhysicsObject : public sgAction
{
public:
	void onInit(sgEntity *e);
	void onDrawLate(float timestep);
};

#endif
