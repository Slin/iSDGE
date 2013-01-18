//
//	sgTimer.cpp
//	iSDGE
//
//	Created by Nils Daumann on 08.06.10.
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

#include "sgTimer.h"

sgTimer::sgTimer()
{
#if defined __IOS__
	mach_timebase_info(&timebase);
	timer.start = mach_absolute_time();
#elif defined __ANDROID__
	timer.start.tv_sec = 0;
	timer.start.tv_usec = 0;
	timer.stop.tv_sec = 0;
	timer.stop.tv_usec = 0;
#elif defined __WIN32__
	timer.start.QuadPart = 0;
	timer.stop.QuadPart = 0;
	QueryPerformanceFrequency(&frequency);
#else
	timer.start = 0.0;
	timer.stop = 0.0;
#endif
}

void sgTimer::start( )
{
#if defined __IOS__
	timer.start = mach_absolute_time();
#elif defined __ANDROID__
	gettimeofday(&timer.start, NULL);
#elif defined __WIN32__
	QueryPerformanceCounter(&timer.start);
#else
	timer.start = glfwGetTime();
#endif
}

void sgTimer::stop( )
{
#if defined __IOS__
	timer.stop = mach_absolute_time();
#elif defined __ANDROID__
	gettimeofday(&timer.stop, NULL);
#elif defined __WIN32__
	QueryPerformanceCounter(&timer.stop);
#else
	timer.stop = glfwGetTime();
#endif
}


double sgTimer::getElapsedTime()
{
#if defined __IOS__
	double elapsedtime = (double)(timer.stop-timer.start)*(double)timebase.numer/(double)timebase.denom/1000000000.0f;
#elif defined __ANDROID__
	double elapsedtime = timer.stop.tv_sec-timer.start.tv_sec;
	elapsedtime += (timer.stop.tv_usec-timer.start.tv_usec)/1000000.0f;
#elif defined __WIN32__
	LARGE_INTEGER time;
	time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
	double elapsedtime = ((double)time.QuadPart /(double)frequency.QuadPart);
#else
    double elapsedtime = timer.stop-timer.start;
#endif
	return elapsedtime;
}
