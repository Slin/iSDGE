//
//	sgSound.h
//	Engine
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

#ifndef __SGSOUND_H__
#define __SGSOUND_H__

#include "sgBase.h"

/**
 * Sound class. Stores and handles a sound.
 */
class sgSound : public sgBase
{
	public:
		/**
		 *	Constructor.
		 *	Initializes without a sound.
		 */
		sgSound();
	
		/**
		 *	Deconstructor.
		 *	Frees all memory allocated by this class.
		 */
		~sgSound();
		
		/**
		 *	Create Sound.
		 *	If there isn´t already a sound, this function loads it from the given file.
		 * @param filename the name of the file to load
		 */
		void createSound(const char *filename);
		
		/**
		 *	Get Sound.
		 *	Creates and returns a pointer to a sound loaded from the given file.
		 * @param filename the name of the file to load
		 * @return pointer to the sound classes instance
		 */
		static sgSound *getSound(const char *filename);
		
		/**
		 *	Destroy.
		 *	Destroys the instance.
		 */
		void destroy();
	
		/**
		 *	Sound ID.
		 *	The OpenAL ID of the sound.
		 */
		unsigned int sndid;
	
		/**
		 *	Audio data.
		 *	Pointer to the uncompressed audio data.
		 */
		unsigned char* audiodata;
};

#endif
