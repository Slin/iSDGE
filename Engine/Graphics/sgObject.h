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

//class sgMesh;
class sgTexture;
class sgMaterial;
class sgLight;
class sgMesh;
class sgShadowVolume;

/**
 * Object container class. Used to store objects in and load objects from.
 */
class sgObjectContainer : public sgBase
{
	public:
		std::vector<sgMesh*>meshs;
		std::vector<sgMaterial*>materials;
};

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
		 *	Creates a new object loaded from a model file or a predefined name and makes it next of this.
		 * @param name name of the object or the file to load it from.
		 * @return pointer to the new object.
		 */
		sgObject *createObject(const char *name);
	
		/**
		 *	Add plane.
		 *	Creates a new plane mesh with the given settings and adds it to the object. The distance of vertices belong one axis is 1 unit by default.
		 * @param xverts vertex count along the x axis.
		 * @param zverts vertex count along the z axis.
		 * @param posoffset offset of the position relative to the objects center.
		 * @param mat material to use for the plane or NULL to create a new one.
		 * @param xchunk the chunks x position (0 to xchunks). FOR INTERNAL USE!
		 * @param zchunk the chunks z position (0 to zchunks). FOR INTERNAL USE!
		 * @param uvfac factor applied to the texcoords. FOR INTERNAL USE!
		 * @param hmp pointer to a locked heightmap texture to extract the height from. FOR INTERNAL USE!
		 * @param hmppartsize the size in pixels of the texture part used for the chunk. FOR INTERNAL USE!
		 * @param hmpscale the scale factors for the heightmaps color channels. FOR INTERNAL USE!
		 */
		void addPlane(unsigned int xverts = 2, unsigned int zverts = 2, sgVector3 posoffset = sgVector3(0, 0, 0), sgMaterial *mat = NULL, unsigned char xchunk = 0, unsigned char zchunk = 0, sgVector2 uvfac = sgVector2(1, 1), sgTexture *hmp = NULL, sgVector2 hmppartsize = sgVector2(1, 1), sgVector4 hmpscale = sgVector4(0, 0, 0, 0));
	
		/**
		 *	Create terrain.
		 *	Creates a new terrain object with the given settings. The distance of vertices belong one axis is 1 unit by default. When using chunks, the number of vertices differs due to the need of dublicated ones.
		 * @param xverts vertex count along the x axis. POT recommended.
		 * @param zverts vertex count along the z axis. POT recommended.
		 * @param xchunks number of chunks along the x axis. xverts/xchunks should be a clean division.
		 * @param zchunks number of chunks along the z axis. zverts/zchunks should be a clean division.
		 * @param hmp filename of heightmap texture to extract the height from.
		 * @param hmpscale the scale factors for the heightmaps color channels.
		 * @return pointer to the new object
		 */
		sgObject *createTerrain(unsigned int xverts = 2, unsigned int zverts = 2, unsigned char xchunks = 1, unsigned char zchunks = 1, const char *hmp = NULL, sgVector4 hmpscale = sgVector4(0, 0, 0, 0));
	
		/**
		 *	Clone material.
		 *	Makes the given material a clone from the previous one, to allow independant materials for objects of the same type.
		 * @param mat index of the material to clone.
		 */
		void cloneMaterial(unsigned int mat);
	
		/**
		 *	Set animation.
		 *	Sets the meshes given animation frame.
		 * @param name name of the animation.
		 * @param percent position within the animation in percent.
		 */
		void setAnimation(const char *name, float percent);
	
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
		 *	Shadow volume mesh.
		 *	The mesh of the volume used for stencil shadows. Generated automatically.
		 */
		sgShadowVolume *shadowvolume;
	
		/**
		 *	Materials.
		 *	A vector of the materials. There have to be as many materials as meshes, where each mesh is rendered with the material at the same position as the mesh in the mesh array.
		 */
		std::vector<sgMaterial*> materials;
	
		/**
		 *	Meshs.
		 *	A vector of the meshs. There have to be as many meshes as materials, where each mesh is rendered with the material at the same position in the material array.
		 */
		std::vector<sgMesh*> meshs;
	
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
