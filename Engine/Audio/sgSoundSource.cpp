//
//	sgSoundSource.cpp
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

#include "sgSoundSource.h"
#include "sgSound.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

sgSoundSource::sgSoundSource(sgSoundSource *p, sgSoundSource *n)
{
	prev = p;
	next = n;
	if(next)
	{
		next->prev = this;
	}
}

sgSoundSource::~sgSoundSource()
{
	if(handles.size() > 0)
		alDeleteSources(handles.size(), &handles[0]);
}

sgSoundSource *sgSoundSource::createSoundSource()
{
	next = new sgSoundSource(this, next);
	return next;
}

unsigned int sgSoundSource::registerSound(sgSound *snd)
{
	unsigned int src;
	alGenSources(1, &src);
	alSourcei(src, AL_SOURCE_RELATIVE, AL_FALSE);
	alSourcefv(src, AL_POSITION, &position.x);
    alSourcei(src, AL_BUFFER, snd->sndid);
	handles.push_back(src);
	return src;
}

void sgSoundSource::unregisterSound(unsigned int handle)
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

void sgSoundSource::playSound(unsigned int handle, bool loop, float nearrange, float falloff, float farrange, float vol)
{
	alSourcef(handle, AL_REFERENCE_DISTANCE, nearrange);
	alSourcef(handle, AL_MAX_DISTANCE, farrange);
	alSourcef(handle, AL_ROLLOFF_FACTOR, falloff);
	alSourcef(handle, AL_GAIN, vol);
	alSourcei(handle, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
	alSourcePlay(handle);
}

void sgSoundSource::stopSound(unsigned int handle)
{
	alSourceStop(handle);
}

bool sgSoundSource::isPlaying(unsigned int handle)
{
	ALenum state;
	alGetSourcei(handle, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void sgSoundSource::update(float timestep)
{
	sgVector3 vel = lastpos-position;
	vel /= timestep;
	lastpos = position;
	
	for(unsigned int i = 0; i < handles.size(); i++)
	{
		alSourcefv(handles[i], AL_POSITION, &position.x);
		alSourcefv(handles[i], AL_VELOCITY, &vel.x);
	}
}

void sgSoundSource::destroy()
{
	if(prev)
		prev->next = next;
	else
		return;
	
	if(next)
		next->prev = prev;
	
	delete this;
}

void sgSoundSource::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
	else
		next = 0;
}