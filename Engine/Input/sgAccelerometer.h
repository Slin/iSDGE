//
//	sgAccelerometer.h
//	Engine
//
//	Created by Nils Daumann on 18.04.10.
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

#ifndef __SGACCELEROMETER_H__
#define __SGACCELEROMETER_H__

#include "sgVector3.h"

/**
 * Accelerometer namespace. Handles the accelerometer.
 */
namespace sgAccelerometer
{
	/**
	 *	Current absolute acceleration.
	 *	The current absolute values provided by the accelerometer, without device rotation.
	 */
	extern sgVector3 curraccabs;
	
	/**
	 *	Current acceleration.
	 *	The current values provided by the accelerometer.
	 */
	extern sgVector3 curracc;
	
	/**
	 *	Current smoothed absolute acceleration.
	 *	The current smoothed absolute values provided by the accelerometer, without device rotation. This basicly represents gravity.
	 */
	extern sgVector3 currsmoothaccabs;
	
	/**
	 *	Current smoothed acceleration.
	 *	The current smoothed values provided by the accelerometer. This basicly represents gravity.
	 */
	extern sgVector3 currsmoothacc;
	
	/**
	 *	Current heading.
	 *	The current heading in degrees given by the gyrometer.
	 */
	extern float currheading;
}

#endif
