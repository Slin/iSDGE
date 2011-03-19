//
//	sgVertex.h
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

#ifndef __SGVERTEX_H__
#define __SGVERTEX_H__

#include "sgVector4.h"
#include "sgVector3.h"
#include "sgVector2.h"

enum VERTEXFORMAT
{
	BASIC = 0,
	SECONDUV = 1,
	COLOR = 2,
	SECONDUVCOLOR = 3,
	TANGENT = 4,
	TANGENTSECONDUV = 5,
	TANGENTCOLOR = 6,
	TANGENTSECONDUVCOLOR = 7
};

enum VERTEXSIZE
{
	SZ_BASIC = 32,
	SZ_SECONDUV = 40,
	SZ_COLOR = 48,
	SZ_SECONDUVCOLOR = 56,
	SZ_TANGENT = 48,
	SZ_TANGENTSECONDUV = 56,
	SZ_TANGENTCOLOR = 64,
	SZ_TANGENTSECONDUVCOLOR = 72
};

/**
 * Vertex base class. This is the base vertex format.
 */
class sgVertex
{
	public:
		sgVertex &operator= (const sgVertex &other)
		{
			position = other.position;
			normal = 0;//other.normal;
			uv = other.uv;
			return *this;
		}
	
		sgVector3 position;
		sgVector3 normal;
		sgVector2 uv;
};

class sgVertexUV
{
	public:
		sgVector3 position;
		sgVector3 normal;
		sgVector2 uv;
		sgVector2 uv2;
};

class sgVertexCol
{
	public:
		sgVector3 position;
		sgVector3 normal;
		sgVector2 uv;
		sgVector4 color;
};

class sgVertexUVCol
{
	public:
		sgVector3 position;
		sgVector3 normal;
		sgVector2 uv;
		sgVector2 uv2;
		sgVector4 color;
};

class sgVertexTan
{
public:
	sgVector3 position;
	sgVector3 normal;
	sgVector2 uv;
	sgVector4 tangent;
};

class sgVertexTanUV
{
public:
	sgVector3 position;
	sgVector3 normal;
	sgVector2 uv;
	sgVector2 uv2;
	sgVector4 tangent;
};

class sgVertexTanCol
{
public:
	sgVector3 position;
	sgVector3 normal;
	sgVector2 uv;
	sgVector4 tangent;
	sgVector4 color;
};

class sgVertexTanUVCol
{
public:
	sgVector3 position;
	sgVector3 normal;
	sgVector2 uv;
	sgVector2 uv2;
	sgVector4 tangent;
	sgVector4 color;
};

#endif
