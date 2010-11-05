//
//	sgEvents.h
//	iSDGE
//
//	Created by Nils Daumann on 22.04.10.
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

#ifndef __SGEVENTS_H__
#define __SGEVENTS_H__

class sgMain;

/**
 * Event class. Inherit from this one and assign an instance of it to the view to handle engine events.
 */
class sgEvents
{
	public:
		/**
		 *	On init.
		 *	This function is called just before the first frame is rendered.
		 * @param pointer to the engines main class
		 */
		virtual void onInit(sgMain *main);
	
		/**
		 *	On draw.
		 *	This function is called every frame just before anything gets rendered, before all entities onDraw events.
		 * @param timestep time in seconds since the last frame was rendered
		 */
		virtual void onDraw(float timestep);
	
		/**
		 *	On draw late.
		 *	This function is called every frame just before anything gets rendered, after all entities onDraw events.
		 * @param timestep time in seconds since the last frame was rendered
		 */
		virtual void onDrawLate(float timestep);
};

#endif
