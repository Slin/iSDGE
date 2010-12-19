//
//	sgTouches.cpp
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

#include "sgTouches.h"

namespace sgTouches
{
	std::vector<sgTouch*> touches;
	
	void addTouch(sgVector2 pos, sgVector2 abspos)
	{
		sgTouch *touch = new sgTouch;
		touch->position = pos;
		touch->direction = sgVector2(0, 0);
		touch->positionabs = abspos;
		touches.push_back(touch);
	}
	
	void updateTouch(int num, sgVector2 pos, sgVector2 prepos, sgVector2 abspos)
	{
		touches[num]->position = pos;
		touches[num]->direction = sgVector2(prepos.x-pos.x, prepos.y-pos.y);
		touches[num]->positionabs = abspos;
	}
	
	void removeTouch(int num)
	{
		delete touches[num];
		touches.erase(touches.begin()+num);
	}
	
	void resetDirections()
	{
		for(int i = 0; i < touches.size(); i++)
		{
			touches[i]->direction = 0.0;
		}
	}
}
