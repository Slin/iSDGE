//
//	sgRenderer.h
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

#ifndef __SGRENDERER_H__
#define __SGRENDERER_H__

#include <vector>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "sgObjectFiles.h"
#include "sgObject.h"
#include "sgPanel.h"
#include "sgLight.h"
#include "sgCamera.h"
#include "sgParticleEmitter.h"

#include "sgTexture.h"
#include "sgShader.h"
#include "sgMaterial.h"
#include "sgVertex.h"
#include "sgMesh.h"
#include "sgObjectBody.h"

#include "sgMatrix4x4.h"
#include "sgColor.h"

#include "sgRenderEvent.h"
#include "sgTimer.h"

struct sgOptimizedMesh
{
	sgMesh *mesh;
	sgMaterial *material;
	sgObject *object;
	
	sgOptimizedMesh *next;
	sgOptimizedMesh *prev;
};

/**
 * Renderer class. This class defines a renderer which manages all objects and draws them.
 */
class sgRenderer
{
	public:
		/**
		 * Constructor.
		 * Initializes a new instance of this class.
		 */
		sgRenderer();
	
		/**
		 *	Deconstructor.
		 *	Frees all memory allocated by this class.
		 */
		~sgRenderer();
	
		/**
		 * Render.
		 * Makes this renderer render.
		 */
		virtual void render();
	
		/**
		 * Resize buffers.
		 * Resizes all render buffers to the size of backingWidth and backingHeight.
		 */
		virtual bool resizeBuffers();
	
		/**
		 * Set device orientation.
		 * Sets the device orientation to display everything for.
		 * @param ori the new orientation 0 = portrait, 1 = flipped portrait, 2 = landscape right, 3 = landscape left
		 */
		void setOrientation(unsigned int ori);
	
		/**
		 * Set multisampling.
		 * Sets the number of samples used for multisampling.
		 * @param samples number of samples to use. 0 deactivates multisampling (default), 4 is maximum on iPhone and iPod Touch. It will use the highest possible if the value is to high.
		 */
		virtual void setMultisampling(unsigned short samples);
	
		/**
		 * Check for extension.
		 * Checks if the device supports the given OpenGL ES extension.
		 * @param name the name of the extension to check for.
		 * @return true if the extension is supported, false otherwize.
		 */
		bool checkForExtension(char *name);
	
		/**
		 * Backing width.
		 * Width of the window the engine is rendering into.
		 */
		static int backingWidth;
		
		/**
		 * Backing height.
		 * Height of the window the engine is rendering into.
		 */
		static int backingHeight;
	
		/**
		 * Scale factor.
		 * Factor used to scale the gui with to make it fit for displays with different dpi counts. This is for example 2.0 on a device with retina display and 1.0 otherwize.
		 */
		static float scaleFactor;
	
		/**
		 * Current time.
		 * The time in seconds, the renderer is already running (automatically passed to shaders defining it as "Time").
		 */
		double currenttime;
	
		/**
		 * Time step.
		 * The time in seconds, passed during the previous frame.
		 */
		double timestep;
	
		/**
		 * Renderer event class.
		 * Class used for renderer events. NULL to just ignore them. It will be freed automatically if set.
		 */
		sgRenderEvent *event;
	
		/**
		 * First sky.
		 * Pointer to a first sky object, which is empty, but meant to be used to create new sky objects from. Sky objects are rendered before anything else.
		 */
		sgObject *first_sky;
	
		/**
		 * First solid.
		 * Pointer to a first solid object, which is empty, but meant to be used to create new solid objects from.
		 */
		sgObject *first_solid;
	
		/**
		 * First camera.
		 * Pointer to a first camera, which is empty, but meant to be used to create new cameras from.
		 */
		sgCamera *first_cam;
	
		/**
		 * First light.
		 * Pointer to a first light, which is empty, but meant to be used to create new lights from.
		 */
		sgLight *first_light;
	
		/**
		 * First panel.
		 * Pointer to a first panel, which is empty, but meant to be used to create new panels from.
		 */
		sgPanel *first_panel;
	
		/**
		 * First particle emitter.
		 * Pointer to the first particle emitter, which is empty, but meant to be used to create new particle emitters from.
		 */
		sgParticleEmitter *first_partemitter;
	
		/**
		 * Clear color.
		 * Defines the color to clear the framebuffer with.
		 */
		sgFColorA clearcolor;
	
		/**
		 * OpenGL ES Version.
		 * 0 if the renderer couldn´t be initialized, 1 for OpenGL ES 1.1 and 2 for 2.0.
		 */
		static unsigned int oglversion;
	
		/**
		 * Maximum number of bones per mesh.
		 * Default value is 30.
		 */
		static unsigned int maxbones;
	
		/**
		 * Device orientation.
		 * The current device orientation.
		 */
		unsigned int orientation;
	
		/**
		* Render optimized.
		* Culls and sorts the geometry before rendering to speed it up, if this is TRUE.
		*/
		bool optimize;
	
		/**
		* Renderbuffers.
		* The renderbuffers needed for rendering.
		*/
		unsigned int mainFramebuffer;
		unsigned int colorRenderbuffer;
		unsigned int depthRenderbuffer;
		unsigned int stencilRenderbuffer;
		unsigned int msaaFramebuffer;
		unsigned int msaaRenderbuffer;
		unsigned int currfbo;
	
	protected:
		void updateOrientation();
	
		void chooseObjLOD(sgObject *obj, float dist);
		void chooseMeshLOD(sgObject *obj, unsigned int mesh, float dist);
	
		void culling(sgCamera *cam, sgObject *first);
		void sorting();
		void optimizing();
	
		float quadposuv[20];
		unsigned int quadvbo;
		sgMaterial *lastmat;
	
		sgOptimizedMesh *first_optimized;
	
		std::vector<sgMesh*> meshes;
		std::vector<sgMaterial*> materials;
	
		sgMatrix4x4 matglobal2d;
		sgMatrix4x4 matglobal3d;
	
		unsigned short msaasamples;
	
		bool supportmultisampling;
		bool supportdiscard;
		bool supportpackedstencil;
	
		sgTimer timer;
};

#endif
