//
//	sgLight.h
//	iSDGE
//
//	Created by Nils Daumann on 19.04.10.
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

#ifndef __SGLIGHT_H__
#define __SGLIGHT_H__

#include "sgColor.h"
#include "sgVector4.h"

/**
 * Light class. Responsible for light handling.
 */
class sgLight
{
	public:
		/**
		 *	Constructor.
		 *	Creates a light.
		 * @param p the previous light in the linked light list
		 * @param n the next light in the linked light list
		 */
		sgLight(sgLight *p, sgLight *n);
	
		/**
		 *	Create light.
		 *	Creates a new light and makes it next of this.
		 * @return pointer to the new light
		 */
		sgLight *createLight();
	
		/**
		 *	Set to default.
		 *	Sets all light parameters to the default ones (position is not effected).
		 */
		void setDefault();
	
		/**
		 *	Destroy.
		 *	Destroys this light.
		 */
		void destroy();
	
		/**
		 *	Destroy All.
		 *	Destroys this and all lights connected to it.
		 */
		void destroyAll();
	
		/**
		 *	Shadow.
		 *	If true, objects with shadow flag set, will cast a shadow when they are lit by this light.
		 */
		bool shadow;
		
		/**
		 *	Position.
		 *	The position of the light. Set the w component to 0, to make this a sun with an endless distance, where xyz define the lights direction.
		 */
		sgVector4 position;
	
		/**
		 *	Ambient.
		 *	Color used for the ambient lighting.
		 */
		sgFColorA ambient;
	
		/**
		 *	Diffuse.
		 *	Color used for the diffuse lighting.
		 */
		sgFColorA diffuse;
	
		/**
		 *	Specular.
		 *	Color used for the specular lighting.
		 */
		sgFColorA specular;
	
		/**
		 *	Attenuation.
		 *	Factor used for the quadratic attenuation, if this is a point light source. The smaller, the higher is the range.
		 */
		float attenuation;
		
		/**
		 *	Previous.
		 *	Pointer to the previous light within the lights list or NULL if there is none.
		 */
		sgLight *prev;
	
		/**
		 *	Next.
		 *	Pointer to the next light within the lights list or NULL if there is none.
		 */
		sgLight *next;
};

#endif
