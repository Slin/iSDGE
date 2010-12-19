//
//	sgPlane.h
//	iSDGE
//
//	Created by Nils Daumann on 08.12.10.
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

#ifndef __SGPLANE_H__
#define __SGPLANE_H__

#include "sgVector3.h"

/**
 * Plane class. Stores a plane in 3d space and provides functionality for it.
 */
class sgPlane
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param pos a position within the plane
		 * @param norm the planes normal
		 */
		sgPlane(sgVector3 pos = sgVector3(), sgVector3 norm = sgVector3(0.0, 1.0, 0.0));
	
		/**
		 *	Constructor.
		 *	Initializes a new instance.
		 * @param pos1 a position within the plane
		 * @param pos2 a second position within the plane
		 * @param pos3 a third position within the plane
		 */
		sgPlane(sgVector3 pos1, sgVector3 pos2, sgVector3 pos3);
	
		/**
		 *	Set plane.
		 *	Sets new values for the plane.
		 * @param pos a position within the plane
		 * @param norm the planes normal
		 */
		void setPlane(sgVector3 pos = sgVector3(), sgVector3 norm = sgVector3(0.0, 1.0, 0.0));
		
		/**
		 *	Set plane.
		 *	Sets new values for the plane.
		 * @param pos1 a position within the plane
		 * @param pos2 a second position within the plane
		 * @param pos3 a third position within the plane
		 */
		void setPlane(sgVector3 pos1, sgVector3 pos2, sgVector3 pos3);
		
		/**
		 *	Calculate d.
		 *	Calculates d used for HNF.
		 */
		void calcD();
	
		/**
		 *	Distance.
		 *	Calculates the distance of the plane to the given position. Call calcD before, if you changed the planes position or normal directly.
		 */
		float dist(sgVector3 pos);
	
		/**
		 *	Position.
		 *	A point within the plane.
		 */
		sgVector3 position;
	
		/**
		 *	Normal.
		 *	The planes normal.
		 */
		sgVector3 normal;
	
		/**
		 *	d.
		 *	The distance to the coordinatesystems center, needed for the dist function.
		 */
		float d;
};

#endif
