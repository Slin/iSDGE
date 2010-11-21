//
//	sgVector2.cpp
//	iSDGE
//
//	Created by Nils Daumann on 17.04.10.
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

#include "sgVector2.h"
#include <cmath>

sgVector2::sgVector2(float x_, float y_)
{
	x = x_;
	y = y_;
}

sgVector2::sgVector2(const sgVector2 &other)
{
	x = other.x;
	y = other.y;
}

bool sgVector2::operator== (const sgVector2 &other)
{
	return (x == other.x && y == other.y);
}

bool sgVector2::operator!= (const sgVector2 &other)
{
	return (x != other.x || y != other.y);
}

sgVector2 &sgVector2::operator= (const sgVector2 &other)
{
	x = other.x;
	y = other.y;
	return *this;
}

sgVector2 &sgVector2::operator= (const float &other)
{
	x = other;
	y = other;
	return *this;
}

sgVector2 &sgVector2::operator+= (const sgVector2 &other)
{
	x += other.x;
	y += other.y;
	return *this;
}

sgVector2 &sgVector2::operator-= (const sgVector2 &other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

sgVector2 &sgVector2::operator*= (const sgVector2 &other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}

sgVector2 &sgVector2::operator/= (const sgVector2 &other)
{
	x /= other.x;
	y /= other.y;
	return *this;
}

sgVector2 sgVector2::operator+ (const sgVector2 &other)
{
	sgVector2 res(*this);
	res.x += other.x;
	res.y += other.y;
	return res;
}

sgVector2 sgVector2::operator- (const sgVector2 &other)
{
	sgVector2 res(*this);
	res.x -= other.x;
	res.y -= other.y;
	return res;
}

sgVector2 sgVector2::operator* (const sgVector2 &other)
{
	sgVector2 res(*this);
	res.x *= other.x;
	res.y *= other.y;
	return res;
}

sgVector2 sgVector2::operator/ (const sgVector2 &other)
{
	sgVector2 res(*this);
	res.x /= other.x;
	res.y /= other.y;
	return res;
}

sgVector2 &sgVector2::operator+= (const float &other)
{
	x += other;
	y += other;
	return *this;
}

sgVector2 &sgVector2::operator-= (const float &other)
{
	x -= other;
	y -= other;
	return *this;
}

sgVector2 &sgVector2::operator*= (const float &other)
{
	x *= other;
	y *= other;
	return *this;
}

sgVector2 &sgVector2::operator/= (const float &other)
{
	x /= other;
	y /= other;
	return *this;
}

sgVector2 sgVector2::operator+ (const float &other)
{
	sgVector2 res(*this);
	res.x += other;
	res.y += other;
	return res;
}

sgVector2 sgVector2::operator- (const float &other)
{
	sgVector2 res(*this);
	res.x -= other;
	res.y -= other;
	return res;
}

sgVector2 sgVector2::operator* (const float &other)
{
	sgVector2 res(*this);
	res.x *= other;
	res.y *= other;
	return res;
}

sgVector2 sgVector2::operator/ (const float &other)
{
	sgVector2 res(*this);
	res.x /= other;
	res.y /= other;
	return res;
}

void sgVector2::makeIdentity()
{
	x = 0.0;
	y = 0.0;
}

float sgVector2::length()
{
	return sqrt(x*x+y*y);
}

float sgVector2::squaredLength()
{
	return x*x+y*y;
}

void sgVector2::normalize()
{
	float len = length();
	if(len != 0)
	{
		float fac = 1/len;
		x *= fac;
		y *= fac;
	}
}

float sgVector2::dot(sgVector2 &other)
{
	return x*other.x+y*other.y;
}

float sgVector2::dist(sgVector2 &other)
{
	sgVector2 temp(*this);
	temp -= other;
	return temp.length();
}

