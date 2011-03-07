//
//	sgQuaternion.cpp
//	iSDGE
//
//	Created by Nils Daumann on 25.05.10.
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

#include <cmath>
#include "sgDebug.h"
#include "sgVector3.h"
#include "sgVector4.h"
#include "sgMatrix4x4.h"

#include "sgQuaternion.h"

sgQuaternion::sgQuaternion(const sgQuaternion &quat)
{
    *this = quat;
}

sgQuaternion::sgQuaternion(float x_, float y_, float  z_, float w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

sgQuaternion::sgQuaternion(const sgVector3 &rot)
{
    makeEuler(rot);
}

sgQuaternion::sgQuaternion(const sgVector4 &rot)
{
    makeAxisAngle(rot);
}

bool sgQuaternion::operator== (sgQuaternion &other)
{
    return (x == other.x && y == other.y && z == other.z && w == other.w);
}

bool sgQuaternion::operator!= (sgQuaternion &other)
{
    return (x != other.x || y != other.y || z != other.z || w != other.w);
}

sgQuaternion &sgQuaternion::operator= (const sgQuaternion &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
	return *this;
}

sgQuaternion &sgQuaternion::operator= (const sgVector3 &other)
{
    makeEuler(other);
    return *this;
}

sgQuaternion &sgQuaternion::operator= (const sgVector4 &other)
{
    makeAxisAngle(other);
    return *this;
}

sgQuaternion &sgQuaternion::operator+=  (const sgQuaternion  &other)
{
    w += other.w;
    x += other.x;
    y += other.y;
    z += other.z;
	return *this;
}

sgQuaternion &sgQuaternion::operator-=  (const sgQuaternion  &other)
{
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;
	return *this;
}

sgQuaternion &sgQuaternion::operator*= (const sgQuaternion &other)
{
	sgQuaternion temp(*this);
	w = -temp.x * other.x-temp.y*other.y-temp.z*other.z+temp.w*other.w;
	x =  temp.x * other.w+temp.y*other.z-temp.z*other.y+temp.w*other.x;
    y = -temp.x * other.z+temp.y*other.w+temp.z*other.x+temp.w*other.y;
    z =  temp.x * other.y-temp.y*other.x+temp.z*other.w+temp.w*other.z;
    return *this;
}

sgQuaternion &sgQuaternion::operator/=  (const sgQuaternion &other)
{
	sgQuaternion temp(*this);
    w = (temp.w*other.w + temp.x*other.x + temp.y*other.y + temp.z*other.z) / (other.w*other.w + other.x*other.x + other.y*other.y + other.z*other.z);
    x = (temp.x*other.w - temp.w*other.x - temp.z*other.y + temp.y*other.z) / (other.w*other.w +  other.x*other.x + other.y*other.y + other.z*other.z);
    y = (temp.y*other.w + temp.z*other.x - temp.w*other.y - temp.x*other.z) / (other.w*other.w +  other.x*other.x + other.y*other.y + other.z*other.z);
    z = (temp.z*other.w - temp.y*other.x + temp.x*other.y - temp.w*other.z) / (other.w*other.w +  other.x*other.x + other.y*other.y + other.z*other.z);
    return *this;
}

sgQuaternion sgQuaternion::operator+ (const sgQuaternion &other)
{
    sgQuaternion res(*this);
    res += other;
    return res;
}

sgQuaternion sgQuaternion::operator- (const sgQuaternion &other)
{
    sgQuaternion res(*this);
    res -= other;
    return res;
}

sgQuaternion sgQuaternion::operator* (const sgQuaternion &other)
{
    sgQuaternion res(*this);
    res *= other;
    return res;
}
sgQuaternion sgQuaternion::operator/ (const  sgQuaternion &other)
{
    sgQuaternion res(*this);
    res /= other;
    return res;
}

sgQuaternion &sgQuaternion::operator*= (const sgVector4 &other)
{
	sgQuaternion axang(other);
	*this *= axang;
	return *this;
}

sgQuaternion &sgQuaternion::operator/= (const sgVector4 &other)
{
	sgQuaternion axang(other);
	*this /= axang;
	return *this;
}

sgQuaternion sgQuaternion::operator* (const sgVector4 &other)
{
	sgQuaternion res(*this);
	res *= other;
	return res;
}

sgQuaternion sgQuaternion::operator/ (const sgVector4 &other)
{
	sgQuaternion res(*this);
	res /= other;
	return res;
}

sgQuaternion &sgQuaternion::operator+= (const sgVector3 &other)
{
	sgVector3 euler = *this;
	euler += other;
	*this = euler;
	return *this;
}

sgQuaternion &sgQuaternion::operator-= (const sgVector3 &other)
{
	sgVector3 euler = *this;
	euler -= other;
	*this = euler;
	return *this;
}

sgQuaternion sgQuaternion::operator+ (const sgVector3 &other)
{
	sgQuaternion res(*this);
	res += other;
	return res;
}

sgQuaternion sgQuaternion::operator- (const sgVector3 &other)
{
	sgQuaternion res(*this);
	res -= other;
	return res;
}

sgQuaternion &sgQuaternion::operator*= (const float &other)
{
    x *= other;
    y *= other;
    z *= other;
    w *= other;
	return *this;
}

sgQuaternion &sgQuaternion::operator/=  (const float &other)
{
    x /= other;
    y /= other;
    z /= other;
    w /= other;
	return *this;
}

sgQuaternion sgQuaternion::operator* (const float &other)
{
    sgQuaternion res(*this);
    res *= other;
    return res;
}

sgQuaternion sgQuaternion::operator/ (const float &other)
{
    sgQuaternion res(*this);
    res /= other;
    return res;
}

void sgQuaternion::makeIdentity()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
}

