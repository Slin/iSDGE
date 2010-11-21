//
//	sgVector2.h
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

#ifndef __SGVECTOR2_H__
#define __SGVECTOR2_H__

/**
 * 2D vector class. Stores a vector with two elements and provides functionality for it.
 */
class sgVector2
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param x_ the x value
		 * @param y_ the y value
		 */
		sgVector2(float x_ = 0.0f, float y_ = 0.0f);
	
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param other vector to copy the values from
		 */
		sgVector2(const sgVector2 &other);
		
		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (const sgVector2 &other);
	
		/**
		 *	Not equal.
		 *	Checks if two instances do not equal each other.
		 */
		bool operator!= (const sgVector2 &other);
		
		/**
		 *	Copy.
		 *	Copy the values of one instance to the other.
		 */
		sgVector2 &operator= (const sgVector2 &other);
	
		/**
		 *	Copy.
		 *	Sets all components to the value of the scalar.
		 */
		sgVector2 &operator= (const float &other);
		
		/**
		 *	Add.
		 *	Add the two vectors component wise.
		 */
		sgVector2 &operator+= (const sgVector2 &other);
	
		/**
		 *	Substract.
		 *	Substract the two vectors component wise.
		 */
		sgVector2 &operator-= (const sgVector2 &other);
	
		/**
		 *	Multiply.
		 *	Multiply the two vectors component wise.
		 */
		sgVector2 &operator*= (const sgVector2 &other);
	
		/**
		 *	Divide.
		 *	Divide the two vectors component wise.
		 */	
		sgVector2 &operator/= (const sgVector2 &other);
		
		/**
		 *	Add.
		 *	Add the two vectors component wise.
		 */
		sgVector2 operator+ (const sgVector2 &other);
	
		/**
		 *	Substract.
		 *	Substract the two vectors component wise.
		 */
		sgVector2 operator- (const sgVector2 &other);
	
		/**
		 *	Multiply.
		 *	Multiply the two vectors component wise.
		 */
		sgVector2 operator* (const sgVector2 &other);
	
		/**
		 *	Divide.
		 *	Divide the two vectors component wise.
		 */	
		sgVector2 operator/ (const sgVector2 &other);
		
		/**
		 *	Add.
		 *	Add the scalar to each component of the vector.
		 */
		sgVector2 &operator+= (const float &other);
	
		/**
		 *	Substract.
		 *	Substract the scalar from each component of the vector.
		 */
		sgVector2 &operator-= (const float &other);
	
		/**
		 *	Multiply.
		 *	Multiply the scalar to each component of the vector.
		 */
		sgVector2 &operator*= (const float &other);
	
		/**
		 *	Divide.
		 *	Divide each component of the vector by the scalar.
		 */
		sgVector2 &operator/= (const float &other);
		
		/**
		 *	Add.
		 *	Add the scalar to each component of the vector.
		 */
		sgVector2 operator+ (const float &other);
	
		/**
		 *	Substract.
		 *	Substract the scalar from each component of the vector.
		 */
		sgVector2 operator- (const float &other);
	
		/**
		 *	Multiply.
		 *	Multiply the scalar to each component of the vector.
		 */
		sgVector2 operator* (const float &other);
	
		/**
		 *	Divide.
		 *	Divide each component of the vector by the scalar.
		 */
		sgVector2 operator/ (const float &other);
		
		/**
		 *	Length.
		 *	Get the length of the vector.
		 * @return the vectors length
		 */
		float length();
	
		/**
		 *	Squared length.
		 *	Get the squared length of the vector.
		 * @return the vectors squared length
		 */
		float squaredLength();
		
		/**
		 *	Make identity.
		 *	Sets the vectors components to 0.
		 */
		void makeIdentity();
	
		/**
		 *	Normalize.
		 *	Keeps the vectors direction, but sets its length to 1.
		 */
		void normalize();
		
		/**
		 *	Dot product.
		 * The dot product of the vector with the other one.
		 * @param other another vector
		 * @return the resulting scalar
		 */
		float dot(sgVector2 &other);
	
		/**
		 *	Distance.
		 * The distance between the vector and the other one.
		 * @param other another vector
		 * @return the distance between both vectors
		 */
		float dist(sgVector2 &other);
		
		/**
		 *	X.
		 * The vectors x component.
		 */
		float x;
		
		/**
		 *	Y.
		 * The vectors y component.
		 */
		float y;
};

#endif
