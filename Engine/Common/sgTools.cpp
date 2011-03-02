//
//	sgTools.cpp
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

#include "sgTools.h"

#include <cmath>
#include <vector>
#include "sgVector3.h"
#include "sgVertex.h"
#include "sgMesh.h"

namespace sgTools
{
	long getFileLength(FILE *file)
	{
		long length = 0;
		
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		
		return length;
	}
	
	// Copyright 2001, softSurfer (www.softsurfer.com)
	// This code may be freely used and modified for any purpose
	// providing that this copyright notice is included with it.
	// SoftSurfer makes no warranty for this code, and cannot be held
	// liable for any real or imagined damage resulting from its use.
	// Users of this code must verify correctness for their application.
	
	// traceRayTriangle(): intersect a ray with a 3D triangle
	//    Input:  a ray R, and a triangle T
	//    Output: *I = intersection point (when it exists)
	//    Return: -1 = triangle is degenerate (a segment or point)
	//             0 = disjoint (no intersect)
	//             1 = intersect in unique point I1
	//             2 = are in the same plane
	int traceRayTriangle(sgVector3 from, sgVector3 to, sgVector3 pos1, sgVector3 pos2, sgVector3 pos3, sgVertex* I)
	{
		sgVector3 u, v, n;             // triangle vectors
		sgVector3 dir, w0, w;          // ray vectors
		float r, a, b;             // params to calc ray-plane intersect
		
		// get triangle edge vectors and plane normal
		u = pos2 - pos1;
		v = pos3 - pos1;
		n = u.cross(v);             // cross product
		if(n.x == 0 && n.y == 0 && n.z == 0)            // triangle is degenerate
			return -1;                 // do not deal with this case
		
		dir = to-from;             // ray direction vector
		w0 = from - pos1;
		a = -n.dot(w0);
		b = n.dot(dir);
		if(fabs(b) < 0.00000001) // ray is parallel to triangle plane
		{
			if (a == 0)                // ray lies in triangle plane
				return 2;
			else return 0;             // ray disjoint from plane
		}
		
		// get intersect point of ray with triangle plane
		r = a/b;
		if(r < 0.0)                   // ray goes away from triangle
			return 0;                  // => no intersect
		// for a segment, also test if (r > 1.0) => no intersect
		
		I->position = from + dir*r;           // intersect point of ray and plane
		I->normal = n;
		I->normal.normalize();
		
		// is I inside T?
		float uu, uv, vv, wu, wv, D;
		uu = u.dot(u);
		uv = u.dot(v);
		vv = v.dot(v);
		w = I->position - pos1;
		wu = w.dot(u);
		wv = w.dot(v);
		D = uv * uv - uu * vv;
		
		// get and test parametric coords
		float s, t;
		s = (uv * wv - vv * wu) / D;
		if (s < 0.0 || s > 1.0)        // I is outside T
			return 0;
		t = (uv * wu - uu * wv) / D;
		if (t < 0.0 || (s + t) > 1.0)  // I is outside T
			return 0;
		
		return 1;                      // I is in T
	}
	
	void traceRayMesh(sgMesh *mesh, sgVector3 from, sgVector3 to, sgVertex* I)
	{
		sgVertex point;
		std::vector<sgVertex> points;
		if(mesh->vtxform == BASIC)
		{
			for(int i = 0; i < mesh->indexnum; i += 3)
			{
				if(traceRayTriangle(from, to, mesh->vertices[mesh->indices[i+0]].position, mesh->vertices[mesh->indices[i+1]].position, mesh->vertices[mesh->indices[i+2]].position, &point) == 1)
					points.push_back(point);
			}
		}else if(mesh->vtxform == SECONDUV)
		{
			for(int i = 0; i < mesh->indexnum; i += 3)
			{
				if(traceRayTriangle(from, to, ((sgVertexUV*)mesh->vertices)[mesh->indices[i+0]].position, ((sgVertexUV*)mesh->vertices)[mesh->indices[i+1]].position, ((sgVertexUV*)mesh->vertices)[mesh->indices[i+2]].position, &point) == 1)
					points.push_back(point);
			}
		}else if(mesh->vtxform == COLOR)
		{
			for(int i = 0; i < mesh->indexnum; i += 3)
			{
				if(traceRayTriangle(from, to, ((sgVertexCol*)mesh->vertices)[mesh->indices[i+0]].position, ((sgVertexCol*)mesh->vertices)[mesh->indices[i+1]].position, ((sgVertexCol*)mesh->vertices)[mesh->indices[i+2]].position, &point) == 1)
					points.push_back(point);
			}
		}
		
		float dist;
		float ndist = from.dist(to);
		for(int i = 0; i < points.size(); i++)
		{
			dist = from.dist(points[i].position);
			if(dist < ndist)
			{
				ndist = dist;
				*I = points[i];
				I->position = points[i].position;
				I->normal = points[i].normal;
			}
		}
	}
}
