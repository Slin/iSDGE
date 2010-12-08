//
//	sgObject.h
//	iSDGE
//
//	Created by Nils Daumann on 17.04.10.
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

#ifndef __SGOBJECT_H__
#define __SGOBJECT_H__

#include <vector>
#include <string>

#include "sgBase.h"
#include "sgVector4.h"
#include "sgVector3.h"
#include "sgVector2.h"
#include "sgMatrix4x4.h"
#include "sgQuaternion.h"
#include "sgObjectBody.h"

//class sgMesh;
class sgTexture;
class sgMaterial;
class sgLight;
class sgMesh;
class sgShadowVolume;


/**
 * Object class. Responsible for object handling.
 */
class sgObject
{
	public:
		/**
		 *	Constructor.
		 *	Creates an empty object.
		 * @param p the previous object in the linked object list.
		 * @param n the next object in the linked object list.
		 */
		sgObject(sgObject *p, sgObject *n);
	
		/**
		 *	Deconstructor.
		 *	Frees the object.
		 */
		~sgObject();
	
		/**
		 *	Create object.
		 *	Creates a new empty object and makes it next of this.
		 * @return pointer to the new object.
		 */
		sgObject *createObject();
		
		/**
		 *	Create object.
		 *	Creates a new object loaded from a model file or a predefined name and makes it next of this.
		 * @param name name of the object or the file to load it from.
		 * @return pointer to the new object.
		 */
		sgObject *createObject(const char *name);
		
		/**
		 *	Create terrain.
		 *	Creates a new terrain object with the given settings. The distance of vertices belong one axis is 1 unit by default. When using chunks, the number of vertices differs due to the need of dublicated ones.
		 * @param xverts vertex count along the x axis. POT recommended.
		 * @param zverts vertex count along the z axis. POT recommended.
		 * @param xchunks number of chunks along the x axis. xverts/xchunks should be a clean division.
		 * @param zchunks number of chunks along the z axis. zverts/zchunks should be a clean division.
		 * @param lodsteps number of lod steps to create for this terrain.
		 * @param hmp filename of heightmap texture to extract the height from.
		 * @param hmpscale the scale factors for the heightmaps color channels.
		 * @return pointer to the new object
		 */
		sgObject *createTerrain(unsigned int xverts = 2, unsigned int zverts = 2, unsigned char xchunks = 1, unsigned char zchunks = 1, unsigned int lodsteps = 3, const char *hmp = NULL, sgVector4 hmpscale = sgVector4(0, 0, 0, 0));
	
		/**
		 * Init shadow volume.
		 * Initializes the shadow volume. Has to be called once, before shadows will be displayed. This function is slow and best called at object creation time.
		 **/
		void initShadowVolume();
	
		/**
		 *	Calculate cull sphere.
		 *	Calculates the cull sphere for the object and its meshs.
		 */
		void calcCullSphere();
	
		/**
		 *	Destroy.
		 *	Destroys this object.
		 */
		void destroy();
	
		/**
		 *	Destroy All.
		 *	Destroys this and all objects connected to it.
		 */
		void destroyAll();
	
		/**
		 *	Update model.
		 *	Updates this objects model transformation matrix. This function is called automatically before rendering.
		 */
		void updateModel();
	
		/**
		 *	Position.
		 *	The position of this object.
		 */
		sgVector3 position;
	
		/**
		 *	Rotation.
		 *	The rotation of this object as quaternion.
		 */
		sgQuaternion rotation;
	
		/**
		 *	Scale.
		 *	The scale of this object.
		 */
		sgVector3 scale;
	
		/**
		 *	Model matrix.
		 *	The transformation matrix of this object.
		 */
		sgMatrix4x4 matmodel;
	
		/**
		 *	Normal matrix.
		 *	The normal transformation matrix of this object.
		 */
		sgMatrix4x4 matnormal;
	
		/**
		 *	Activate shadow.
		 *	Makes this object throwing a stencil shadow.
		 */
		bool shadow;
	
		/**
		 * Tag.
		 * Allows to ignore this object for rendering or whereever one can specify it.
		 **/
		unsigned int tag;
	
		/**
		 * Per mesh lod
		 * LOD is applied per mesh.
		 **/
		bool permeshlod;
	
		/**
		 *	Culled.
		 *	True, if the object wasn´t rendered the previous frame.
		 */
		bool culled;
	
		/**
		 *	Cull sphere.
		 *	Sphere checked against the view frustum for culling. XYZ is the local center and W the radius.
		 */
		sgVector4 cullsphere;
	
		/**
		 *	LOD max dist.
		 *	Maximum distance to show this object at.
		 */
		float loddist;
		
		/**
		 *	LOD object.
		 *	Object switched to at distances higher than loddist.
		 */
		sgObjectBody *body;
	
		/**
		 *	LOD object.
		 *	Object switched to at distances higher than loddist.
		 */
		sgObjectBody *currbody;
	
		/**
		 *	Shadow volume mesh.
		 *	The mesh of the volume used for stencil shadows. Generated automatically.
		 */
		sgShadowVolume *shadowvolume;
	
		/**
		 *	Sky.
		 *	Handle as sky. This causes the object to be rendered without camera translation.
		 */
		bool sky;
	
		/**
		 *	Next.
		 *	Pointer to the next object within the object list or NULL if there is none.
		 */
		sgObject *next;
	
		/**
		 *	Previous.
		 *	Pointer to the previous object within the object list or NULL if there is none.
		 */
		sgObject *prev;
};

#endif
