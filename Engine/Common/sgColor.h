//
//	sgColor.h
//	iSDGE
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

#ifndef __SGCOLOR_H__
#define __SGCOLOR_H__

/**
 * Color class. Stores a color without alpha channel with values between 0 and 255 and provides functionality for it.
 */
class sgColor
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param r_ the r value
		 * @param g_ the g value
		 * @param b_ the b value
		 */
		sgColor(unsigned char r_ = 0, unsigned char g_ = 0, unsigned char b_ = 0);
	
		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (const sgColor &other);
	
		/**
		 *	Red.
		 * The colors red component.
		 */
		unsigned char r;
	
		/**
		 *	Green.
		 * The colors green component.
		 */
		unsigned char g;
	
		/**
		 *	Blue.
		 * The colors blue component.
		 */
		unsigned char b;
};

/**
 * ColorA class. Stores a color with alpha channel with values between 0 and 255 and provides functionality for it.
 */
class sgColorA : public sgColor
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param r_ the r value
		 * @param g_ the g value
		 * @param b_ the b value
		 * @param a_ the a value
		 */
		sgColorA(unsigned char r_ = 0, unsigned char g_ = 0, unsigned char b_ = 0, unsigned char a_ = 255);
	
		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (const sgColorA &other);
	
		/**
		 *	Alpha.
		 * The colors alpha component.
		 */
		unsigned char a;
};

/**
 * Floating point color class. Stores a color without alpha channel with values between 0 and 1 and provides functionality for it.
 */
class sgFColor
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param r_ the r value
		 * @param g_ the g value
		 * @param b_ the b value
		 */
		sgFColor(float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f);
	
		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (const sgFColor &other);
	
		/**
		 *	Red.
		 * The colors red component.
		 */
		float r;
	
		/**
		 *	Green.
		 * The colors green component.
		 */
		float g;
	
		/**
		 *	Blue.
		 * The colors blue component.
		 */
		float b;
};

/**
 * Floating point colorA class. Stores a color with alpha channel with values between 0 and 1 and provides functionality for it.
 */
class sgFColorA : public sgFColor
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param r_ the r value
		 * @param g_ the g value
		 * @param b_ the b value
		 * @param a_ the a value
		 */
		sgFColorA(float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f, float a_ = 1.0f);
	
		/**
		 *	Equal.
		 *	Checks if two instances equal each other.
		 */
		bool operator== (const sgFColorA &other);
	
		/**
		 *	Alpha.
		 * The colors alpha component.
		 */
		float a;
};

#endif
