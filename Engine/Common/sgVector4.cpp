//
//	sgVector4.cpp
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

#include "sgVector4.h"

#include <cmath>
#include "sgMatrix4x4.h"
#include "sgQuaternion.h"

sgVector4::sgVector4(float x_, float y_, float z_, float w_)
{
	x = x_;
	y = y_;
	z = z_;
	w = w_;
}

sgVector4::sgVector4(const sgVector4 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

sgVector4::sgVector4(const sgQuaternion &other)
{
	sgQuaternion q(other);
	*this = q.getAxisAngle();
}

bool sgVector4::operator== (const sgVector4 &other)
{
	return (x == other.x && y == other.y && z == other.z && w == other.w);
}

bool sgVector4::operator!= (const sgVector4 &other)
{
	return (x != other.x || y != other.y || z != other.z || w != other.w);
}

sgVector4 &sgVector4::operator= (const sgVector4 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

sgVector4 &sgVector4::operator= (const float &other)
{
	x = other;
	y = other;
	z = other;
	w = other;
	return *this;
}

sgVector4 &sgVector4::operator= (const sgQuaternion &other)
{
	sgQuaternion q(other);
	*this = q.getAxisAngle();
	return *this;
}

sgVector4 &sgVector4::operator+= (const sgVector4 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

sgVector4 &sgVector4::operator-= (const sgVector4 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

sgVector4 &sgVector4::operator*= (const sgVector4 &other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

sgVector4 &sgVector4::operator/= (const sgVector4 &other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
	return *this;
}

sgVector4 sgVector4::operator+ (const sgVector4 &other)
{
	sgVector4 res(*this);
	res.x += other.x;
	res.y += other.y;
	res.z += other.z;
	res.w += other.w;
	return  res;
}

sgVector4 sgVector4::operator- (const sgVector4 &other)
{
	sgVector4 res(*this);
	res.x -= other.x;
	res.y -= other.y;
	res.z -= other.z;
	res.w -= other.w;
	return  res;
}

sgVector4 sgVector4::operator* (const sgVector4 &other)
{
	sgVector4 res(*this);
	res.x *= other.x;
	res.y *= other.y;
	res.z *= other.z;
	res.w *= other.w;
	return  res;
}

sgVector4 sgVector4::operator/ (const sgVector4 &other)
{
	sgVector4 res(*this);
	res.x /= other.x;
	res.y /= other.y;
	res.z /= other.z;
	res.w /= other.w;
	return res;
}

sgVector4 &sgVector4::operator+= (const float &other)
{
	x += other;
	y += other;
	z += other;
	w += other;
	return *this;
}

sgVector4 &sgVector4::operator-= (const float &other)
{
	x -= other;
	y -= other;
	z -= other;
	w -= other;
	return *this;
}

sgVector4 &sgVector4::operator*= (const float &other)
{
	x *= other;
	y *= other;
	z *= other;
	w *= other;
	return *this;
}

sgVector4 &sgVector4::operator/= (const float &other)
{
	x /= other;
	y /= other;
	z /= other;
	w /= other;
	return *this;
}

sgVector4 sgVector4::operator+ (const float &other)
{
	sgVector4 res(*this);
	res.x += other;
	res.y += other;
	res.z += other;
	res.w += other;
	return res;
}

sgVector4 sgVector4::operator- (const float &other)
{
	sgVector4 res(*this);
	res.x -= other;
	res.y -= other;
	res.z -= other;
	res.w -= other;
	return res;
}

sgVector4 sgVector4::operator* (const float &other)
{
	sgVector4 res(*this);
	res.x *= other;
	res.y *= other;
	res.z *= other;
	res.w *= other;
	return res;
}

sgVector4 sgVector4::operator/ (const float &other)
{
	sgVector4 res(*this);
	res.x /= other;
	res.y /= other;
	res.z /= other;
	res.w /= other;
	return res;
}

sgVector4 &sgVector4::operator*= (const sgMatrix4x4 &other)
{
	sgVector4 res;
	res.x = other.mat[0]*x+other.mat[1]*x+other.mat[2]*x+other.mat[3]*x;
	res.y = other.mat[4]*y+other.mat[5]*y+other.mat[6]*y+other.mat[7]*y;
	res.z = other.mat[8]*z+other.mat[9]*z+other.mat[10]*z+other.mat[11]*z;
	res.w = other.mat[12]*w+other.mat[13]*w+other.mat[14]*w+other.mat[15]*w;
	*this = res;
	return *this;
}

sgVector4 sgVector4::operator* (const sgMatrix4x4 &other)
{
	sgVector4 res;
	res.x = other.mat[0]*x+other.mat[1]*x+other.mat[2]*x+other.mat[3]*x;
	res.y = other.mat[4]*y+other.mat[5]*y+other.mat[6]*y+other.mat[7]*y;
	res.z = other.mat[8]*z+other.mat[9]*z+other.mat[10]*z+other.mat[11]*z;
	res.w = other.mat[12]*w+other.mat[13]*w+other.mat[14]*w+other.mat[15]*w;
	return res;
}

void sgVector4::makeIdentity()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
}

float sgVector4::length()
{
	return sqrt(x*x+y*y+z*z+w*w);
}

float sgVector4::squaredLength()
{
	return x*x+y*y+z*z+w*w;
}

void sgVector4::normalize()
{
	float len = length();
	if(len != 0)
	{
		float fac = 1/len;
		x *= fac;
		y *= fac;
		z *= fac;
		w *= fac;
	}
}

float sgVector4::dot(sgVector4 &other)
{
	return x*other.x+y*other.y+z*other.z+w*other.w;
}

sgVector4 sgVector4::cross(sgVector4 &other)
{
	sgVector4 res;
	res.x = y*other.z-z*other.y;
	res.y = z*other.x-x*other.z;
	res.z = x*other.y-y*other.x;
	res.w = 0.0;
	return res;
}

float sgVector4::dist(sgVector4 &other)
{
	sgVector4 temp(*this);
	temp -= other;
	return temp.length();
}
