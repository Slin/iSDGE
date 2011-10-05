//
//	sgSoundSource.h
//	iSDGE
//
//	Created by Nils Daumann on 09.07.11.
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

#ifndef __SGSOUNDSOURCE_H__
#define __SGSOUNDSOURCE_H__

#include "sgVector3.h"
#include <vector>

class sgSound;

/**
 * Sound source class. Responsible for 3d sound handling.
 */
class sgSoundSource
{
	public:
		/**
		 *	Constructor.
		 *	Creates a sound source.
		 * @param p the previous sound source in the linked sound source list
		 * @param n the next sound source in the linked sound source list
		 */
		sgSoundSource(sgSoundSource *p, sgSoundSource *n);
	
		/**
		 *	Destructor.
		 *	Cleans everything up.
		 */
		~sgSoundSource();
	
		/**
		 *	Create sound source.
		 *	Creates a new sound source and makes it next of this.
		 * @return pointer to the new sound source
		 */
		sgSoundSource *createSoundSource();
	
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
		 *	Plays the sound from the given handle from this sources position, with the given falloff distance.
		 * @param handle handle returned when registering the sound
		 * @param loop loops the sound
		 * @param nearrange distance to which is sound is played at full volume
		 * @param falloff falloff factor, a higher value causes the sound to get attenuated faster
		 * @param farrange distance after which the sound is clamped to the volume at that distance
		 * @param vol volume factor
		 */
		void playSound(unsigned int handle, bool loop = false, float nearrange = 1.0f, float falloff = 2.0f, float farrange = 1000.0f, float vol = 1.0f);
	
		/**
		 *	Stop sound.
		 *	Stops the sound with the given handle.
		 * @param handle handle to the sound to stop
		 */
		void stopSound(unsigned int handle);
	
		/**
		 *	Is playing.
		 *	Returns weather the sound with the given handle is playing or not.
		 * @param handle handle to the sound to check
		 * @return true if the sound is currently playing, false otherwize
		 */
		bool isPlaying(unsigned int handle);
	
		/**
		 *	Update.
		 *	Updates this sound source (is called automatically each frame).
		 * @param timestep time passed since the previous frame
		 */
		void update(float timestep);
	
		/**
		 *	Destroy.
		 *	Destroys this sound source.
		 */
		void destroy();
	
		/**
		 *	Destroy All.
		 *	Destroys this and all sound sources connected to it.
		 */
		void destroyAll();
		
		/**
		 *	Position.
		 *	The position of the sound source.
		 */
		sgVector3 position;
		
		/**
		 *	Previous.
		 *	Pointer to the previous sound source within the sound source list or NULL if there is none.
		 */
		sgSoundSource *prev;
	
		/**
		 *	Next.
		 *	Pointer to the next sound source within the sound source list or NULL if there is none.
		 */
		sgSoundSource *next;
	
	private:
		std::vector<unsigned int> handles;
		sgVector3 lastpos;
};

#endif
