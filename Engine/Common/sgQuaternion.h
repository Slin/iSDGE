//
//	sgQuaternion.h
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

#ifndef __SGQUATERNION_H__
#define __SGQUATERNION_H__

#include "sgVector3.h"
class sgVector4;
class sgMatrix4x4;

/**
 * Quaternion class. Stores a quaternion and provides functionality for it. All angles are in degrees!
 */
class sgQuaternion
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance from the other quaternion.
		 * @param quat the other quaternion to copy the values from
		 */
		sgQuaternion(const sgQuaternion &quat);
	
		/**
		 *	Constructor.
		 *	Initializes a new instance from the given values.
		 * @param x_ the quaternions x value
		 * @param y_ the quaternions y value
		 * @param z_ the quaternions z value
		 * @param w_ the quaternions w value
		 */
		sgQuaternion(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 1.0f);
	
		/**
		 *	Constructor.
		 *	Initializes a new instance from the vector.
		 * @param rot the rotation as euler angles to initialize the quaternion with
		 */
		sgQuaternion(const sgVector3 &rot);
	
		/**
		 *	Constructor.
		 *	Initializes a new instance from the vector.
		 * @param rot the rotation in axis angle format to initialize the quaternion with
		 */
		sgQuaternion(const sgVector4 &rot);

		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (sgQuaternion &other);
	
		/**
		 *	Not equal.
		 *	Checks if two instances do not equal each other.
		 */
		bool operator!= (sgQuaternion &other);

		/**
		 *	Copy.
		 *	Copy the values of one instance to the other.
		 */
		sgQuaternion &operator= (const sgQuaternion &other);
	
		/**
		 *	Copy.
		 *	Set the quaternion to the representation of the euler angles given by the parameter.
		 */
		sgQuaternion &operator= (const sgVector3 &other);
	
		/**
		 *	Copy.
		 *	Set the quaternion to the representation of the axis and angle given by the parameter.
		 */
		sgQuaternion &operator= (const sgVector4 &other);

		/**
		 *	Add.
		 *	Add the two quaternions.
		 */
		sgQuaternion &operator+= (const sgQuaternion &other);
	
		/**
		 *	Substract.
		 *	Substract the two quaternions.
		 */
		sgQuaternion &operator-=  (const sgQuaternion &other);
	
		/**
		 *	Multiply.
		 *	Multiply the two quaternions.
		 */
		sgQuaternion &operator*= (const sgQuaternion &other);
	
		/**
		 *	Divide.
		 *	Divide the two quaternions.
		 */
		sgQuaternion &operator/=  (const sgQuaternion &other);

		/**
		 *	Add.
		 *	Add the two quaternions.
		 */
		sgQuaternion operator+ (const sgQuaternion &other);
	
		/**
		 *	Substract.
		 *	Substract the two quaternions.
		 */
		sgQuaternion operator- (const sgQuaternion &other);
	
		/**
		 *	Multiply.
		 *	Multiply the two quaternions.
		 */
		sgQuaternion operator* (const sgQuaternion &other);
	
		/**
		 *	Divide.
		 *	Divide the two quaternions.
		 */
		sgQuaternion operator/ (const sgQuaternion &other);
	
		/**
		 *	Add.
		 *	Add the euler angles in the vector to the rotation of the quaternion.
		 */
		sgQuaternion &operator+= (const sgVector3 &other);
	
		/**
		 *	Substract.
		 *	Substract the euler angles in the vector from the rotation of the quaternion.
		 */
		sgQuaternion &operator-= (const sgVector3 &other);
	
		/**
		 *	Add.
		 *	Add the euler angles in the vector to the rotation of the quaternion.
		 */
		sgQuaternion operator+ (const sgVector3 &other);
	
		/**
		 *	Substract.
		 *	Substract the euler angles in the vector from the rotation of the quaternion.
		 */
		sgQuaternion operator- (const sgVector3 &other);

		/**
		 *	Multiply.
		 *	Multiply the quaternions with a scalar.
		 */
		sgQuaternion &operator*= (const float &other);
	
		/**
		 *	Divide.
		 *	Divide the quaternions by a scalar.
		 */
		sgQuaternion &operator/=  (const float &other);

		/**
		 *	Multiply.
		 *	Multiply the quaternions with a scalar.
		 */
		sgQuaternion operator* (const float &other);
	
		/**
		 *	Divide.
		 *	Divide the quaternions by a scalar.
		 */
		sgQuaternion operator/ (const float &other);

		/**
		 *	Make identity.
		 *	Set the quaternion to the one representing no rotation.
		 */
		void makeIdentity();

		/**
		 *	Make euler.
		 *	Set the quaternion to the representation of the given euler angles.
		 * @param rot vector containing the rotation as euler angles
		 */
		void makeEuler(const sgVector3 &rot);
	
		/**
		 *	Make euler.
		 *	Set the quaternion to the representation of the given euler angles, by ignoring the vectors w component.
		 * @param rot vector containing the rotation as euler angles
		 */
		void makeEuler(const sgVector4 &rot);

		/**
		 *	Make axis angle.
		 *	Set the quaternion to the representation of the given rotation around the given axis.
		 * @param axis vector containing the orientation of the axis to rotate around
		 * @param ang rotation angle in degrees
		 */
		void makeAxisAngle(const sgVector3 &axis, float ang);
	
		/**
		 *	Make axis angle.
		 *	Set the quaternion to the representation of the given rotation around the given axis.
		 * @param axis vector containing the orientation of the axis to rotate around and the angle as fourth parameter
		 */
		void makeAxisAngle(const sgVector4 &axis);
		
		/**
		 *	Make look at.
		 *	Set the quaternion to the representation of the given look at vector.
		 * @param dir look at direction vector
		 * @param up up vector
		 */
		void makeLookAt(sgVector3 dir, sgVector3 up = sgVector3(0.0, 1.0, 0.0));

		/**
		 *	Make spherical interpolation.
		 *	Set the quaternion to the result of the spherical interpolation given by the parameters.
		 * @param quat1 the first quaternion to interpolate from
		 * @param quat2 the second quaternion to interpolate from
		 * @param fac the interpolation factor to interpolate with ranging from 0 to 1
		 */
		void makeLerpS(const sgQuaternion &quat1, const sgQuaternion &quat2, float fac);
	
		/**
		 *	Make linear interpolation.
		 *	Set the quaternion to the result of the linear interpolation given by the parameters.
		 * @param quat1 the first quaternion to interpolate from
		 * @param quat2 the second quaternion to interpolate from
		 * @param fac the interpolation factor to interpolate with, ranging from 0 to 1
		 */
		void makeLerpN(const sgQuaternion &quat1, const sgQuaternion &quat2, float fac);
	
		/**
		 *	Make spherical interpolation.
		 *	Set the quaternion to the result of the spherical interpolation given by the instance and the parameters.
		 * @param other the second quaternion to interpolate from
		 * @param fac the interpolation factor to interpolate with, ranging from 0 to 1
		 */
		void makeLerpS(const sgQuaternion &other, float fac);
	
		/**
		 *	Make linear interpolation.
		 *	Set the quaternion to the result of the linear interpolation given by the instance and the parameters.
		 * @param other the second quaternion to interpolate from
		 * @param fac the interpolation factor to interpolate with, ranging from 0 to 1
		 */
		void makeLerpN(const sgQuaternion &other, float fac);

		/**
		 *	Spherical interpolation.
		 *	Interpolate spherical between the instance and the first parameter.
		 * @param other the second quaternion to interpolate from
		 * @param fac the interpolation factor to interpolate with, ranging from 0 to 1
		 * @return resulting quaternion from the interpolation
		 */
		sgQuaternion lerpS(sgQuaternion &other, float fac);
	
		/**
		 *	Linear interpolation.
		 *	Interpolate linear between the instance and the first parameter.
		 * @param other the second quaternion to interpolate from
		 * @param fac the interpolation factor to interpolate with, ranging from 0 to 1
		 * @return resulting quaternion from the interpolation
		 */
		sgQuaternion lerpN(sgQuaternion &other, float fac);

		/**
		 *	Normalize.
		 *	Normalizes the quaternion.
		 */
		void normalize();
	
		/**
		 *	Conjugate.
		 *	Inverts the quaternions imaginary parts.
		 */
		void conjugate();

		/**
		 *	Length.
		 *	Get the length of the quaternion.
		 * @return the quaternions length
		 */
		float length();
	
		/**
		 *	Dot product.
		 * The dot product of the quaternion with the other one.
		 * @param other another quaternion
		 * @return the resulting scalar
		 */
		float dot(const sgQuaternion &other);
	
		/**
		 *	Rotate vector.
		 * Applies the rotation represented by the quaternion to a vector.
		 * @param vec the vector to rotate
		 * @return the rotated vector
		 */
		sgVector3 rotate(const sgVector3 &vec);
	
		/**
		 *	Rotate vector.
		 * Applies the rotation represented by the quaternion to a vector.
		 * @param vec the vector to rotate
		 * @return the rotated vector
		 */
		sgVector4 rotate(const sgVector4 &vec);

		/**
		 *	Get matrix.
		 * Returns a rotation matrix representing the same rotation as the quaternion.
		 * @return the rotation matrix
		 */
		sgMatrix4x4 getMatrix();
	
		/**
		 *	Get euler.
		 * Returns the quaternions rotation as euler angles.
		 * @return the rotation as euler angles
		 */
		sgVector3 getEuler();
	
		/**
		 *	Get axis angle.
		 * Returns the quaternions rotation in axis angle format.
		 * @return the rotation as axis and angle
		 */
		sgVector4 getAxisAngle();

		/**
		 *	X.
		 * The quaternions x component.
		 */
		float x;
		
		/**
		 *	Y.
		 * The quaternions y component.
		 */
		float y;
		
		/**
		 *	Z.
		 * The quaternions z component.
		 */
		float z;
		
		/**
		 *	W.
		 * The quaternions w component. The real part.
		 */
		float w;
};

#endif
