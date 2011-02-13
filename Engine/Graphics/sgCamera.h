//
//	sgCamera.h
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

#ifndef __SGCAMERA_H__
#define __SGCAMERA_H__

#include "sgMatrix4x4.h"
#include "sgVector4.h"
#include "sgVector3.h"
#include "sgVector2.h"
#include "sgQuaternion.h"
#include "sgPlane.h"

class sgTexture;

/**
 * Camera class. Responsible for camera handling.
 */
class sgCamera
{
	public:
		/**
		 *	Constructor.
		 *	Creates a camera.
		 * @param p the previous camera in the linked camera list
		 * @param n the next camera in the linked camera list
		 */
		sgCamera(sgCamera *p, sgCamera *n);
		
		/**
		 *	Create camera.
		 *	Creates a new camera and makes it next of this.
		 * @return pointer to the new camera
		 */
		sgCamera *createCamera();
	
		/**
		 *	Project proj to world.
		 *	Transforms the vector with the cameras projection and view matrices to worldspace.
		 * @param dir the vector to transform.
		 * @return the transformed vector.
		 */
		sgVector3 camToWorld(sgVector3 dir);
	
		/**
		 *	Destroy.
		 *	Destroys this camera.
		 */
		void destroy();
		
		/**
		 *	Destroy All.
		 *	Destroys this and all cameras connected to it.
		 */
		void destroyAll();
		
		/**
		 *	Update projection.
		 *	Updates this cameras projection matrix. This function has to be called after making changes to arc, aspect or the far and near clipping planes.
		 */
		void updateProj();
	
		/**
		 *	Update view.
		 *	Updates this cameras view transformation matrix. This function is called automatically before rendering.
		 */
		void updateView();
	
		/**
		 *	Update frustum.
		 *	Updates this cameras view frustum planes. This function is called automatically before view frustum culling.
		 */
		void updateFrustum();
	
		/**
		 *	Check visibility.
		 *	Checks if the given sphere is within the view frustum.
		 */
		bool inFrustum(sgVector3 center, float radius);
	
		/**
		 *	Rendertarget.
		 *	The target texture, this view has to render into. Assign a prepared texture here or NULL to deactivate rendering into a texture.
		 */
		sgTexture *rendertarget;
	
		/**
		 *	Arc.
		 *	The cameras field of view.
		 */
		float arc;
	
		/**
		 *	Aspect ratio.
		 *	The cameras aspect ratio.
		 */
		float aspect;
	
		/**
		 *	Near clipping plane dist.
		 *	The distance of the near clipping plane of this camera.
		 */
		float clipnear;
	
		/**
		 *	Far clipping plane dist.
		 *	The distance of the far clipping plane of this camera.
		 */
		float clipfar;
	
		/**
		 * Ignore tag.
		 * Doesn´t render objects with the same tag.
		 **/
		unsigned int tag;
	
		/**
		 * LOD shift.
		 * This is the distance LOD steps are shifted with for this view.
		 **/
		float lodshift;
	
		/**
		 *	Position.
		 *	The position of the camera.
		 */
		sgVector3 position;
	
		/**
		 *	Rotation.
		 *	The rotation of the camera as quaternion.
		 */
		sgQuaternion rotation;
	
		/**
		 *	Camera window size.
		 *	The size of the window the camera is rendered into.
		 */
		sgVector2 size;
	
		/**
		 *	Camera window position.
		 *	The position the cameras window is rendered at.
		 */
		sgVector2 screenpos;
		
		/**
		 *	Projection matrix.
		 *	The transformation matrix for this cameras projection.
		 */
		sgMatrix4x4 matproj;
	
		/**
		 *	Inverse projection matrix.
		 *	The inverse transformation matrix for this cameras projection.
		 */
		sgMatrix4x4 matinvproj;
		
		/**
		 *	View matrix.
		 *	The transformation matrix of this camera.
		 */
		sgMatrix4x4 matview;
	
		/**
		 *	Inverse view projection matrix.
		 *	The inverse transformation matrix of this camera.
		 */
		sgMatrix4x4 matinvview;
		
		/**
		 *	View frustum.
		 *	The view frustum planes.
		 */
		sgPlane plleft;
		sgPlane plright;
		sgPlane pltop;
		sgPlane pldown;
	
		/**
		 *	View frustum sphere center.
		 *	The view frustums spheres center.
		 */
		sgVector3 frustumcenter;
	
		/**
		 *	View frustum sphere radius.
		 *	The view frustums spheres radius.
		 */
		float frustumradius;
	
		/**
		 *	Previous.
		 *	Pointer to the previous camera within the cameras list or NULL if there is none.
		 */
		sgCamera *prev;
		
		/**
		 *	Next.
		 *	Pointer to the next camera within the cameras list or NULL if there is none.
		 */
		sgCamera *next;
};

#endif
