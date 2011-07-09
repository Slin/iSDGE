//
//	sgPhysWorld.h
//	Engine
//
//	Created by Nils Daumann on 30.03.11.
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

#ifndef __SGAUDIOPLAYER_H__
#define __SGAUDIOPLAYER_H__

#include "sgSound.h"
#include "sgSoundSource.h"
#include <vector>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

/**
 *	Audio player. Manages 2D and 3D sounds as well as music playback.
 */
class sgAudioPlayer
{
	public:
		/**
		 *	Constructor.
		 *	Initializes the audio player world.
		 */
		sgAudioPlayer();
	
		/**
		 *	Destructor.
		 *	Cleans everything up.
		 */
		~sgAudioPlayer();
	
		/**
		 *	Register sound.
		 *	Registers the given sound and returns a handle.
		 * @param snd pointer to the sound to register
		 * @return handle handle to the registered sound
		 */
		unsigned int registerSound(sgSound *snd);
	
		/**
		 *	Unregister sound.
		 *	Unregisters the sound with the given handle.
		 * @param handle handle to the sound to unregister
		 */
		void unregisterSound(unsigned int handle);
	
		/**
		 *	Play sound.
		 *	Plays the given sound as a 2D sound.
		 * @param handle handle of a registered sound to play
		 * @param loop loops the sound
		 * @param vol volume factor
		 */
		void playSound(unsigned int handle, bool loop = false, float vol = 1.0f);
	
		/**
		 *	Stop sound.
		 *	Stops playing the sound with the given handle.
		 * @param handle handle to the sound to stop
		 */
		void stopSound(unsigned int handle);
	
		/**
		 *	Set listener.
		 *	Sets the listener rotation, position and velocity.
		 * @param pos new position to set the listener to
		 * @param rot new rotation of the listener
		 * @param vel new velocity of the listener
		 */
		void setListener(sgVector3 &pos, sgQuaternion &rot, sgVector3 vel = sgVector3());
	
		/**
		 *	Update listener.
		 *	Updates the listener rotation and position and calculates its velocity from its previous location.
		 * @param pos new position to set the listener to
		 * @param rot new rotation of the listener
		 * @param time of the position change to claculate the velocity with
		 */
		void updateListener(sgVector3 &pos, sgQuaternion &rot, float timestep);
	
		/**
		 *	Update.
		 *	Updates the audio world (is called automatically each frame).
		 * @param timestep time passed since the previous frame
		 */
		void update(float timestep);
	
		/**
		 * First sound source.
		 * Pointer to a first sound source, which is empty, but meant to be used to create new sound sources from.
		 */
		sgSoundSource *first_sndsource;
	
	private:
		std::vector<unsigned int> handles;
		sgVector3 listenerpos;
		ALCdevice *device;
		ALCcontext *context;
};

#endif
