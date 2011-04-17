//
//	sgCharacterController.h
//	Engine
//
//	Created by Nils Daumann on 09.04.11.
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

#ifndef __SGCHARACTERCONTROLLER_H__
#define __SGCHARACTERCONTROLLER_H__

#include "sgVector3.h"
#include "sgQuaternion.h"

/**
 *	Physics body. Represents a body within a physics world handled by a physics engine.
 */
class sgCharacterController
{
	public:
		/**
		 *	Body shapes. The supported shapes.
		 */
		enum eShape
		{
			ES_BOX,
			ES_SPHERE,
			ES_MESH
		};
	
		/**
		 *	Constructor.
		 *	Makes the basic setup for this body to completely initialize it from, later on.
		 *	@param shp the shape to use (ES_MESH is always static)
		 *	@param pos the position vector to effect
		 *	@param rot the rotation quaternion to effect
		 *	@param sz the size of the shape (ignored for ES_MESH)
		 */
		sgCharacterController(eShape shp, sgVector3 &pos, sgQuaternion &rot, sgVector3 &sz);
	
		/**
		 *	Destructor.
		 *	Cleans everything up.
		 */
		~sgCharacterController();
	
		/**
		 *	Add face.
		 *	If using ES_MESH as shape, this function has to be used to setup the meshs faces to use. Has to be called before calling init.
		 *	@param vert1 the position of the faces first vertex
		 *	@param vert2 the position of the faces second vertex
		 *	@param vert3 the position of the faces third vertex
		 */
		virtual void addFace(sgVector3 vert1, sgVector3 vert2, sgVector3 vert3){}
	
		/**
		 *	Init.
		 *	Finishes the initialization of a body and adds it to the physics world.
		 *	@param mass the bodys mass
		 *	@param userdata data attached to the body, used internally to get the entity hit or collided with and such
		 */
		virtual void init(float mass, void *userdata){}
	
		/**
		 *	Destroy.
		 *	Destroys the body.
		 */
		virtual void destroy(){}
	
		/**
		 *	Static.
		 *	Returns weather the body is static or not.
		 */
		virtual bool isStatic(){return false;}
		
		/**
		 *	Apply central impulse.
		 *	Applies the given impulse (a force over one frame) to the bodys center.
		 *	@param impulse the impulse to apply
		 */
		virtual void applyCentralImpulse(sgVector3 impulse){}
	
		/**
		 *	Set friction.
		 *	Sets a value for the friction.
		 *	@param friction the value between 0 and 1, where 0 means no friction at all
		 */
		virtual void setFriction(float friction){}
	
		/**
		 *	Set always active.
		 *	Activates or deactivates the body and keeps it active if set to be active.
		 *	@param active sets the body active if true, deactivates it otherwize
		 */
		virtual void setAlwaysActive(bool active = true){}
	
	
		virtual void setKinematic(bool enable = true){}
	
		/**
		 *	Get body.
		 *	Returns a pointer to the used physics engines internal body.
		 *	@return pointer to the physics engines internal body
		 */
		virtual void *getBody(){return 0;}
	
		/**
		 *	Position.
		 *	Reference to the bodys position (usually the graphic objects position).
		 */
		sgVector3 &position;
	
		/**
		 *	Rotation.
		 *	Reference to the bodys rotation (usually the graphic objects rotation).
		 */
		sgQuaternion &rotation;
		
	protected:
		bool initialized;
};

#endif
