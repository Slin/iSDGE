//
//	sgShadowVolume.cpp
//	iSDGE
//
//	Created by Nils Daumann on 11.08.10.
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

#include <cmath>
#include "sgShadowVolume.h"
#include "sgVertex.h"
#include "sgMesh.h"
#include "sgObjectBody.h"
#include "sgObject.h"
#include "sgLight.h"
#include "sgDebug.h"

#if !defined __IOS__
	#include <cstring>
#endif

bool sgShadowEdge::operator== (const sgShadowEdge &other)
{
	if(vert1 == other.vert1 && vert2 == other.vert2)
	{
		lastaddor = false;
		return true;
	}

	if(vert1 == other.vert2 && vert2 == other.vert1)
	{
		lastaddor = true;
		return true;
	}

	if(verts[vert1].position.dist(other.verts[other.vert1].position) < 0.0001 && verts[vert2].position.dist(other.verts[other.vert2].position) < 0.0001)
	{
		lastaddor = false;
		return true;
	}

	if(verts[vert1].position.dist(other.verts[other.vert2].position) < 0.0001 && verts[vert2].position.dist(other.verts[other.vert1].position) < 0.0001)
	{
		lastaddor = true;
		return true;
	}

	return false;
}

sgShadowVolume::sgShadowVolume(sgObject *obj, unsigned int lod)
{
	//Set object and create a mesh
	object = obj;
	sgObjectBody *lodstep = object->body;
	for(int i = 0; i < lod; i++)
		lodstep = lodstep->nextbody;
	mesh = new sgMesh;

	//Get vertex and face count
	unsigned int vertexnum = 0;
	unsigned int indexnum = 0;

	for(int i = 0; i < lodstep->meshs.size(); i++)
	{
		vertexnum += lodstep->meshs[i]->vertexnum;
		indexnum += lodstep->meshs[i]->indexnum;
	}

	//Create vertex and face array
	vertices = new sgVertex[vertexnum];
	faces = new sgShadowFace[indexnum/3];

	//Init the vertex array
	vertexnum = 0;
	for(int i = 0; i < lodstep->meshs.size(); i++)
	{
		for(int n = 0; n < lodstep->meshs[i]->vertexnum; n++)
		{
			memcpy(&(vertices[vertexnum+n]), lodstep->meshs[i]->vertices+n*lodstep->meshs[i]->vtxsize, sizeof(sgVertex));
		}

		vertexnum += lodstep->meshs[i]->vertexnum;
	}

	//Create array of unique edges and initialize the faces
	std::vector<sgShadowEdge> edgesvec;
	sgShadowEdge tempedge1;
	sgShadowEdge tempedge2;
	sgShadowEdge tempedge3;
	unsigned int edgeincl1;
	unsigned int edgeincl2;
	unsigned int edgeincl3;
	vertexnum = 0;
	unsigned int realn = 0;
	for(int i = 0; i < lodstep->meshs.size(); i++)
	{
		if(lodstep->meshs[i]->indexsize == 4)
		{
			printf("shadows don´t support objects with 32 bit indices (todo)");
			return;
		}
		unsigned int n = 0;
		for(n = 0; n < lodstep->meshs[i]->indexnum; n += 3)
		{
			tempedge1.verts = vertices;
			tempedge1.vert1 = ((unsigned short*)lodstep->meshs[i]->indices)[n+0]+vertexnum;
			tempedge1.vert2 = ((unsigned short*)lodstep->meshs[i]->indices)[n+1]+vertexnum;
			tempedge1.counter1 = 1;
			tempedge1.counter2 = 0;

			tempedge2.verts = vertices;
			tempedge2.vert1 = ((unsigned short*)lodstep->meshs[i]->indices)[n+1]+vertexnum;
			tempedge2.vert2 = ((unsigned short*)lodstep->meshs[i]->indices)[n+2]+vertexnum;
			tempedge2.counter1 = 1;
			tempedge2.counter2 = 0;

			tempedge3.verts = vertices;
			tempedge3.vert1 = ((unsigned short*)lodstep->meshs[i]->indices)[n+2]+vertexnum;
			tempedge3.vert2 = ((unsigned short*)lodstep->meshs[i]->indices)[n+0]+vertexnum;
			tempedge3.counter1 = 1;
			tempedge3.counter2 = 0;

			calculateFaceNormal(&faces[realn], &vertices[((unsigned short*)lodstep->meshs[i]->indices)[n]+vertexnum], &vertices[((unsigned short*)lodstep->meshs[i]->indices)[n+2]+vertexnum], &vertices[((unsigned short*)lodstep->meshs[i]->indices)[n+1]+vertexnum]);
			faces[realn].flipped1 = false;
			faces[realn].flipped2 = false;
			faces[realn].flipped3 = false;

			edgeincl1 = -1;
			edgeincl2 = -1;
			edgeincl3 = -1;

			for(int x = 0; x < edgesvec.size(); x++)
			{
				if(edgesvec[x] == tempedge1)
				{
					edgeincl1 = x;
					faces[realn].flipped1 = edgesvec[x].lastaddor;
					if(edgesvec[x].lastaddor)
						edgesvec[x].counter2 += 1;
					else
						edgesvec[x].counter1 += 1;

					break;
				}
			}
			for(int x = 0; x < edgesvec.size(); x++)
			{
				if(edgesvec[x] == tempedge2)
				{
					edgeincl2 = x;
					faces[realn].flipped2 = edgesvec[x].lastaddor;
					if(edgesvec[x].lastaddor)
						edgesvec[x].counter2 += 1;
					else
						edgesvec[x].counter1 += 1;
					break;
				}
			}
			for(int x = 0; x < edgesvec.size(); x++)
			{
				if(edgesvec[x] == tempedge3)
				{
					edgeincl3 = x;
					faces[realn].flipped3 = edgesvec[x].lastaddor;
					if(edgesvec[x].lastaddor)
						edgesvec[x].counter2 += 1;
					else
						edgesvec[x].counter1 += 1;
					break;
				}
			}

			if(edgeincl1 == -1)
			{
				edgesvec.push_back(tempedge1);
				faces[realn].edge1 = edgesvec.size()-1;
			}else
			{
				faces[realn].edge1 = edgeincl1;
			}
			if(edgeincl2 == -1)
			{
				edgesvec.push_back(tempedge2);
				faces[realn].edge2 = edgesvec.size()-1;
			}else
			{
				faces[realn].edge2 = edgeincl2;
			}
			if(edgeincl3 == -1)
			{
				edgesvec.push_back(tempedge3);
				faces[realn].edge3 = edgesvec.size()-1;
			}else
			{
				faces[realn].edge3 = edgeincl3;
			}

			realn += 1;
		}

		vertexnum += lodstep->meshs[i]->vertexnum;
	}

	edges = new sgShadowEdge[edgesvec.size()];
	tempedges = new sgShadowEdge[edgesvec.size()];
	memcpy(edges, &edgesvec[0], edgesvec.size()*sizeof(sgShadowEdge));
	edgenum = edgesvec.size();
	facenum = indexnum/3;
}

