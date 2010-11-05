//
//	sgTimer.h
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

#ifndef __SGTIMER_H__
#define __SGTIMER_H__

//#include <sys/time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

typedef struct
{
/*	timeval start;
	timeval stop;*/
	unsigned long long int start;
	unsigned long long int stop;
}sgTimerWatch;

class sgTimer
{
	public:
		sgTimer();
		void start( );
		void stop( );
		double getElapsedTime();
	
	private:
		sgTimerWatch timer;
		mach_timebase_info_data_t timebase;
};

#endif
