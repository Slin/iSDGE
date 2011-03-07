//
//	sgParticleEmitter.h
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

#ifndef __SGPARTICLEEMITTER_H__
#define __SGPARTICLEEMITTER_H__

#include <vector>
#include <string>

class sgParticle;
class sgMaterial;
class sgCamera;

/**
 * Particle compare class. Needed to sort particles.
 */
class sgParticleCompare
{
	public:
		/**
		 *	Particle compare.
		 *	Needed to sort particles.
		 **/
		bool operator()(sgParticle *p1, sgParticle *p2);
};

/**
 * Particle emitter class. Responsible for creating particle meshs and updating particles.
 */
class sgParticleEmitter
{
	public:
		/**
		 *	Constructor.
		 *	Creates an empty object.
		 * @param p the previous object in the linked object list.
		 * @param n the next object in the linked object list.
		 */
		sgParticleEmitter(sgParticleEmitter *p, sgParticleEmitter *n);
	
		/**
		 *	Deconstructor.
		 *	Frees the object.
		 */
		~sgParticleEmitter();
	
		/**
		 *	Create emitter.
		 *	Creates a new empty object and makes it next of this.
		 *	@param texfile name of the texture file to use.
		 * @return pointer to the new object.
		 */
		sgParticleEmitter *createEmitter(const char* texfile);
	
		/**
		 *	Emit particle.
		 *	Registers the particles of type p to be rendered by this emitter.
		 *	@param p the particle to emit, will be deleted automatically!
		 */
		void emitPart(sgParticle *p);
		
		/**
		 *	Destroy.
		 *	Destroys this object.
		 */
		void destroy();
	
		/**
		 *	Destroy All.
		 *	Destroys this and all emitters connected to it.
		 */
		void destroyAll();
	
		/**
		 * Update mesh.
		 * Called before rendering. Updates the particle mesh.
		 * @param cam pointer to the camera the particles are meant to face
		 * @param timestep the time in seconds passed during the previous frame
		 **/
		void updateMesh(sgCamera *cam, float timestep);
		
		/**
		 * Sorted
		 * If set, the particles will be sorted from front to back before rendering.
		 **/
		bool sorted;
	
		/**
		 * Tag.
		 * Allows to ignore this object for rendering or whereever one can specify it.
		 **/
		unsigned int tag;
	
		/**
		 * Material.
		 * Material shared by all particles emitted by this emitter.
		 **/
		sgMaterial *material;
	
		/**
		 * Vertices.
		 * The particle meshs vertices.
		 **/
		float *vertices;
	
		/**
		 * Indices.
		 * The particle meshs indices.
		 **/
		unsigned short *indices;
	
		/**
		 * Index number.
		 * The particle meshs number of indices.
		 **/
		unsigned int indexnum;
	
		/**
		 *	Next.
		 *	Pointer to the next emitter within the emitter list or NULL if there is none.
		 */
		sgParticleEmitter *next;
	
		/**
		 *	Previous.
		 *	Pointer to the previous emitter within the emitter list or NULL if there is none.
		 **/
		sgParticleEmitter *prev;
	
	private:
	
		/**
		 *	Particles.
		 *	List of the particles handled by this emitter.
		 */
		std::vector<sgParticle*> particles;
};

#endif
