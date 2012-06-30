//
//	sgMain.h
//	Engine
//
//	Created by Nils Daumann on 07.06.10.
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

#ifndef __SGMAIN_H__
#define __SGMAIN_H__

#include "sgRenderer.h"
#include "sgRendererES1.h"
#include "sgRendererES2.h"
#include "sgTouches.h"
#include "sgAccelerometer.h"
#include "sgMouse.h"
#include "sgCameraStream.h"
#include "sgEvents.h"
#include "sgAction.h"
#include "sgEntity.h"
#include "sgTimer.h"
#include "sgPhysWorld.h"
#include "sgPhysWorldBullet.h"
#include "sgAudioPlayer.h"

class sgMain
{
	public:
		/**
		 *	Constructor.
		 *	Initializes an instance of this class.
		 * @param oglvers the OpenGL ES version to initialize the engine for (1 for 1.1 or 2 for 2.0)
		 */
		sgMain(unsigned int oglvers);

		/**
		 *	Deconstructor.
		 *	Frees an instance of this class.
		 */
		~sgMain();

		/**
		 *	Draw view.
		 *	Called automatically every frame and draws everything. Just don´t touch it.
		 */
		void drawView();

		/**
		 *	Set orientation.
		 *	Call this to set a new interface orientation.
		 * @param interfaceOrientation the new orientation 0 = portrait, 1 = flipped portrait, 2 = landscape right, 3 = landscape left
		 */
		void setOrientation(unsigned int interfaceOrientation);

		/**
		 *	Renderer.
		 *	Pointer to the renderer.
		 */
		sgRenderer *renderer;

		/**
		 *	Physics world.
		 *	Pointer to the physics world.
		 */
		sgPhysWorld *physworld;

		/**
		 *	Audio player.
		 *	Pointer to the audio player.
		 */
		sgAudioPlayer *audioplayer;

		/**
		 *	First entity.
		 *	Create all your entities from this one.
		 */
		sgEntity *first_ent;

		/**
		 *	Event handler.
		 *	Set this to a custom sgEvent class to handle events.
		 */
		static sgEvents *eventhandler;

		/**
		 *	Device orientation.
		 *	The currently active device orientation.
		 */
		unsigned int orientation;

		/**
		 *	Time step.
		 *	The time the previous frame took in seconds.
		 */
		double timestep;

		/**
		 *	Current frames.
		 *	The number of frames rendered from the start of the engine.
		 */
		unsigned long currframes;

	private:
		sgTimer timer;
};

#endif
