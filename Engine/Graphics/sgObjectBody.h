//
//	sgObjectBody.h
//	iSDGE
//
//	Created by Nils Daumann on 24.11.10.
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

#ifndef __SGOBJECTBODY_H__
#define __SGOBJECTBODY_H__

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
class sgMesh;

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
class sgObjectBody
{
	public:
		/**
		 *	Constructor.
		 *	Creates an empty object.
		 */
		sgObjectBody();
	
		/**
		 *	Deconstructor.
		 *	Frees the object.
		 */
		~sgObjectBody();
	
		/**
		 *	Make object.
		 *	Creates a new object body loaded from a model file or a predefined name.
		 * @param name name of the object or the file to load it from.
		 * @return pointer to the new object body.
		 */
		sgObjectBody *makeObject(const char *name);
		
		/**
		 *	Add terrain plane.
		 *	Creates a new terrain plane mesh with the given settings and adds it to the object. The distance of vertices belong one axis is 1 unit by default.
		 * @param xverts vertex count along the x axis.
		 * @param zverts vertex count along the z axis.
		 * @param posoffset offset of the position relative to the objects center.
		 * @param mat material to use for the plane or NULL to create a new one.
		 * @param scale the distance between two vertices in x and y direction
		 * @param xchunk the chunks x position (0 to xchunks). FOR INTERNAL USE!
		 * @param zchunk the chunks z position (0 to zchunks). FOR INTERNAL USE!
		 * @param uvfac factor applied to the texcoords. FOR INTERNAL USE!
		 * @param hmp pointer to a locked heightmap texture to extract the height from. FOR INTERNAL USE!
		 * @param hmppartsize the size in pixels of the texture part used for the chunk. FOR INTERNAL USE!
		 * @param hmpscale the scale factors for the heightmaps color channels. FOR INTERNAL USE!
		 * @param addborder adds a border around the plane which is needed to hide holes between LOD steps. FOR INTERNAL USE!
		 */
		void addTerrainPlane(unsigned int xverts, unsigned int zverts, sgVector3 posoffset, sgMaterial *mat, sgVector2 scale, unsigned char xchunk, unsigned char zchunk, sgVector2 uvfac, sgTexture *hmp, sgVector2 hmppartsize, sgVector4 hmpscale, bool addborder);
	
		/**
		 *	Calculate cull sphere.
		 *	Calculates the cull spheres for the meshs.
		 */
		void calcCullSphere();
	
		/**
		 *	Clone material.
		 *	Makes the given material a clone from the previous one, to allow independant materials for objects of the same type.
		 * @param mat index of the material to clone.
		 */
		void cloneMaterial(unsigned int mat);
	
		/**
		 *	Destroy.
		 *	Destroys this object body.
		 */
		void destroy();
	
		/**
		 *	Destroy All.
		 *	Destroys this and all object bodies connected to it.
		 */
		void destroyAll();
	
		/**
		 *	LOD max dist.
		 *	Maximum distance to show this object at.
		 */
		float loddist;
		
		/**
		 *	LOD object.
		 *	Object switched to at distances higher than loddist.
		 */
		sgObjectBody *nextbody;
	
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
};

#endif