void sgQuaternion::makeEuler(const sgVector3 &rot)
{
	const float fSinPitch(sin(rot.x*M_PI/360.0f));
	const float fCosPitch(cos(rot.x*M_PI/360.0f));
	const float fSinYaw(sin(rot.y*M_PI/360.0f));
	const float fCosYaw(cos(rot.y*M_PI/360.0f));
	const float fSinRoll(sin(rot.z*M_PI/360.0f));
	const float fCosRoll(cos(rot.z*M_PI/360.0f));
	const float fCosPitchCosYaw(fCosPitch*fCosYaw);
	const float fSinPitchSinYaw(fSinPitch*fSinYaw);
	x = fSinRoll * fCosPitchCosYaw     - fCosRoll * fSinPitchSinYaw;
	y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
	z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
	w = fCosRoll * fCosPitchCosYaw     + fSinRoll * fSinPitchSinYaw;
	
    normalize();
}

void sgQuaternion::makeEuler(const sgVector4 &rot)
{
    makeEuler(sgVector3(rot.x, rot.y, rot.z));
}

void sgQuaternion::makeAxisAngle(const sgVector3 &axis, float ang)
{
    const float halfang = ang*M_PI/360.0f;
    const float fsin = sin(halfang);
    w = cos(halfang);
    x = fsin*axis.x;
    y = fsin*axis.y;
    z = fsin*axis.z;
    
    normalize();
}

void sgQuaternion::makeAxisAngle(const sgVector4 &axis)
{
    makeAxisAngle(sgVector3(axis.x, axis.y, axis.z), axis.w);
}

void sgQuaternion::makeLookAt(sgVector3 dir, sgVector3 up)
{
	//Setup basis vectors describing the rotation given the input vector
	dir.normalize();
	sgVector3 right = up.cross(dir);    // The perpendicular vector to Up and Direction
	right.normalize();
	up = dir.cross(right);            // The actual up vector given the direction and the right vector
	up.normalize();
	
/*	w = sqrt(fmax(0.0, 1.0+right.x+up.y+dir.z))/2.0;
	x = sqrt(fmax(0.0, 1.0+right.x-up.y-dir.z))/2.0;
	y = sqrt(fmax(0.0, 1.0-right.x+up.y-dir.z))/2.0;
	z = sqrt(fmax(0.0, 1.0-right.x-up.y+dir.z))/2.0;
	x =	copysign(x, up.z-dir.y);
	y =	copysign(y, dir.x-right.z);
	z =	copysign(z, right.y-up.x);*/
	
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".
	// Implementation taken from Ogre3D.

	float kRot[3][3];
	kRot[0][0] = right.x;
	kRot[1][0] = right.y;
	kRot[2][0] = right.z;
	kRot[0][1] = up.x;
	kRot[1][1] = up.y;
	kRot[2][1] = up.z;
	kRot[0][2] = dir.x;
	kRot[1][2] = dir.y;
	kRot[2][2] = dir.z;
	
	float fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
	float fRoot;

	if ( fTrace > 0.0 )
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrt(fTrace + 1.0f);  // 2w
		w = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;  // 1/(4w)
		x = (kRot[2][1]-kRot[1][2])*fRoot;
		y = (kRot[0][2]-kRot[2][0])*fRoot;
		z = (kRot[1][0]-kRot[0][1])*fRoot;
	}else
	{
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if ( kRot[1][1] > kRot[0][0] )
			i = 1;
		if ( kRot[2][2] > kRot[i][i] )
			i = 2;
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
		float* apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;
		w = (kRot[k][j]-kRot[j][k])*fRoot;
		*apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
		*apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
	}
	
	
/*	float tx = up.z-dir.y;
	float ty = dir.x-right.z;
	float tz = right.y-up.x;
	sgLog("%f %f %f, %f %f %f", tx, ty, tz, x, y, z);*/
	
	normalize();
}

