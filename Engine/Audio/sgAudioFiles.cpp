//
//	sgTextureFiles.cpp
//	iSDGE
//
//	Created by Nils Daumann on 23.03.10.
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

#include "sgAudioFiles.h"

#include "sgXML.h"
#include "sgResourceManager.h"
#include "sgDebug.h"

#if defined __IOS__ && defined __OPEN_AL__
	#include <AudioToolbox/AudioToolbox.h>
	#include <CoreFoundation/CoreFoundation.h>
#endif


namespace sgAudioFiles
{
	bool loadAudio(sgUncompressedAudio **audio, const char *filename)
	{
#if defined __IOS__ && defined __OPEN_AL__
		const char *filepath = sgResourceManager::getPath(filename);
		CFStringRef pathstring = CFStringCreateWithCString(NULL, filepath, kCFStringEncodingASCII);
		delete[] filepath;

		CFStringRef finalpathstr = CFURLCreateStringByAddingPercentEscapes(NULL, pathstring, NULL, NULL, kCFStringEncodingUTF8);
		CFURLRef pathasurl = CFURLCreateWithString(NULL, finalpathstr, NULL);
		CFRelease(pathstring);
		CFRelease(finalpathstr);

		ExtAudioFileRef audiofileobject = NULL;
		OSStatus err = ExtAudioFileOpenURL(pathasurl, &audiofileobject);
		CFRelease(pathasurl);

		if(err)
		{
			sgLog("Could not open audio file: %s", filename);
			return false;
		}

		long long int theFileLengthInFrames = 0;
		AudioStreamBasicDescription theFileFormat;
		unsigned long int thePropertySize = sizeof(theFileFormat);
		AudioStreamBasicDescription theOutputFormat;

		// Get the audio data format
		err = ExtAudioFileGetProperty(audiofileobject, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
		if(theFileFormat.mChannelsPerFrame > 2)
		{
			sgLog("Unsupported Format, channel count is greater than stereo: %s", filename);
			ExtAudioFileDispose(audiofileobject);
			return false;
		}

		// Set the client format to 16 bit signed integer (native-endian) data
		// Maintain the channel count and sample rate of the original source format
		theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
		theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
		theOutputFormat.mFormatID = kAudioFormatLinearPCM;
		theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
		theOutputFormat.mFramesPerPacket = 1;
		theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
		theOutputFormat.mBitsPerChannel = 16;
		theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;

		// Set the desired client (output) data format
		err = ExtAudioFileSetProperty(audiofileobject, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
		if(err)
		{
			sgLog("Loading of audio file failed: %s", filename);
			ExtAudioFileDispose(audiofileobject);
			return false;
		}

		// Get the total frame count
		thePropertySize = sizeof(long long int);
		err = ExtAudioFileGetProperty(audiofileobject, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
		if(err)
		{
			sgLog("Loading of audio file failed: %s Error = %ld", filename, err);
			ExtAudioFileDispose(audiofileobject);
			return false;
		}

		*audio = new sgUncompressedAudio;

		// Read all the data into memory
		(*audio)->bitsPerSample = theOutputFormat.mBitsPerChannel;
		(*audio)->dataLength = (unsigned long)theFileLengthInFrames*theOutputFormat.mBytesPerFrame;
		(*audio)->bytes = new unsigned char[(*audio)->dataLength];
		if((*audio)->bytes)
		{
			AudioBufferList theDataBuffer;
			theDataBuffer.mNumberBuffers = 1;
			theDataBuffer.mBuffers[0].mDataByteSize = (*audio)->dataLength;
			theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
			theDataBuffer.mBuffers[0].mData = (*audio)->bytes;

			// Read the data into an AudioBufferList
			err = ExtAudioFileRead(audiofileobject, (unsigned long int*)&theFileLengthInFrames, &theDataBuffer);
			if(err == noErr)
			{
				(*audio)->channelCount = theOutputFormat.mChannelsPerFrame;
				(*audio)->sampleRate = theOutputFormat.mSampleRate;
			}
			else
			{
				// failure
				delete[] (*audio)->bytes;
				(*audio)->bytes = NULL;
				sgLog("Failed to read audio data from file: %s", filename);
				ExtAudioFileDispose(audiofileobject);
				return false;
			}
		}

		ExtAudioFileDispose(audiofileobject);

		return true;
#elif defined __OPEN_AL__
		//Local Declarations
		FILE* soundFile = NULL;
		WAVE_Format wave_format;
		RIFF_Header riff_header;
		WAVE_Data wave_data;
		unsigned char* data;

		const char *filepath = sgResourceManager::getPath(filename);
		soundFile = fopen(filepath, "rb");
		delete[] filepath;
		if(!soundFile)
		{
			sgLog("Could not open audio file: %s", filename);
			return false;
		}

		// Read in the first chunk into the struct
		fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		//check for RIFF and WAVE tag in memeory
		if((riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F') &&
			(riff_header.format[0] != 'W' ||
			riff_header.format[1] != 'A' ||
			riff_header.format[2] != 'V' ||
			riff_header.format[3] != 'E'))
			{
				sgLog("Invalid RIFF or WAVE Header: %s", filename);
				fclose(soundFile);
				return false;
			}

		//Read in the 2nd chunk for the wave info
		fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);

		//check for fmt tag in memory
		if(wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ')
			{
				sgLog("Invalid Wave Format: %s", filename);
				fclose(soundFile);
				return false;
			}

		//check for extra parameters;
		if(wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		//Read in the the last byte of data before the sound file
		fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);

		//check for data tag in memory
		if(wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a')
			{
				sgLog("Invalid data header: %s", filename);
				fclose(soundFile);
				return false;
			}

		//Allocate memory for data
		data = new unsigned char[wave_data.subChunk2Size];

		// Read in the sound data into the soundData variable
		if(!fread(data, wave_data.subChunk2Size, 1, soundFile))
		{
			sgLog("Error loading WAVE data into struct: %s", filename);
			fclose(soundFile);
			return false;
		}

		*audio = new sgUncompressedAudio;

		(*audio)->channelCount = wave_format.numChannels;
		(*audio)->sampleRate = wave_format.sampleRate;
		(*audio)->dataLength = wave_data.subChunk2Size;
		(*audio)->bytes = data;
		(*audio)->bitsPerSample = wave_format.bitsPerSample;

		//clean up and return true if successful
		fclose(soundFile);
		return true;
#else
		return false;
#endif
	}
}
