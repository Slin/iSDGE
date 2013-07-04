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
#include <cstdlib>
#if defined __IOS__
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#else
	#define GLEW_STATIC
	#include <GL/glew.h>
	#include <GL/glfw.h>
#endif
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

	bonenum = 0;
	boneindices = NULL;
	bonematrices = NULL;

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

	if(vertices != NULL)
		free(vertices);
	if(indices != NULL)
		delete[] indices;
	if(bonematrices != NULL)
		delete[] bonematrices;
	if(boneindices != NULL)
		delete[] boneindices;
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
		glBufferData(GL_ARRAY_BUFFER, vertexnum*vtxsize*sizeof(float), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ivbo0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum*indexsize, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		vbo = vbo0;
		ivbo = ivbo0;
	}else
	{
		glGenBuffers(1, &vbo0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo0);
		glBufferData(GL_ARRAY_BUFFER, vertexnum*vtxsize*sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ivbo0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum*indexsize, indices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferData(GL_ARRAY_BUFFER, vertexnum*vtxsize*sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ivbo1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexnum*indexsize, indices, GL_DYNAMIC_DRAW);
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
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexnum*vtxsize*sizeof(float), vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, ivbo0);
		glBufferSubData(GL_ARRAY_BUFFER, 0, indexnum*indexsize, indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}else
	{
		vbo = vbo0;
		ivbo = ivbo0;

		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexnum*vtxsize*sizeof(float), vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, ivbo1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, indexnum*indexsize, indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void sgMesh::calculateNormals()
{
	int i;
	int n;
	sgVertex *tempvtx;
	float *tempverts = (float*)malloc(vtxsize*vertexnum*sizeof(float));

	//Reset all normals
	for(i = 0; i < vertexnum; i++)
	{
		tempvtx = (sgVertex*)(vertices+vtxsize*i);

		tempvtx->normal.x = 0;
		tempvtx->normal.y = 0;
		tempvtx->normal.z = 0;
	}
	memcpy(tempverts, vertices, vtxsize*vertexnum*sizeof(float));

	//Calculate the normal of each vertex
	for(i = 0; i < indexnum; i += 3)
	{
		if(indexsize == 2)
			calculateFaceNormal((sgVertex*)(vertices+((unsigned short*)indices)[i]*vtxsize), (sgVertex*)(vertices+((unsigned short*)indices)[i+2]*vtxsize), (sgVertex*)(vertices+((unsigned short*)indices)[i+1]*vtxsize));
		else
			calculateFaceNormal((sgVertex*)(vertices+((unsigned long*)indices)[i]*vtxsize), (sgVertex*)(vertices+((unsigned long*)indices)[i+2]*vtxsize), (sgVertex*)(vertices+((unsigned long*)indices)[i+1]*vtxsize));
	}

	//Normalize all normals
	for(i = 0; i < vertexnum; i++)
	{
		tempvtx = (sgVertex*)(vertices+i*vtxsize);
		tempvtx->normal.normalize();
	}

	//Interpolate normals between doublicated vertices
	for(i = 0; i < vertexnum; i++)
	{
		for(n = 0; n < vertexnum; n++)
		{
			if(((sgVertex*)(vertices+i*vtxsize))->position.dist(((sgVertex*)(vertices+n*vtxsize))->position) < 0.00001 && ((sgVertex*)(vertices+i*vtxsize))->normal.dot(((sgVertex*)(vertices+n*vtxsize))->normal) > 0.8)
				((sgVertex*)(tempverts+i*vtxsize))->normal += ((sgVertex*)(vertices+n*vtxsize))->normal;
		}
	}
	free(vertices);
	vertices = tempverts;

	//Normalize all normals
	for(i = 0; i < vertexnum; i++)
	{
		tempvtx = (sgVertex*)(vertices+i*vtxsize);
		tempvtx->normal.normalize();
	}
}

void sgMesh::calculateTangents()
{
	if((vtxfeatures & sgVertex::TANGENT) == 0)
		return;

	int i;
	char tangentoffset = 8;
	if((vtxfeatures & sgVertex::UV1))
		tangentoffset += 2;
	if((vtxfeatures & sgVertex::COLOR))
		tangentoffset += 4;

	sgVector3 tempnormal;
	sgVector3 temptangent;
	sgVector3 *bitangents = new sgVector3[vertexnum];

	//Reset all tangents
	for(i = 0; i < vertexnum; i++)
	{
		vertices[i*vtxsize+tangentoffset] = 0;
		vertices[i*vtxsize+tangentoffset+1] = 0;
		vertices[i*vtxsize+tangentoffset+2] = 0;
		vertices[i*vtxsize+tangentoffset+3] = 0;
	}

	//Calculate the tangent of each vertex
	for(i = 0; i < indexnum; i += 3)
	{
		if(indexsize == 2)
			calculateFaceTangent((sgVertex*)(vertices+((unsigned short*)indices)[i]*vtxsize), (sgVertex*)(vertices+((unsigned short*)indices)[i+2]*vtxsize), (sgVertex*)(vertices+((unsigned short*)indices)[i+1]*vtxsize), (sgVector3*)(vertices+((unsigned short*)indices)[i]*vtxsize+tangentoffset), (sgVector3*)(vertices+((unsigned short*)indices)[i+2]*vtxsize+tangentoffset), (sgVector3*)(vertices+((unsigned short*)indices)[i+1]*vtxsize+tangentoffset), &bitangents[((unsigned short*)indices)[i]], &bitangents[((unsigned short*)indices)[i+2]], &bitangents[((unsigned short*)indices)[i+1]]);
		else
			calculateFaceTangent((sgVertex*)(vertices+((unsigned long*)indices)[i]*vtxsize), (sgVertex*)(vertices+((unsigned long*)indices)[i+2]*vtxsize), (sgVertex*)(vertices+((unsigned long*)indices)[i+1]*vtxsize), (sgVector3*)(vertices+((unsigned long*)indices)[i]*vtxsize+tangentoffset), (sgVector3*)(vertices+((unsigned long*)indices)[i+2]*vtxsize+tangentoffset), (sgVector3*)(vertices+((unsigned long*)indices)[i+1]*vtxsize+tangentoffset), &bitangents[((unsigned long*)indices)[i]], &bitangents[((unsigned long*)indices)[i+2]], &bitangents[((unsigned long*)indices)[i+1]]);
	}

	//Normalize all tangents
	for(i = 0; i < vertexnum; i++)
	{
		tempnormal = sgVector3(vertices[i*vtxsize+3]);
		temptangent = sgVector3(vertices[i*vtxsize+tangentoffset]);
		temptangent -= tempnormal*tempnormal.dot(temptangent);
		temptangent.normalize();
		memccpy(&vertices[i*vtxsize+tangentoffset], &temptangent.x, 3, sizeof(float));
	}

	//Calculate bitangent direction
	for(i = 0; i < vertexnum; i++)
	{
		tempnormal = sgVector3(vertices[i*vtxsize+3]);
		temptangent = sgVector3(vertices[i*vtxsize+tangentoffset]);;
		vertices[i*vtxsize+tangentoffset+3] = (tempnormal.cross(temptangent).dot(bitangents[i]) < 0.0f)? -1.0f : 1.0f;
	}

	delete[] bitangents;
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

	vertex->normal.x += normal[0];
	vertex->normal.y += normal[1];
	vertex->normal.z += normal[2];

	neighbour_a->normal.x += normal[0];
	neighbour_a->normal.y += normal[1];
	neighbour_a->normal.z += normal[2];
	neighbour_b->normal.x += normal[0];
	neighbour_b->normal.y += normal[1];
	neighbour_b->normal.z += normal[2];
}

void sgMesh::calculateFaceTangent(sgVertex *vertex, sgVertex *neighbour_a, sgVertex *neighbour_b, sgVector3 *tangent0, sgVector3 *tangent1, sgVector3 *tangent2, sgVector3 *bitangent0, sgVector3 *bitangent1, sgVector3 *bitangent2)
{
	sgVector3 posdir1 = neighbour_a->position-vertex->position;
	sgVector3 posdir2 = neighbour_b->position-vertex->position;
	sgVector2 uvdir1 = neighbour_a->uv-vertex->uv;
	sgVector2 uvdir2 = neighbour_b->uv-vertex->uv;

	float r = (uvdir1.x*uvdir2.y-uvdir2.x*uvdir1.y);
	if(r != 0)
		r = 1.0/r;
	else
		r = 1.0;
	sgVector4 tangent((uvdir2.y*posdir1.x-uvdir1.y*posdir2.x)*r, (uvdir2.y*posdir1.y-uvdir1.y*posdir2.y)*r, (uvdir2.y*posdir1.z-uvdir1.y*posdir2.z)*r, 0.0);
	sgVector3 bitangent((uvdir1.x*posdir2.x-uvdir2.x*posdir1.x)*r, (uvdir1.x*posdir2.y-uvdir2.x*posdir1.y)*r, (uvdir1.x*posdir2.z-uvdir2.x*posdir1.z)*r);

	*tangent0 += tangent;
	*tangent1 += tangent;
	*tangent2 += tangent;

	*bitangent0 += bitangent;
	*bitangent1 += bitangent;
	*bitangent2 += bitangent;
}

void sgMesh::invertTexCoordsX()
{
	for(int i = 0; i < vertexnum; i++)
	{
		vertices[i*vtxsize+6] = 1.0f-vertices[i*vtxsize+6];
	}
}

void sgMesh::invertTexCoordsY()
{
	for(int i = 0; i < vertexnum; i++)
	{
		vertices[i*vtxsize+7] = 1.0f-vertices[i*vtxsize+7];
	}
}

void sgMesh::calcCullSphere()
{
	//Find the center
	sgVector3 vmax(-1000000000.0, -1000000000.0, -1000000000.0);
	sgVector3 vmin(1000000000.0, 1000000000.0, 1000000000.0);
	sgVector3 center;
	sgVertex *tempvtx;
	for(int i = 0; i < vertexnum; i++)
	{
		tempvtx = (sgVertex*)(vertices+i*vtxsize);

		if(tempvtx->position.x > vmax.x)
			vmax.x = tempvtx->position.x;
		if(tempvtx->position.y > vmax.y)
			vmax.y = tempvtx->position.y;
		if(tempvtx->position.z > vmax.z)
			vmax.z = tempvtx->position.z;

		if(tempvtx->position.x < vmin.x)
			vmin.x = tempvtx->position.x;
		if(tempvtx->position.y < vmin.y)
			vmin.y = tempvtx->position.y;
		if(tempvtx->position.z < vmin.z)
			vmin.z = tempvtx->position.z;
	}
	center = vmin+vmax;
	center *= 0.5;

	//Find the radius
	float radius = center.dist(vmax);
	cullsphere = sgVector4(center.x, center.y, center.z, radius);
}
