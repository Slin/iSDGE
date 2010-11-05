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
/*	timer.start.tv_sec = 0;
	timer.start.tv_usec = 0;
	timer.stop.tv_sec = 0;
	timer.stop.tv_usec = 0;*/
	
	mach_timebase_info(&timebase);
	timer.start = mach_absolute_time();
}

void sgTimer::start( )
{
//	gettimeofday(&timer.start, NULL);
	timer.start = mach_absolute_time();
}

void sgTimer::stop( )
{
//	gettimeofday(&timer.stop, NULL);
	timer.stop = mach_absolute_time();
}


double sgTimer::getElapsedTime()
{
/*	double elapsedtime = timer.stop.tv_sec-timer.start.tv_sec;
	elapsedtime += (timer.stop.tv_usec-timer.start.tv_usec)/1000000.0f;*/
	double elapsedtime = (double)(timer.stop-timer.start)*(double)timebase.numer/(double)timebase.denom/1000000000.0f;
	return elapsedtime;
}
