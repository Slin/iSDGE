//
//	sgAction.h
//	Engine
//
//	Created by Nils Daumann on 30.04.10.
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

#ifndef __SGACTION_H__
#define __SGACTION_H__

#include "sgEntity.h"

/**
 * Action class. An inherited class from this one can be a part of an entity for comfortable control of it.
 */
class sgAction
{
	public:
	
		/**
		 *	On init.
		 *	This function is called directly after the creation of the entity. It should set the actions pointer to the entity.
		 * @param e pointer to the entity this action is part of.
		 */
		virtual void onInit(sgEntity *e);
	
		/**
		 *	On draw.
		 *	This function is called every frame just before rendering anything. It is called in the order of the entities in their list.
		 * @param timestep the time in seconds since the previous frame was rendered.
		 */
		virtual void onDraw(float timestep);
	
		/**
		 *	On draw late.
		 *	This function is called every frame just before rendering anything, but after all entities onDraw. It is called in the order of the entities in their list.
		 * @param timestep the time in seconds since the previous frame was rendered.
		 */
		virtual void onDrawLate(float timestep);
	
		/**
		 *	On destroy.
		 *	This function is called directly before the entity is destroyed.
		 */
		virtual void onDestroy();
	
		/**
		 *	Entity.
		 *	The pointer to an entity, usually set to the one this action is part of.
		 */
		sgEntity *ent;
};

#endif
