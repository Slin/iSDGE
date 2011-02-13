//
//  sgMatrix4x4.cpp
//  Engine
//
//	Created by Nils Daumann on 24.05.10.
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

#import "sgMatrix4x4.h"

#include <cmath>
#include <cstring>

#include "sgVector4.h"
#include "sgVector3.h"
#include "sgQuaternion.h"

sgMatrix4x4::sgMatrix4x4()
{
	makeIdentity();
}

sgMatrix4x4::sgMatrix4x4(const sgMatrix4x4 &other)
{
	set((float*)other.mat);
}

sgMatrix4x4::sgMatrix4x4(float *other)
{
	set(other);
}

bool sgMatrix4x4::operator== (const sgMatrix4x4 &other)
{
	return (memcmp(mat, other.mat, sizeof(float)*16) == 0);
}

bool sgMatrix4x4::operator!= (const sgMatrix4x4 &other)
{
	return (memcmp(mat, other.mat, sizeof(float)*16) != 0);
}

sgMatrix4x4 &sgMatrix4x4::operator= (const sgMatrix4x4 &other)
{
	set((float*)other.mat);
	return *this;
}

sgMatrix4x4 &sgMatrix4x4::operator= (float *other)
{
	set(other);
	return *this;
}

sgMatrix4x4 sgMatrix4x4::operator* (const sgMatrix4x4 &other)
{
	sgMatrix4x4 res(mat);
	res *= other;
	return res;
}

sgMatrix4x4 &sgMatrix4x4::operator*= (const sgMatrix4x4 &other)
{
	sgMatrix4x4 temp(mat);

	mat[0] = temp.mat[0]*other.mat[0]+temp.mat[4]*other.mat[1]+temp.mat[8]*other.mat[2]+temp.mat[12]*other.mat[3];
	mat[1] = temp.mat[1]*other.mat[0]+temp.mat[5]*other.mat[1]+temp.mat[9]*other.mat[2]+temp.mat[13]*other.mat[3];
	mat[2] = temp.mat[2]*other.mat[0]+temp.mat[6]*other.mat[1]+temp.mat[10]*other.mat[2]+temp.mat[14]*other.mat[3];
	mat[3] = temp.mat[3]*other.mat[0]+temp.mat[7]*other.mat[1]+temp.mat[11]*other.mat[2]+temp.mat[15]*other.mat[3];
	
	mat[4] = temp.mat[0]*other.mat[4]+temp.mat[4]*other.mat[5]+temp.mat[8]*other.mat[6]+temp.mat[12]*other.mat[7];
	mat[5] = temp.mat[1]*other.mat[4]+temp.mat[5]*other.mat[5]+temp.mat[9]*other.mat[6]+temp.mat[13]*other.mat[7];
	mat[6] = temp.mat[2]*other.mat[4]+temp.mat[6]*other.mat[5]+temp.mat[10]*other.mat[6]+temp.mat[14]*other.mat[7];
	mat[7] = temp.mat[3]*other.mat[4]+temp.mat[7]*other.mat[5]+temp.mat[11]*other.mat[6]+temp.mat[15]*other.mat[7];
	
	mat[8] = temp.mat[0]*other.mat[8]+temp.mat[4]*other.mat[9]+temp.mat[8]*other.mat[10]+temp.mat[12]*other.mat[11];
	mat[9] = temp.mat[1]*other.mat[8]+temp.mat[5]*other.mat[9]+temp.mat[9]*other.mat[10]+temp.mat[13]*other.mat[11];
	mat[10] = temp.mat[2]*other.mat[8]+temp.mat[6]*other.mat[9]+temp.mat[10]*other.mat[10]+temp.mat[14]*other.mat[11];
	mat[11] = temp.mat[3]*other.mat[8]+temp.mat[7]*other.mat[9]+temp.mat[11]*other.mat[10]+temp.mat[15]*other.mat[11];
	
	mat[12] = temp.mat[0]*other.mat[12]+temp.mat[4]*other.mat[13]+temp.mat[8]*other.mat[14]+temp.mat[12]*other.mat[15];
	mat[13] = temp.mat[1]*other.mat[12]+temp.mat[5]*other.mat[13]+temp.mat[9]*other.mat[14]+temp.mat[13]*other.mat[15];
	mat[14] = temp.mat[2]*other.mat[12]+temp.mat[6]*other.mat[13]+temp.mat[10]*other.mat[14]+temp.mat[14]*other.mat[15];
	mat[15] = temp.mat[3]*other.mat[12]+temp.mat[7]*other.mat[13]+temp.mat[11]*other.mat[14]+temp.mat[15]*other.mat[15];

	return *this;
}

sgVector4 sgMatrix4x4::operator* (const sgVector4 &other)
{
	sgVector4 res;
	res.x = mat[0]*other.x+mat[4]*other.y+mat[8]*other.z+mat[12]*other.w;
	res.y = mat[1]*other.x+mat[5]*other.y+mat[9]*other.z+mat[13]*other.w;
	res.z = mat[2]*other.x+mat[6]*other.y+mat[10]*other.z+mat[14]*other.w;
	res.w = mat[3]*other.x+mat[7]*other.y+mat[11]*other.z+mat[15]*other.w;
	return res;
}

sgVector3 sgMatrix4x4::operator* (const sgVector3 &other)
{
	sgVector3 res;
	res.x = mat[0]*other.x+mat[4]*other.y+mat[8]*other.z+mat[12];
	res.y = mat[1]*other.x+mat[5]*other.y+mat[9]*other.z+mat[13];
	res.z = mat[2]*other.x+mat[6]*other.y+mat[10]*other.z+mat[14];
	return res;
}

sgVector4 sgMatrix4x4::transform(const sgVector4 &other)
{
	sgVector4 res;
	res = (*this)*other;
	return res;
}

