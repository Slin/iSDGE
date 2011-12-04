//
//	sgVector3.cpp
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

#include "sgVector3.h"

#include <cmath>
#include "sgMatrix4x4.h"
#include "sgQuaternion.h"
#include "sgVector4.h"

sgVector3::sgVector3(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

sgVector3::sgVector3(const float *other)
{
	x = other[0];
	y = other[1];
	z = other[2];
}

sgVector3::sgVector3(const sgVector3 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

sgVector3::sgVector3(const sgVector4 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

sgVector3::sgVector3(const sgQuaternion &other)
{
	sgQuaternion q(other);
	*this = q.getEuler();
}

bool sgVector3::operator== (const sgVector3 &other)
{
	return (x == other.x && y == other.y && z == other.z);
}

bool sgVector3::operator!= (const sgVector3 &other)
{
	return (x != other.x || y != other.y || z != other.z);
}

sgVector3 &sgVector3::operator= (const sgVector3 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

sgVector3 &sgVector3::operator= (const sgVector4 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

sgVector3 &sgVector3::operator= (const float &other)
{
	x = other;
	y = other;
	z = other;
	return *this;
}

sgVector3 &sgVector3::operator= (const sgQuaternion &other)
{
	sgQuaternion q(other);
	*this = q.getEuler();
	return *this;
}

sgVector3 &sgVector3::operator+= (const sgVector3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

sgVector3 &sgVector3::operator-= (const sgVector3 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

sgVector3 &sgVector3::operator*= (const sgVector3 &other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

sgVector3 &sgVector3::operator/= (const sgVector3 &other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

sgVector3 sgVector3::operator+ (const sgVector3 &other)
{
	sgVector3 res(*this);
	res.x += other.x;
	res.y += other.y;
	res.z += other.z;
	return res;
}

sgVector3 sgVector3::operator- (const sgVector3 &other)
{
	sgVector3 res(*this);
	res.x -= other.x;
	res.y -= other.y;
	res.z -= other.z;
	return res;
}

sgVector3 sgVector3::operator* (const sgVector3 &other)
{
	sgVector3 res(*this);
	res.x *= other.x;
	res.y *= other.y;
	res.z *= other.z;
	return res;
}

sgVector3 sgVector3::operator/ (const sgVector3 &other)
{
	sgVector3 res(*this);
	res.x /= other.x;
	res.y /= other.y;
	res.z /= other.z;
	return res;
}

sgVector3 &sgVector3::operator+= (const float &other)
{
	x += other;
	y += other;
	z += other;
	return *this;
}

sgVector3 &sgVector3::operator-= (const float &other)
{
	x -= other;
	y -= other;
	z -= other;
	return *this;
}

sgVector3 &sgVector3::operator*= (const float &other)
{
	x *= other;
	y *= other;
	z *= other;
	return *this;
}

sgVector3 &sgVector3::operator/= (const float &other)
{
	x /= other;
	y /= other;
	z /= other;
	return *this;
}

sgVector3 sgVector3::operator+ (const float &other)
{
	sgVector3 res(*this);
	res.x += other;
	res.y += other;
	res.z += other;
	return res;
}

sgVector3 sgVector3::operator- (const float &other)
{
	sgVector3 res(*this);
	res.x -= other;
	res.y -= other;
	res.z -= other;
	return res;
}

sgVector3 sgVector3::operator* (const float &other)
{
	sgVector3 res(*this);
	res.x *= other;
	res.y *= other;
	res.z *= other;
	return res;
}

sgVector3 sgVector3::operator/ (const float &other)
{
	sgVector3 res(*this);
	res.x /= other;
	res.y /= other;
	res.z /= other;
	return res;
}

sgVector3 &sgVector3::operator*= (const sgMatrix4x4 &other)
{
	sgVector3 res;
	res.x = other.mat[0]*x+other.mat[1]*x+other.mat[2]*x+other.mat[3]*x;
	res.y = other.mat[4]*y+other.mat[5]*y+other.mat[6]*y+other.mat[7]*y;
	res.z = other.mat[8]*z+other.mat[9]*z+other.mat[10]*z+other.mat[11]*z;
	*this = res;
	return *this;
}

sgVector3 sgVector3::operator* (const sgMatrix4x4 &other)
{
	sgVector3 res;
	res.x = other.mat[0]*x+other.mat[1]*x+other.mat[2]*x+other.mat[3]*x;
	res.y = other.mat[4]*y+other.mat[5]*y+other.mat[6]*y+other.mat[7]*y;
	res.z = other.mat[8]*z+other.mat[9]*z+other.mat[10]*z+other.mat[11]*z;
	return res;
}

void sgVector3::makeIdentity()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

float sgVector3::length()
{
	return sqrt(x*x+y*y+z*z);
}

float sgVector3::squaredLength()
{
	return x*x+y*y+z*z;
}

void sgVector3::normalize()
{
	float len = length();
	if(len != 0)
	{
		float fac = 1/len;
		x *= fac;
		y *= fac;
		z *= fac;
	}
}

float sgVector3::dot(sgVector3 &other)
{
	return x*other.x+y*other.y+z*other.z;
}

float sgVector3::dot(sgVector4 &other)
{
	return x*other.x+y*other.y+z*other.z;
}

sgVector3 sgVector3::cross(sgVector3 &other)
{
	sgVector3 res;
	res.x = y*other.z-z*other.y;
	res.y = z*other.x-x*other.z;
	res.z = x*other.y-y*other.x;
	return res;
}

float sgVector3::dist(sgVector3 &other)
{
	sgVector3 temp(*this);
	temp -= other;
	return temp.length();
}

float sgVector3::dist(sgVector4 &other)
{
	sgVector3 temp(other);
	temp -= *this;
	return temp.length();
}
