//
//	sgMesh.cpp
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

#include "sgMesh.h"

#include <cmath>
#include <cstring>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "sgVector3.h"
#include "sgDebug.h"

sgMesh::sgMesh()
{
	vbo = -1;
	ivbo = -1;
	vbo0 = -1;
	ivbo0 = -1;
	vbo1 = -1;
	ivbo1 = -1;
	vbotoggle = false;
	dynamic = false;
	
	vertexnum = 0;
	indexnum = 0;
	vertices = NULL;
	indices = NULL;
	
	culled = false;
}

sgMesh::~sgMesh()
{
	if(vbo0 != -1)
		glDeleteBuffers(1, &vbo0);
	if(ivbo0 != -1)
		glDeleteBuffers(1, &ivbo0);
	if(vbo1 != -1)
		glDeleteBuffers(1, &vbo1);
	if(ivbo1 != -1)
		glDeleteBuffers(1, &ivbo1);
	
	delete[] vertices;
	delete[] indices;
}

void sgMesh::generateVBO(bool dyn)
{
	dynamic = dyn;
	
	if(vbo0 != -1)
		glDeleteBuffers(1, &vbo0);
	if(ivbo0 != -1)
		glDeleteBuffers(1, &ivbo0);
	if(vbo1 != -1)
		glDeleteBuffers(1, &vbo1);
	if(ivbo1 != -1)
		glDeleteBuffers(1, &ivbo1);
	
	vbo = -1;
	ivbo = -1;
	vbo0 = -1;
	ivbo0 = -1;
	vbo1 = -1;
	ivbo1 = -1;
	vbotoggle = false;
	
	if(!dynamic)
	{
		glGenBuffers(1, &vbo0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo0);
		glBufferData(GL_ARRAY_BUFFER, vertexnum*sizeof(sgVertex), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &ivbo0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum*sizeof(unsigned short), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		vbo = vbo0;
		ivbo = ivbo0;
	}else
	{
		glGenBuffers(1, &vbo0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo0);
		glBufferData(GL_ARRAY_BUFFER, vertexnum*sizeof(sgVertex), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &ivbo0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum*sizeof(unsigned short), indices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferData(GL_ARRAY_BUFFER, vertexnum*sizeof(sgVertex), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &ivbo1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum*sizeof(unsigned short), indices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		vbo = vbo0;
		ivbo = ivbo0;
	}
}

void sgMesh::updateVBO()
{
	if(!dynamic)
		return;

	vbotoggle = !vbotoggle;
	
	if(vbotoggle)
	{
		vbo = vbo1;
		ivbo = ivbo1;
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo0);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexnum*sizeof(sgVertex), vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, ivbo0);
		glBufferSubData(GL_ARRAY_BUFFER, 0, indexnum*sizeof(unsigned short), indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}else
	{
		vbo = vbo0;
		ivbo = ivbo0;
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexnum*sizeof(sgVertex), vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, ivbo1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, indexnum*sizeof(unsigned short), indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void sgMesh::calculateNormals()
{
	int i;
	int n;
	sgVertex *tempverts = new sgVertex[vertexnum];
	
	//Reset all normals
	for(i = 0; i < vertexnum; i++)
	{
		vertices[i].normal.x = 0;
		vertices[i].normal.y = 0;
		vertices[i].normal.z = 0;
	}
	memcpy(tempverts, vertices, sizeof(sgVertex)*vertexnum);
	
	//Calculate the normal of each vertex
	for(i = 0; i < indexnum; i += 3)
	{
		calculateFaceNormal(&vertices[indices[i]], &vertices[indices[i+2]], &vertices[indices[i+1]]);
	}
	
	//Normalize all normals
	for(i = 0; i < vertexnum; i++)
	{
		vertices[i].normal.normalize();
	}
	
	//Interpolate normals between doublicated vertices
	for(i = 0; i < vertexnum; i++)
	{
		for(n = 0; n < vertexnum; n++)
		{
			if(vertices[i].position.dist(vertices[n].position) < 0.000001)
				tempverts[i].normal += vertices[n].normal;
		}
	}
	delete[] vertices;
	vertices = tempverts;
	
	//Normalize all normals
	for(i = 0; i < vertexnum; i++)
	{
		vertices[i].normal.normalize();
	}
}

void sgMesh::calculateFaceNormal(sgVertex *vertex, sgVertex *neighbour_a, sgVertex *neighbour_b)
{
	float a[3];
	float b[3];
	float normal[3];
	
	a[0] = vertex->position.x - neighbour_a->position.x;
	a[1] = vertex->position.y - neighbour_a->position.y;
	a[2] = vertex->position.z - neighbour_a->position.z;
	
	b[0] = neighbour_a->position.x - neighbour_b->position.x;
	b[1] = neighbour_a->position.y - neighbour_b->position.y;
	b[2] = neighbour_a->position.z - neighbour_b->position.z;
	
	normal[0] = (a[1] * b[2]) - (a[2] * b[1]);
	normal[1] = (a[2] * b[0]) - (a[0] * b[2]);
	normal[2] = (a[0] * b[1]) - (a[1] * b[0]);
	
	float len = normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2];
	if(len == 0.0f) len = 1.0;
	len = sqrt(len);
	
	normal[0] /= -len;
	normal[1] /= -len;
	normal[2] /= -len;
	
	vertex->normal.x = normal[0];
	vertex->normal.y = normal[1];
	vertex->normal.z = normal[2];
	
	neighbour_a->normal.x = normal[0];
	neighbour_a->normal.y = normal[1];
	neighbour_a->normal.z = normal[2];
	neighbour_b->normal.x = normal[0];
	neighbour_b->normal.y = normal[1];
	neighbour_b->normal.z = normal[2];
}

void sgMesh::invertTexCoordsX()
{
	for(int i = 0; i < vertexnum; i++)
	{
		vertices[i].uv.x = 1.0f-vertices[i].uv.x;
	}
}

void sgMesh::invertTexCoordsY()
{
	for(int i = 0; i < vertexnum; i++)
	{
		vertices[i].uv.y = 1.0f-vertices[i].uv.y;
	}
}

void sgMesh::calcCullSphere()
{
	//Find the center
	sgVector3 vmax(-1000000000.0, -1000000000.0, -1000000000.0);
	sgVector3 vmin(1000000000.0, 1000000000.0, 1000000000.0);
	sgVector3 center;
	for(int i = 0; i < vertexnum; i++)
	{
		if(vertices[i].position.x > vmax.x)
			vmax.x = vertices[i].position.x;
		if(vertices[i].position.y > vmax.y)
			vmax.y = vertices[i].position.y;
		if(vertices[i].position.z > vmax.z)
			vmax.z = vertices[i].position.z;
		
		if(vertices[i].position.x < vmin.x)
			vmin.x = vertices[i].position.x;
		if(vertices[i].position.y < vmin.y)
			vmin.y = vertices[i].position.y;
		if(vertices[i].position.z < vmin.z)
			vmin.z = vertices[i].position.z;
	}
	center = vmin+vmax;
	center *= 0.5;
	
	//Find the radius
	float radius = 0;
	float temp;
	sgVector3 diff;
	for(int i = 0; i < vertexnum; i++)
	{
		diff = vertices[i].position-center;
		temp = diff.length();
		if(radius < temp)
			radius = temp;
		
	}
	cullsphere = sgVector4(center.x, center.y, center.z, radius);
}
