//
//	sgParticle.h
//	iSDGE
//
//	Created by Nils Daumann on 13.02.11.
//	Copyright (c) 2011 Nils Daumann

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

#ifndef __SGPARTICLE_H__
#define __SGPARTICLE_H__

#include <vector>
#include <string>

#include "sgVector4.h"
#include "sgVector3.h"


/**
 * Particle class. Represents a particle.
 */
class sgParticle
{
	public:
		/**
		 *	Constructor.
		 *	Creates a particle.
		 */
		sgParticle();
	
		/**
		 *	Deconstructor.
		 *	Frees the particle.
		 */
		virtual ~sgParticle();
	
		/**
		 *	On draw.
		 *	Will be called each frame and is meant to be used to controll the particles behaviour.
		 *	@param timestep time in seconds of the previous frame.
		 */
		virtual void onDraw(float timestep);
	
		/**
		 *	Position.
		 *	The position of this particle.
		 */
		sgVector3 position;
	
		/**
		 *	Scale.
		 *	The scale of this particle.
		 */
		float scale;
	
		/**
		 *	Color
		 *	The particles color including transparency.
		 */
		sgVector4 color;
	
		/**
		 *	Texcoords
		 *	The particles texcoords to use. Can be used to have texture atlasses for particles as well as animated particles.
		 */
		sgVector4 texcoords;
		
		/**
		 *	Destroy
		 *	If this is set to true, the particle will be destroyed within the same frame.
		 */
		bool destroy;
	
		/**
		 * Camdist
		 * Distance to the camera, needed for sorting. This variable is automatically updated, if the emitter sorts its particles.
		 **/
		float camdist;
};

#endif
