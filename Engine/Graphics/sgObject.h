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
#include "sgVector3.h"
#include "sgMatrix4x4.h"
#include "sgQuaternion.h"

//class sgMesh;
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
		 * @param p the previous object in the linked object list
		 * @param n the next object in the linked object list
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
		 * @param name name of the object or the file to load it from
		 * @return pointer to the new object
		 */
		sgObject *createObject(const char *name);
	
		/**
		 *	Create plane
		 *	Creates a new plane object with the given number of vertices in x and y direction. The distance of vertices belong one axis is 1 unit by default.
		 * @param name name of the object or the file to load it from
		 * @return pointer to the new object
		 */
		sgObject *createPlane(unsigned int xverts, unsigned int zverts, float xtexscale, float ytexscale);
	
		/**
		 *	Clone material.
		 *	Makes the given material a clone from the previous one, to allow independant materials for objects of the same type.
		 * @param mat index of the material to clone
		 */
		void cloneMaterial(unsigned int mat);
	
		/**
		 *	Set animation.
		 *	Sets the meshes given animation frame.
		 * @param name name of the animation
		 * @param percent position within the animation in percent
		 */
		void setAnimation(const char *name, float percent);
	
		/**
		 * Init shadow volume.
		 * Initializes the shadow volume. Has to be called once, before shadows will be displayed. This function is slow and best called at object creation time.
		 **/
		void initShadowVolume();
	
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
