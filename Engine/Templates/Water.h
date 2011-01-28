/*
 *  Water.h
 *  sgWavefield
 *
 *  Created by Nils Daumann on 29.12.10.
 *  Copyright 2010 SlinDev. All rights reserved.
 *
 */

#ifndef __Water_H__
#define __Water_H__

#include "sgAction.h"

class Water : public sgAction
{
public:
	Water(sgMain *m, const char *bf, sgCamera *cam);
	void onInit(sgEntity *e);
	void onDraw(float timestep);
	void onDrawLate(float timestep);
	
	sgEntity *mirror;
	sgCamera *camera;
	const char *bumpfile;
};

#endif