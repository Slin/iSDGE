//
//	sgEntity.h
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

#ifndef __SGENTITY_H__
#define __SGENTITY_H__

#include "sgMaterial.h"
#include "sgObject.h"
#include "sgCamera.h"
#include "sgLight.h"
#include "sgPanel.h"
#include "sgVector4.h"

class sgAction;
class sgMain;

/**
 * Entity class. Container class for the different engine objects for easy creation and handling.
 */
class sgEntity
{
	public:
		/**
		 *	Constructor.
		 *	Creates an empty enity.
		 * @param p the previous entity in the linked object list
		 * @param n the next entity in the linked object list
		 */
		sgEntity(sgEntity *p, sgEntity *n, sgMain *v);
		
		/**
		 *	Create object entity.
		 *	Creates a new entity with an object loaded from a model file or a predefined name and makes it next of this.
		 * @param name name of the entity or the file to load it from.
		 * @param a pointer to the action which will be assigned to the new entity.
		 * @return pointer to the new entity
		 */
		sgEntity *createObjEntity(const char *name, sgAction *a = NULL);
	
		/**
		 *	Create terrain entity.
		 *	Creates a new terrain entity from a heightmap image with the given number of vertices in x and z direction splitted into several meshes (chunks). The distance of vertices along an axis is 1 unit by default.
		 * @param hmp name of the heightmap image, or NULL for a flat terrain.
		 * @param xverts number of vertices along the x axis. POT recommended.
		 * @param zverts number of vertices along the z axis. POT recommended.
		 * @param xchunks number of chunks along the x axis. xverts/xchunks should be a clean division.
		 * @param zchunks number of chunks along the z axis. zverts/zchunks should be a clean division.
		 * @param hmpscale vector defining how much each heightmap color channel effects the terrains height.
		 * @param a pointer to the action which will be assigned to the new entity.
		 * @return pointer to the new entity
		 */
		sgEntity *createTerrainEntity(const char *hmp, unsigned int xverts = 10, unsigned int zverts = 10, unsigned char xchunks = 1, unsigned char zchunks = 1, sgVector4 hmpscale = sgVector4(0.01, 0, 0, 0), sgAction *a = NULL);
	
		/**
		 *	Create skycube entity.
		 *	Creates a new entity with a skycube as object, textured with the given textures.
		 * @param right filename of the texture of the right face
		 * @param back filename of the texture of the right face
		 * @param left filename of the texture of the right face
		 * @param front filename of the texture of the right face
		 * @param down filename of the texture of the right face
		 * @param up filename of the texture of the right face
		 * @param a pointer to the action which will be assigned to the new entity.
		 * @return pointer to the new entity
		 */
		sgEntity *createSkyCubeEntity(const char *right, const char *back, const char *left, const char *front, const char *down, const char *up, sgAction *a = NULL);
	
		/**
		 *	Create camera entity.
		 *	Creates a new entity consisting of just a camera and makes it next of this.
		 * @param a pointer to the action which will be assigned to the new entity.
		 * @return pointer to the new entity
		 */
		sgEntity *createCamEntity(sgAction *a = NULL);
	
		/**
		 *	Create light entity.
		 *	Creates a new entity consisting of just a light and makes it next of this.
		 * @param a pointer to the action which will be assigned to the new entity.
		 * @return pointer to the new entity
		 */
		sgEntity *createLightEntity(sgAction *a = NULL);
	
		/**
		 *	Create panel entity.
		 *	Creates a new entity consisting of a panel and makes it next of this.
		 * @param a pointer to the action which will be assigned to the new entity.
		 * @return pointer to the new entity
		 */
		sgEntity *createPanEntity(sgAction *a = NULL);
		
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
		 *	Object.
		 *	Pointer to the object handled by this entity or NULL if there is none.
		 */
		sgObject *obj;
	
		/**
		 *	Camera.
		 *	Pointer to the camera handled by this entity or NULL if there is none.
		 */
		sgCamera *cam;
	
		/**
		 *	Light.
		 *	Pointer to the light handled by this entity or NULL if there is none.
		 */
		sgLight *light;
	
		/**
		 *	Panel.
		 *	Pointer to the panel handled by this entity or NULL if there is none.
		 */
		sgPanel *pan;
	
		/**
		 *	Action.
		 *	Pointer to the action handled by this entity or NULL if there is none.
		 */
		sgAction *act;
		
		/**
		 *	Next.
		 *	Pointer to the next entity within the object list or NULL if there is none.
		 */
		sgEntity *next;
		
		/**
		 *	Previous.
		 *	Pointer to the previous entity within the object list or NULL if there is none.
		 */
		sgEntity *prev;
		
		/**
		 *	sgMain.
		 *	The engines root layer, setting up and controlling everything.
		 */
		sgMain *sgmain;
};

#endif