sgVector3 sgMatrix4x4::transform(const sgVector3 &other)
{
	sgVector3 res;
	res = (*this)*other;
	return res;
}

void sgMatrix4x4::set(float *other)
{
	memcpy(mat, other, sizeof(float)*16);
}

void sgMatrix4x4::makeIdentity()
{
	memset(mat, 0, sizeof(float)*16);
	mat[0] = 1.0;
	mat[5] = 1.0;
	mat[10] = 1.0;
	mat[15] = 1.0;
}

void sgMatrix4x4::makeProjectionOrtho(float left, float right, float bottom, float top, float clipnear, float clipfar)
{
	memset(mat, 0, sizeof(float)*16);
	
	float r_l = right - left;
	float t_b = top - bottom;
	float f_n = clipfar - clipnear;
	float tx = - (right + left) / (right - left);
	float ty = - (top + bottom) / (top - bottom);
	float tz = - (clipfar + clipnear) / (clipfar - clipnear);

	mat[0] = 2.0f/r_l;
	mat[5] = 2.0/t_b;
	mat[10] = -2.0f/f_n;

	mat[12] = tx;
	mat[13] = ty;
	mat[14] = tz;
	mat[15] = 1.0f;
}

void sgMatrix4x4::makeProjectionPersp(float arc, float aspect, float clipnear, float clipfar)
{
	memset(mat, 0, sizeof(float)*16);
	float xFac, yFac;
	yFac = tanf(arc * 3.14f/360);
	xFac = yFac*aspect;

	mat[0] = 1/xFac;
	mat[5] = 1/yFac;
	mat[10] = -(clipfar+clipnear)/(clipfar-clipnear);
	mat[11] = -1;
	mat[14] = -(2*clipfar*clipnear)/(clipfar-clipnear);
}

void sgMatrix4x4::makeInvProjectionPersp(float arc, float aspect, float clipnear, float clipfar)
{
	memset(mat, 0, sizeof(float)*16);
	float xFac, yFac;
	yFac = tanf(arc * 3.14f/360);
	xFac = yFac*aspect;
	
	//Thanks to Superku!!!!!!
	mat[0] = xFac;
	mat[5] = yFac;
	mat[11] = -(clipfar-clipnear)/(2*clipfar*clipnear);
	mat[14] = -1;
	mat[15] = (clipfar+clipnear)/(2*clipfar*clipnear);
}

void sgMatrix4x4::makeTranslate(const sgVector4 &trans)
{
	makeIdentity();	
	mat[12] = trans.x;
	mat[13] = trans.y;
	mat[14] = trans.z;
	mat[15] = trans.w;
}

void sgMatrix4x4::makeTranslate(const sgVector3 &trans)
{
	makeIdentity();
	mat[12] = trans.x;
	mat[13] = trans.y;
	mat[14] = trans.z;
	mat[15] = 1.0;
}

void sgMatrix4x4::makeScale(const sgVector4 &scal)
{
	memset(mat, 0, sizeof(float)*16);
	mat[0] = scal.x;
	mat[5] = scal.y;
	mat[10] = scal.z;
	mat[15] = scal.w;
}

void sgMatrix4x4::makeScale(const sgVector3 &scal)
{
	memset(mat, 0, sizeof(float)*16);
	mat[0] = scal.x;
	mat[5] = scal.y;
	mat[10] = scal.z;
	mat[15] = 1.0;
}

void sgMatrix4x4::makeRotate(const sgVector4 &rot)
{
	sgQuaternion quat(rot);
	set(quat.getMatrix().mat);
}

void sgMatrix4x4::makeRotate(const sgVector3 &rot)
{
	sgQuaternion quat(rot);
	set(quat.getMatrix().mat);
}

void sgMatrix4x4::makeRotate(const sgQuaternion &rot)
{
	sgQuaternion rot_(rot);
	set(rot_.getMatrix().mat);
}

void sgMatrix4x4::translate(const sgVector4 &trans)
{
	sgMatrix4x4 temp;
	temp.makeTranslate(trans);
	*this *= temp;
}

void sgMatrix4x4::translate(const sgVector3 &trans)
{
	sgMatrix4x4 temp;
	temp.makeTranslate(trans);
	*this *= temp;
}

void sgMatrix4x4::scale(const sgVector4 &scal)
{
	sgMatrix4x4 temp;
	temp.makeScale(scal);
	*this *= temp;
}

void sgMatrix4x4::scale(const sgVector3 &scal)
{
	sgMatrix4x4 temp;
	temp.makeScale(scal);
	*this *= temp;
}

void sgMatrix4x4::rotate(const sgVector4 &rot)
{
	sgMatrix4x4 temp;
	temp.makeRotate(rot);
	*this *= temp;
}

void sgMatrix4x4::rotate(const sgVector3 &rot)
{
	sgMatrix4x4 temp;
	temp.makeRotate(rot);
	*this *= temp;
}

void sgMatrix4x4::rotate(const sgQuaternion &rot)
{
	sgMatrix4x4 temp;
	temp.makeRotate(rot);
	*this *= temp;
}

void sgMatrix4x4::transpose()
{
	float temp[16];

	temp[0] = mat[0];
	temp[1] = mat[4];
	temp[2] = mat[8];
	temp[3] = mat[12];

	temp[4] = mat[1];
	temp[5] = mat[5];
	temp[6] = mat[9];
	temp[7] = mat[13];

	temp[8] = mat[2];
	temp[9] = mat[6];
	temp[10] = mat[10];
	temp[11] = mat[14];

	temp[12] = mat[3];
	temp[13] = mat[7];
	temp[14] = mat[11];
	temp[15] = mat[15];

	set(temp);
}
