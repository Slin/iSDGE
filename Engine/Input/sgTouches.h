//
//	sgTouches.h
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

#ifndef __SGTOUCHES_H__
#define __SGTOUCHES_H__

#include <vector>
#include "sgVector2.h"

/**
 * Touch struct. Represents one touch.
 */
struct sgTouch
{
	/**
	 *	Absolute position.
	 *	The touches position without rotation.
	 */
	sgVector2 positionabs;
	
	/**
	 *	Position.
	 *	The touches position.
	 */
	sgVector2 position;
	
	/**
	 *	Direction.
	 *	The touches movement direction and speed.
	 */
	sgVector2 direction;
};

/**
 * Touch namespace. Receives and handles touches.
 */
namespace sgTouches
{
	/**
	 *	Touch array.
	 *	Array storing all current touches.
	 */
	extern std::vector<sgTouch*> touches;
	
	/**
	 *	Add touch.
	 *	Adds a new touch to the touch array.
	 * @param pos the position of the new touch
	 * @param abspos the new touches absolute position without device rotation
	 */
	void addTouch(sgVector2 pos, sgVector2 abspos);
	
	/**
	 *	Update touch.
	 *	Updates a touch in the touch array.
	 * @param num the position of the touch within the touch array to update
	 * @param pos the touches new position
	 * @param prepos the touches previous position
	 * @param abspos the new touches absolute position without device rotation
	 */
	void updateTouch(int num, sgVector2 pos, sgVector2 prepos, sgVector2 abspos);
	
	/**
	 *	Remove touch.
	 *	Removes a touch from the touch array.
	 * @param num the position of the touch within the touch array to remove
	 */
	void removeTouch(int num);
}

#endif
