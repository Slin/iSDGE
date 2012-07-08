//
//	sgSound.cpp
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

#include "sgSound.h"
#include "sgAudioFiles.h"
#include "sgResourceManager.h"
#include "sgDebug.h"

#if defined __OPEN_AL__
	#if defined __IOS__
		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>
	#else
		#include <al.h>
		#include <alc.h>
	#endif
#endif

sgSound::sgSound()
{
	sndid = -1;
	audiodata = 0;
}

sgSound::~sgSound()
{
#if defined __OPEN_AL__
	if(sndid != -1)
		alDeleteBuffers(1, &sndid);

	if(audiodata)
		delete[] audiodata;
#endif
}

void sgSound::createSound(const char *filename)
{
	if(sndid != -1)
		return;

#if defined __OPEN_AL__
	sgAudioFiles::sgUncompressedAudio *snd = 0;
	if(!sgAudioFiles::loadAudio(&snd, filename))
	{
		if(snd)
		{
			if(snd->bytes)
				delete[] snd->bytes;
			delete snd;
		}

		sgLog("Can´t load audio file: %s", filename);
		return;
	}

	alGenBuffers(1, &sndid);
	ALenum format = 0;
	if(snd->channelCount == 1)
	{
		if(snd->bitsPerSample == 8 )
			format = AL_FORMAT_MONO8;
		else if(snd->bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
	}
	else if(snd->channelCount == 2)
	{
		if(snd->bitsPerSample == 8 )
			format = AL_FORMAT_STEREO8;
		else if(snd->bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;
	}
#if defined __IOS__
	typedef ALvoid  AL_APIENTRY (*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);
	static alBufferDataStaticProcPtr proc = NULL;
	if (proc == NULL)
	{
        proc = (alBufferDataStaticProcPtr)alcGetProcAddress(NULL, (const ALCchar*)"alBufferDataStatic");
    }
    if(proc)
	{
        proc(sndid, format, snd->bytes, snd->dataLength, snd->sampleRate);
	}
#else
	alBufferData(sndid, format, snd->bytes, snd->dataLength, snd->sampleRate);
#endif

	audiodata = snd->bytes;
	delete snd;
#endif

	sgResourceManager::addResource(filename, this);
}

sgSound *sgSound::getSound(const char *filename)
{
	sgSound *snd = (sgSound*)sgResourceManager::getResource(filename);
	if(snd != NULL)
		return snd;

	snd = new sgSound();
	snd->createSound(filename);

	return snd;
}

void sgSound::destroy()
{
	sgResourceManager::removeResource(this);
	delete this;
}