void sgQuaternion::makeLerpS(const sgQuaternion &quat1, const sgQuaternion &quat2, float fac)
{
	sgQuaternion q1(quat1);
	sgQuaternion q2(quat2);
	
    float angle = q1.dot(q2);
    if(angle < 0.0f)
    {
        q1 *= -1.0f;
        angle *= -1.0f;
    }
    
    float scale;
    float invscale;
    
    if((angle+1.0f) > 0.05f)
    {
        if((1.0f-angle) >= 0.05f) //spherical interpolation
        {
            const float theta = acos(angle);
            const float invsintheta = 1.0f/sin(theta);
            scale = sin(theta*(1.0f-fac))*invsintheta;
            invscale = sin(theta*fac)*invsintheta;
        }else //linear interploation
        {
            scale = 1.0f-fac;
            invscale = fac;
        }
    }else
    {
        q2 = sgQuaternion(-q1.y, q1.x, -q1.w, q1.z);
        scale = sin(M_PI*(0.5f-fac));
        invscale = sin(M_PI*fac);
    }
    
    *this = (q1*scale)+(q2*invscale);
}

void sgQuaternion::makeLerpN(const sgQuaternion &quat1, const sgQuaternion &quat2, float fac)
{
	sgQuaternion q1(quat1);
	sgQuaternion q2(quat2);
    *this = (q2*fac)+(q1*(1.0-fac));
}

void sgQuaternion::makeLerpS(const sgQuaternion &other, float fac)
{
    makeLerpS(*this, other, fac);
}

void sgQuaternion::makeLerpN(const sgQuaternion &other, float fac)
{
    makeLerpN(*this, other, fac);
}

sgQuaternion sgQuaternion::lerpS(sgQuaternion &other, float fac)
{
    sgQuaternion res(*this);
    res.makeLerpS(other, fac);
    return res;
}

sgQuaternion sgQuaternion::lerpN(sgQuaternion &other, float fac)
{
    sgQuaternion res(*this);
    res.makeLerpN(other, fac);
    return res;
}

void sgQuaternion::normalize()
{
    float len = length();
    if(len != 0)
    {
        float fac = 1/len;
        w *= fac;
        x *= fac;
        y *= fac;
        z *= fac;   
    }
}

void sgQuaternion::conjugate()
{
    x = -x;
    y = -y;
    z = -z;
}

float sgQuaternion::length()
{
    return sqrt(x*x+y*y+z*z+w*w);
}

float sgQuaternion::dot(const sgQuaternion &other)
{
    return x*other.x+y*other.y+z*other.z+w*other.w;
}

sgVector3 sgQuaternion::rotate(const sgVector3 &vec)
{
	return getMatrix().transform(vec);
}

sgVector4 sgQuaternion::rotate(const sgVector4 &vec)
{
	return getMatrix().transform(vec);
}

sgMatrix4x4 sgQuaternion::getMatrix()
{
	sgMatrix4x4 res;
	
	res.mat[0] = 1.0f - 2.0f * (y * y + z * z);
	res.mat[4] = 2.0f * (x * y - z * w);
	res.mat[8] = 2.0f * (x * z + y * w);
	res.mat[1] = 2.0f * (x * y + z * w);
	res.mat[5] = 1.0f - 2.0f * (x * x + z * z);
	res.mat[9] = 2.0f * (y * z - x * w);
	res.mat[2] = 2.0f * (x * z - y * w);
	res.mat[6] = 2.0f * (y * z + x * w);
	res.mat[10] = 1.0f - 2.0f * (x * x + y * y);
	
	return res;
}

sgVector3 sgQuaternion::getEuler()
{
	sgVector3 res;
	const float sqx = x*x;
	const float sqy = y*y;
	const float sqz = z*z;

	float clamped = 2.0f*(x*y+z*w);
	if(clamped > 0.99999f)
	{
		res.x = 2.0f*atan2(x, w)*180/M_PI;
		res.y = 90.0f;
		res.z = 0.0f;
		return res;
	}
	if(clamped < -0.99999f)
	{
		res.x = -2.0f*atan2(x, w)*180/M_PI;
		res.y = -90.0f;
		res.z = 0.0f;
		return res;
	}

	res.x = (float)(atan2(2.0*(y*w-x*z), 1.0-2*(sqy+sqz)));
	res.y = asin(clamped);
	res.z = (float)(atan2(2.0*(x*w-y*z), 1.0-2*(sqx+sqz)));
	res *= 180.0f/M_PI;

	return res;
}

sgVector4 sgQuaternion::getAxisAngle()
{
    sgVector4 res;
    const float scale = sqrtf(x*x + y*y + z*z);
    if(scale == 0.0f || w > 1.0f || w < -1.0f)
    {
        res.w = 0.0f;
        res.x = 0.0f;
        res.y = 1.0f;
        res.z = 0.0f;
    }else
    {
        const float invscale = 1.0f/scale;
        res.w = (360/M_PI)*acos(w);
        res.x = x*invscale;
        res.y = y*invscale;
        res.z = z*invscale;
    }
    return res;
}