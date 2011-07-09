//
//	sgTools.h
//	iSDGE
//
//	Created by Nils Daumann on 19.04.10.
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

#ifndef __SGTOOLS_H__
#define __SGTOOLS_H__

#include <cstdio>

class sgVector3;
class sgVertex;
class sgMesh;
class sgObject;

/**
 * Tools namespace. Provides functionality for different often needed tasks.
 */
namespace sgTools
{
	/**
	 * Get file length.
	 * Gets the length of a file.
	 * @param file pointer to the FILE
	 * @return the length
	 */
	long getFileLength(FILE *file);
	
	/**
	 * Ray triangle intersection.
	 * Checks if the given ray intersects the given triangle.
	 * @param from the rays starting point
	 * @param to the rays end point
	 * @param pos1 the first vertex position
	 * @param pos2 the second vertex position
	 * @param pos3 the third vertex position
	 * @param I the position of the intersection
	 * @return -1 = triangle is degenerate (a segment or point)
	 *          0 = disjoint (no intersect)
	 *          1 = intersect in unique point I1
	 *          2 = are in the same plane
	 */
	int traceRayTriangle(sgVector3 from, sgVector3 to, sgVector3 pos1, sgVector3 pos2, sgVector3 pos3, sgVertex* I);
	
	
	/**
	 * Ray mesh intersection.
	 * Checks if the given ray intersects the given mesh.
	 * @param mesh the mesh to check intersection with
	 * @param from the rays starting point
	 * @param to the rays end point
	 * @param I the position of the intersection
	 */
	void traceRayMesh(sgMesh *mesh, sgVector3 from, sgVector3 to, sgVertex* I);
	
	/**
	 * Ray object intersection.
	 * Checks if the given ray intersects the given object.
	 * @param object the object to check intersection with
	 * @param from the rays starting point
	 * @param to the rays end point
	 * @param I the position of the intersection
	 */
	void traceRayObject(sgObject *object, sgVector3 from, sgVector3 to, sgVertex* I);
}

#endif
