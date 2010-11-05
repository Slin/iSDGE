//
//  sgMatrix4x4.h
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

#ifndef __SGMATRIX4X4_H__
#define __SGMATRIX4X4_H__

class sgVector3;
class sgVector4;
class sgQuaternion;

class sgQuaternion;

/**
 * Matrix class. Stores a 4x4 matrix and provides functionality for it. A matrix is stored as column major!
 */
class sgMatrix4x4
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance as identity matrix.
		 */
		sgMatrix4x4();
	
		/**
		 *	Constructor.
		 *	Initializes a new instance from the given matrix.
		 * @param other the other matrix to copy the values from
		 */
		sgMatrix4x4(const sgMatrix4x4 &other);
	
		/**
		 *	Constructor.
		 *	Initializes a new instance from a matrix given by an array.
		 * @param other pointer to the first element of the matrix array (column major!)
		 */
		sgMatrix4x4(float *other);
		
		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (const sgMatrix4x4 &other);
	
		/**
		 *	Not equal.
		 *	Checks if two instances do not equal each other.
		 */
		bool operator!= (const sgMatrix4x4 &other);
	
		/**
		 *	Copy.
		 *	Copy the values of one instance to the other.
		 */
		sgMatrix4x4 &operator= (const sgMatrix4x4 &other);
	
		/**
		 *	Copy.
		 *	Copy the values from the matrix array (column major!) to the matrix.
		 */
		sgMatrix4x4 &operator= (float *other);
		
		/**
		 *	Multiply.
		 *	Multiply the two matrices.
		 */
		sgMatrix4x4 operator* (const sgMatrix4x4 &other);
	
		/**
		 *	Multiply.
		 *	Multiply the two matrices.
		 */
		sgMatrix4x4 &operator*= (const sgMatrix4x4 &other);
		
		/**
		 *	Multiply.
		 *	Transforms the vector with the matrix, by multiplying them with the matrix on the left.
		 */
		sgVector4 operator* (const sgVector4 &other);
	
		/**
		 *	Multiply.
		 *	Transforms the vector with the matrix, by multiplying them with the matrix on the left.
		 */
		sgVector3 operator* (const sgVector3 &other);
		
		/**
		 *	Set to array.
		 *	Copy the values from the given matrix array (column major!) to the matrix.
		 * @param other pointer to first element of the matrix array
		 */
		void set(float *other);
		
		/**
		 *	Make identity matrix.
		 *	Sets the matrix to the identity matrix.
		 */
		void makeIdentity();
	
		/**
		 *	Make orthogonal projection matrix.
		 *	Sets the matrix to the orthogonal projection matrix defined by the parameters.
		 * @param left the left clipping plane
		 * @param right the right clipping plane
		 * @param bottom the bottom clipping plane
		 * @param top the top clipping plane
		 * @param clipnear the near clipping plane
		 * @param clipfar the far clipping plane
		 */
		void makeProjectionOrtho(float left, float right, float bottom, float top, float clipnear, float clipfar);
	
		/**
		 *	Make perspective projection matrix.
		 *	Sets the matrix to the perspective projection matrix defined by the parameters.
		 * @param arc the horizontal opening angle of the camera
		 * @param aspect the aspect ratio
		 * @param clipnear the near clipping plane
		 * @param clipfar the far clipping plane
		 */
		void makeProjectionPersp(float arc, float aspect, float clipnear, float clipfar);
	
		/**
		 *	Make translation matrix.
		 *	Sets the matrix to the translation defined by the parameter.
		 * @param trans the translation to create a transformation matrix for
		 */
		void makeTranslate(const sgVector4 &trans);
	
		/**
		 *	Make translation matrix.
		 *	Sets the matrix to the translation defined by the parameter.
		 * @param trans the translation to create a transformation matrix for
		 */
		void makeTranslate(const sgVector3 &trans);
	
		/**
		 *	Make scale matrix.
		 *	Sets the matrix to the scaling defined by the parameter.
		 * @param scal the scale to create a transformation matrix for
		 */
		void makeScale(const sgVector4 &scal);
	
		/**
		 *	Make scale matrix.
		 *	Sets the matrix to the scaling defined by the parameter.
		 * @param scal the scale to create a transformation matrix for
		 */
		void makeScale(const sgVector3 &scal);
	
		/**
		 *	Make rotation matrix.
		 *	Sets the matrix to the rotation defined by the parameter.
		 * @param rot the rotation to create a transformation matrix for in axis angle format
		 */
		void makeRotate(const sgVector4 &rot);
	
		/**
		 *	Make rotation matrix.
		 *	Sets the matrix to the rotation defined by the parameter.
		 * @param rot the rotation to create a transformation matrix for as euler angles
		 */
		void makeRotate(const sgVector3 &rot);
	
		/**
		 *	Make rotation matrix.
		 *	Sets the matrix to the rotation defined by the parameter.
		 * @param rot the rotation to create a transformation matrix for as quaternion
		 */
		void makeRotate(const sgQuaternion &rot);
		
		/**
		 *	Translate the matrix.
		 *	Applies the translation given by the parameter to the matrix.
		 * @param trans the translation to apply
		 */
		void translate(const sgVector4 &trans);
	
		/**
		 *	Translate the matrix.
		 *	Applies the translation given by the parameter to the matrix.
		 * @param trans the translation to apply
		 */
		void translate(const sgVector3 &trans);
	
		/**
		 *	Scale the matrix.
		 *	Applies the scaling given by the parameter to the matrix.
		 * @param scal the scaling to apply
		 */
		void scale(const sgVector4 &scal);
	
		/**
		 *	Scale the matrix.
		 *	Applies the scaling given by the parameter to the matrix.
		 * @param scal the scaling to apply
		 */
		void scale(const sgVector3 &scal);
	
		/**
		 *	Rotate the matrix.
		 *	Applies the rotation given by the parameter to the matrix.
		 * @param rot the translation to apply in axis angle format
		 */
		void rotate(const sgVector4 &rot);
	
		/**
		 *	Rotate the matrix.
		 *	Applies the rotation given by the parameter to the matrix.
		 * @param rot the translation to apply as euler angles
		 */
		void rotate(const sgVector3 &rot);
	
		/**
		 *	Rotate the matrix.
		 *	Applies the rotation given by the parameter to the matrix.
		 * @param rot the translation to apply as quaternion
		 */
		void rotate(const sgQuaternion &rot);
		
		/**
		 *	Transpose.
		 *	Swap columns and rows.
		 */
		void transpose();
	
		/**
		 *	Transform.
		 *	Transforms a vector with the matrix by multiplying them with the matrix on the left.
		 * @param other the vector to transform
		 */
		sgVector4 transform(const sgVector4 &other);
	
		/**
		 *	Transform.
		 *	Transforms a vector with the matrix by multiplying them with the matrix on the left.
		 * @param other the vector to transform
		 */
		sgVector3 transform(const sgVector3 &other);
		
		/**
		 *	Matrix array.
		 *	Array of the matrix components as column major.
		 */
		float mat[16];
};

#endif
