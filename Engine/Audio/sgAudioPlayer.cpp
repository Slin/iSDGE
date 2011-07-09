//
//	sgPhysWorld.cpp
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

#include "sgAudioPlayer.h"
#include "sgAudioFiles.h"
#include "sgVector3.h"
#include "sgQuaternion.h"
#include <float.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

sgAudioPlayer::sgAudioPlayer()
{
    device = alcOpenDevice(NULL);
    if(device != NULL)
    {
        context = alcCreateContext(device, 0);
        if(context != NULL)
        {
            alcMakeContextCurrent(context);
        }
    }
    alGetError();	//clears all errors
	
	first_sndsource = new sgSoundSource(NULL, NULL);
}

sgAudioPlayer::~sgAudioPlayer()
{
	first_sndsource->destroyAll();
	delete first_sndsource;
	if(handles.size() > 0)
	{
		alDeleteSources(handles.size(), &handles[0]);
	}
    alcDestroyContext(context);
    alcCloseDevice(device);
}

unsigned int sgAudioPlayer::registerSound(sgSound *snd)
{
	static float sourcePosAL[] = {0.0f, 0.0f, 0.0f};
	unsigned int src;
	alGenSources(1, &src);
	alSourcei(src, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef(src, AL_REFERENCE_DISTANCE, FLT_MAX-10.0f);
	alSourcef(src, AL_MAX_DISTANCE, FLT_MAX-9.0f);
	alSourcefv(src, AL_POSITION, sourcePosAL);
    alSourcei(src, AL_BUFFER, snd->sndid);
	handles.push_back(src);
	return src;
}

void sgAudioPlayer::unregisterSound(unsigned int handle)
{
	alSourceStop(handle);
	alDeleteSources(1, &handle);
	for(unsigned int i = 0; i < handles.size(); i++)
	{
		if(handles[i] == handle)
		{
			handles.erase(handles.begin()+i);
			break;
		}
	}
}

void sgAudioPlayer::playSound(unsigned int handle, bool loop, float vol)
{
	alSourcef(handle, AL_GAIN, vol);
	alSourcei(handle, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
	alSourcePlay(handle);
}

void sgAudioPlayer::stopSound(unsigned int handle)
{
	alSourceStop(handle);
}

void sgAudioPlayer::setListener(sgVector3 &pos, sgQuaternion &rot, sgVector3 vel)
{
	sgVector3 upat[2];
	upat[0] = rot.rotate(sgVector3(0.0, 1.0, 0.0));
	upat[1] = rot.rotate(sgVector3(0.0, 0.0, 1.0));
	alListenerfv(AL_ORIENTATION, &upat[0].x);
	alListenerfv(AL_POSITION, &pos.x);
	alListenerfv(AL_VELOCITY, &vel.x);
	listenerpos = pos;
}

void sgAudioPlayer::updateListener(sgVector3 &pos, sgQuaternion &rot, float timestep)
{
	sgVector3 upat[2];
	upat[0] = rot.rotate(sgVector3(0.0, 1.0, 0.0));
	upat[1] = rot.rotate(sgVector3(0.0, 0.0, 1.0));
	alListenerfv(AL_ORIENTATION, &upat[0].x);
	alListenerfv(AL_POSITION, &pos.x);
	sgVector3 vel = listenerpos-pos;
	vel /= timestep;
	alListenerfv(AL_VELOCITY, &vel.x);
	listenerpos = pos;
}

void sgAudioPlayer::update(float timestep)
{
	for(sgSoundSource *src = first_sndsource->next; src != NULL; src = src->next)
	{
		src->update(timestep);
	}
}