sgShadowVolume::~sgShadowVolume()
{
	delete mesh;
	mesh = NULL;

	delete[] vertices;
	delete[] faces;
	delete[] edges;
	delete[] tempedges;
}

void sgShadowVolume::update(sgLight *firstlight)
{
	sgLight *light = firstlight;

	sgVector3 normal;
	sgVector3 lightdir;

	sgVertex *volvertices;
	unsigned short *volindices;
	memcpy(tempedges, edges, edgenum*sizeof(sgShadowEdge));

	if(light->position.w == 0)
	{
		lightdir = sgVector3(light->position.x, light->position.y, light->position.z);
		lightdir.normalize();
	}

	//Find silouette
	for(unsigned int n = 0; n < facenum; n++)
	{
		normal = object->matnormal*faces[n].normal;

		if(light->position.w != 0)
		{
			sgLog("Shadows for point lights aren´t supported yet!");
			return;
		}

		if(normal.dot(lightdir) <= 0.0)
		{
			if(faces[n].flipped1)
				tempedges[faces[n].edge1].counter2 -= 1;
			else
				tempedges[faces[n].edge1].counter1 -= 1;

			if(faces[n].flipped2)
				tempedges[faces[n].edge2].counter2 -= 1;
			else
				tempedges[faces[n].edge2].counter1 -= 1;

			if(faces[n].flipped3)
				tempedges[faces[n].edge3].counter2 -= 1;
			else
				tempedges[faces[n].edge3].counter1 -= 1;
		}
	}

	unsigned int edgnum = 0;
	for(int x = 0; x < edgenum; x++)
	{
		if(tempedges[x].counter1+tempedges[x].counter2 == 1)
		{
			edgnum += 1;
		}
	}

	//Create mesh
	volvertices = new sgVertex[edgnum*4];
	volindices = new unsigned short[edgnum*6];

	unsigned int indcounter = 0;
	for(int x = 0; x < edgenum; x++)
	{
		if(tempedges[x].counter1+tempedges[x].counter2 == 1)
		{
			if(tempedges[x].counter1 == 1)
			{
				volvertices[0+indcounter*4] = vertices[edges[x].vert1];
				volvertices[0+indcounter*4].normal.makeIdentity();
				volvertices[1+indcounter*4] = vertices[edges[x].vert2];
				volvertices[1+indcounter*4].normal.makeIdentity();

				volvertices[2+indcounter*4] = vertices[edges[x].vert1];
				volvertices[2+indcounter*4].normal = lightdir;
				volvertices[3+indcounter*4] = vertices[edges[x].vert2];
				volvertices[3+indcounter*4].normal = lightdir;
			}else
			{
				volvertices[0+indcounter*4] = vertices[edges[x].vert2];
				volvertices[0+indcounter*4].normal.makeIdentity();
				volvertices[1+indcounter*4] = vertices[edges[x].vert1];
				volvertices[1+indcounter*4].normal.makeIdentity();

				volvertices[2+indcounter*4] = vertices[edges[x].vert2];
				volvertices[2+indcounter*4].normal = lightdir;
				volvertices[3+indcounter*4] = vertices[edges[x].vert1];
				volvertices[3+indcounter*4].normal = lightdir;
			}

			volindices[0+indcounter*6] = 3+indcounter*4;
			volindices[1+indcounter*6] = 2+indcounter*4;
			volindices[2+indcounter*6] = 0+indcounter*4;

			volindices[3+indcounter*6] = 1+indcounter*4;
			volindices[4+indcounter*6] = 3+indcounter*4;
			volindices[5+indcounter*6] = 0+indcounter*4;

			indcounter += 1;
		}
	}

	if(mesh->vertices != NULL)
		delete[] mesh->vertices;
	if(mesh->indices != NULL)
	{
		unsigned short *ind = static_cast<unsigned short*>(mesh->indices);
		delete ind;
	}

	mesh->indexsize = 2;
	mesh->vertexnum = edgnum*4;
	mesh->indexnum = edgnum*6;
	mesh->vertices = (float*)volvertices;
	mesh->indices = volindices;

/*	if(mesh->vbo == -1)
		mesh->generateVBO(true);*/
}

void sgShadowVolume::calculateFaceNormal(sgShadowFace *face, sgVertex *vertex, sgVertex *neighbour_a, sgVertex *neighbour_b)
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

	face->normal.x = normal[0];
	face->normal.y = normal[1];
	face->normal.z = normal[2];
}
