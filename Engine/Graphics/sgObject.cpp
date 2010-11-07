//
//	sgObject.cpp
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

#include "sgObject.h"

#include "sgResourceManager.h"
#include "sgObjectFiles.h"
#include "sgMaterial.h"
#include "sgMesh.h"
#include "sgLight.h"
#include "sgShadowVolume.h"
#include "sgRenderer.h"
#include "sgDebug.h"

#include <stdlib.h>
#include <math.h>s

sgObject::sgObject(sgObject* p, sgObject *n)
{
	prev = p;
	next = n;
	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
	shadow = false;
	sky = false;
	shadowvolume = NULL;
	tag = 0;
}

sgObject::~sgObject()
{
	if(shadowvolume != NULL)
	{
		delete shadowvolume;
		shadowvolume = NULL;
	}
}

sgObject *sgObject::createObject(const char *name)
{
	sgObjectContainer *cont = (sgObjectContainer*)sgResourceManager::getResource(name);
	if(cont != NULL)
	{
		next = new sgObject(prev, next);
		for(int i = 0; i < cont->meshs.size(); i++)
		{
			next->meshs.push_back(cont->meshs[i]);
			next->materials.push_back(cont->materials[i]);
		}
		return next;
	}
	
	cont = new sgObjectContainer;
	
	if(std::string(name).compare(std::string("box")) == 0)
	{
		sgMesh *mesh = new sgMesh;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 36;
		mesh->indexnum = 36;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = mesh->indexnum-i-1;
		
		//left
		mesh->vertices[0].position.x = -1.0f;
		mesh->vertices[0].position.y = -1.0f;
		mesh->vertices[0].position.z = -1.0f;
		mesh->vertices[0].uv.x = 0.0f;
		mesh->vertices[0].uv.y = 0.0f;
		
		mesh->vertices[1].position.x = 1.0f;
		mesh->vertices[1].position.y = 1.0f;
		mesh->vertices[1].position.z = -1.0f;
		mesh->vertices[1].uv.x = 1.0f;
		mesh->vertices[1].uv.y = 1.0f;
		
		mesh->vertices[2].position.x = -1.0f;
		mesh->vertices[2].position.y = 1.0f;
		mesh->vertices[2].position.z = -1.0f;
		mesh->vertices[2].uv.x = 0.0f;
		mesh->vertices[2].uv.y = 1.0f;
		
		
		mesh->vertices[3].position.x = -1.0f;
		mesh->vertices[3].position.y = -1.0f;
		mesh->vertices[3].position.z = -1.0f;
		mesh->vertices[3].uv.x = 0.0f;
		mesh->vertices[3].uv.y = 0.0f;
		
		mesh->vertices[4].position.x = 1.0f;
		mesh->vertices[4].position.y = -1.0f;
		mesh->vertices[4].position.z = -1.0f;
		mesh->vertices[4].uv.x = 1.0f;
		mesh->vertices[4].uv.y = 0.0f;
		
		mesh->vertices[5].position.x = 1.0f;
		mesh->vertices[5].position.y = 1.0f;
		mesh->vertices[5].position.z = -1.0f;
		mesh->vertices[5].uv.x = 1.0f;
		mesh->vertices[5].uv.y = 1.0f;
		
		//back
		mesh->vertices[6].position.x = 1.0f;
		mesh->vertices[6].position.y = 1.0f;
		mesh->vertices[6].position.z = 1.0f;
		mesh->vertices[6].uv.x = 1.0f;
		mesh->vertices[6].uv.y = 1.0f;
		
		mesh->vertices[7].position.x = 1.0f;
		mesh->vertices[7].position.y = -1.0f;
		mesh->vertices[7].position.z = -1.0f;
		mesh->vertices[7].uv.x = 0.0f;
		mesh->vertices[7].uv.y = 0.0f;
		
		mesh->vertices[8].position.x = 1.0f;
		mesh->vertices[8].position.y = -1.0f;
		mesh->vertices[8].position.z = 1.0f;
		mesh->vertices[8].uv.x = 1.0f;
		mesh->vertices[8].uv.y = 0.0f;
		
		
		mesh->vertices[9].position.x = 1.0f;
		mesh->vertices[9].position.y = 1.0f;
		mesh->vertices[9].position.z = 1.0f;
		mesh->vertices[9].uv.x = 1.0f;
		mesh->vertices[9].uv.y = 1.0f;
		
		mesh->vertices[10].position.x = 1.0f;
		mesh->vertices[10].position.y = 1.0f;
		mesh->vertices[10].position.z = -1.0f;
		mesh->vertices[10].uv.x = 0.0f;
		mesh->vertices[10].uv.y = 1.0f;
		
		mesh->vertices[11].position.x = 1.0f;
		mesh->vertices[11].position.y = -1.0f;
		mesh->vertices[11].position.z = -1.0f;
		mesh->vertices[11].uv.x = 0.0f;
		mesh->vertices[11].uv.y = 0.0f;
		
		//right
		mesh->vertices[12].position.x = 1.0f;
		mesh->vertices[12].position.y = 1.0f;
		mesh->vertices[12].position.z = 1.0f;
		mesh->vertices[12].uv.x = 0.0f;
		mesh->vertices[12].uv.y = 1.0f;
		
		mesh->vertices[13].position.x = 1.0f;
		mesh->vertices[13].position.y = -1.0f;
		mesh->vertices[13].position.z = 1.0f;
		mesh->vertices[13].uv.x = 0.0f;
		mesh->vertices[13].uv.y = 0.0f;
		
		mesh->vertices[14].position.x = -1.0f;
		mesh->vertices[14].position.y = -1.0f;
		mesh->vertices[14].position.z = 1.0f;
		mesh->vertices[14].uv.x = 1.0f;
		mesh->vertices[14].uv.y = 0.0f;
		
		
		mesh->vertices[15].position.x = 1.0f;
		mesh->vertices[15].position.y = 1.0f;
		mesh->vertices[15].position.z = 1.0f;
		mesh->vertices[15].uv.x = 0.0f;
		mesh->vertices[15].uv.y = 1.0f;
		
		mesh->vertices[16].position.x = -1.0f;
		mesh->vertices[16].position.y = -1.0f;
		mesh->vertices[16].position.z = 1.0f;
		mesh->vertices[16].uv.x = 1.0f;
		mesh->vertices[16].uv.y = 0.0f;
		
		mesh->vertices[17].position.x = -1.0f;
		mesh->vertices[17].position.y = 1.0f;
		mesh->vertices[17].position.z = 1.0f;
		mesh->vertices[17].uv.x = 1.0f;
		mesh->vertices[17].uv.y = 1.0f;
		
		//front
		mesh->vertices[18].position.x = -1.0f;
		mesh->vertices[18].position.y = -1.0f;
		mesh->vertices[18].position.z = -1.0f;
		mesh->vertices[18].uv.x = 1.0f;
		mesh->vertices[18].uv.y = 0.0f;
		
		mesh->vertices[19].position.x = -1.0f;
		mesh->vertices[19].position.y = 1.0f;
		mesh->vertices[19].position.z = -1.0f;
		mesh->vertices[19].uv.x = 1.0f;
		mesh->vertices[19].uv.y = 1.0f;
		
		mesh->vertices[20].position.x = -1.0f;
		mesh->vertices[20].position.y = 1.0f;
		mesh->vertices[20].position.z = 1.0f;
		mesh->vertices[20].uv.x = 0.0f;
		mesh->vertices[20].uv.y = 1.0f;
		
		
		mesh->vertices[21].position.x = -1.0f;
		mesh->vertices[21].position.y = -1.0f;
		mesh->vertices[21].position.z = -1.0f;
		mesh->vertices[21].uv.x = 1.0f;
		mesh->vertices[21].uv.y = 0.0f;
		
		mesh->vertices[22].position.x = -1.0f;
		mesh->vertices[22].position.y = 1.0f;
		mesh->vertices[22].position.z = 1.0f;
		mesh->vertices[22].uv.x = 0.0f;
		mesh->vertices[22].uv.y = 1.0f;
		
		mesh->vertices[23].position.x = -1.0f;
		mesh->vertices[23].position.y = -1.0f;
		mesh->vertices[23].position.z = 1.0f;
		mesh->vertices[23].uv.x = 0.0f;
		mesh->vertices[23].uv.y = 0.0f;
		
		//bottom
		mesh->vertices[24].position.x = -1.0f;
		mesh->vertices[24].position.y = -1.0f;
		mesh->vertices[24].position.z = -1.0f;
		mesh->vertices[24].uv.x = 1.0f;
		mesh->vertices[24].uv.y = 1.0f;
		
		mesh->vertices[25].position.x = 1.0f;
		mesh->vertices[25].position.y = -1.0f;
		mesh->vertices[25].position.z = 1.0f;
		mesh->vertices[25].uv.x = 0.0f;
		mesh->vertices[25].uv.y = 0.0f;
		
		mesh->vertices[26].position.x = 1.0f;
		mesh->vertices[26].position.y = -1.0f;
		mesh->vertices[26].position.z = -1.0f;
		mesh->vertices[26].uv.x = 1.0f;
		mesh->vertices[26].uv.y = 0.0f;
		
		
		mesh->vertices[27].position.x = -1.0f;
		mesh->vertices[27].position.y = -1.0f;
		mesh->vertices[27].position.z = -1.0f;
		mesh->vertices[27].uv.x = 1.0f;
		mesh->vertices[27].uv.y = 1.0f;
		
		mesh->vertices[28].position.x = -1.0f;
		mesh->vertices[28].position.y = -1.0f;
		mesh->vertices[28].position.z = 1.0f;
		mesh->vertices[28].uv.x = 0.0f;
		mesh->vertices[28].uv.y = 1.0f;
		
		mesh->vertices[29].position.x = 1.0f;
		mesh->vertices[29].position.y = -1.0f;
		mesh->vertices[29].position.z = 1.0f;
		mesh->vertices[29].uv.x = 0.0f;
		mesh->vertices[29].uv.y = 0.0f;
		
		//top
		mesh->vertices[30].position.x = 1.0f;
		mesh->vertices[30].position.y = 1.0f;
		mesh->vertices[30].position.z = 1.0f;
		mesh->vertices[30].uv.x = 1.0f;
		mesh->vertices[30].uv.y = 0.0f;
		
		mesh->vertices[31].position.x = -1.0f;
		mesh->vertices[31].position.y = 1.0f;
		mesh->vertices[31].position.z = 1.0f;
		mesh->vertices[31].uv.x = 1.0f;
		mesh->vertices[31].uv.y = 1.0f;
		
		mesh->vertices[32].position.x = -1.0f;
		mesh->vertices[32].position.y = 1.0f;
		mesh->vertices[32].position.z = -1.0f;
		mesh->vertices[32].uv.x = 0.0f;
		mesh->vertices[32].uv.y = 1.0f;
		
		
		mesh->vertices[33].position.x = 1.0f;
		mesh->vertices[33].position.y = 1.0f;
		mesh->vertices[33].position.z = 1.0f;
		mesh->vertices[33].uv.x = 1.0f;
		mesh->vertices[33].uv.y = 0.0f;
		
		mesh->vertices[34].position.x = -1.0f;
		mesh->vertices[34].position.y = 1.0f;
		mesh->vertices[34].position.z = -1.0f;
		mesh->vertices[34].uv.x = 0.0f;
		mesh->vertices[34].uv.y = 1.0f;
		
		mesh->vertices[35].position.x = 1.0f;
		mesh->vertices[35].position.y = 1.0f;
		mesh->vertices[35].position.z = -1.0f;
		mesh->vertices[35].uv.x = 0.0f;
		mesh->vertices[35].uv.y = 0.0f;
		
		mesh->calculateNormals();
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
		
		cont->materials.push_back(sgMaterial::getMaterial());
	}else if(std::string(name).compare(std::string("skycube")) == 0)
	{
		sgMesh *mesh = new sgMesh;
		
		cont->materials.push_back(sgMaterial::getMaterial((unsigned int)0));
		cont->materials.push_back(sgMaterial::getMaterial((unsigned int)0));
		cont->materials.push_back(sgMaterial::getMaterial((unsigned int)0));
		cont->materials.push_back(sgMaterial::getMaterial((unsigned int)0));
		cont->materials.push_back(sgMaterial::getMaterial((unsigned int)0));
		cont->materials.push_back(sgMaterial::getMaterial((unsigned int)0));
		
		float widthoff = 0;
		float heightoff = 0;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = i;
		
		//left
		mesh->vertices[0].position.x = -1.0f;
		mesh->vertices[0].position.y = -1.0f;
		mesh->vertices[0].position.z = -1.0f;
		mesh->vertices[0].uv.x = widthoff;
		mesh->vertices[0].uv.y = 1.0f-heightoff;
		
		mesh->vertices[1].position.x = 1.0f;
		mesh->vertices[1].position.y = 1.0f;
		mesh->vertices[1].position.z = -1.0f;
		mesh->vertices[1].uv.x = 1.0f-widthoff;
		mesh->vertices[1].uv.y = heightoff;
		
		mesh->vertices[2].position.x = -1.0f;
		mesh->vertices[2].position.y = 1.0f;
		mesh->vertices[2].position.z = -1.0f;
		mesh->vertices[2].uv.x = widthoff;
		mesh->vertices[2].uv.y = heightoff;
		
		
		mesh->vertices[3].position.x = -1.0f;
		mesh->vertices[3].position.y = -1.0f;
		mesh->vertices[3].position.z = -1.0f;
		mesh->vertices[3].uv.x = widthoff;
		mesh->vertices[3].uv.y = 1.0f-heightoff;
		
		mesh->vertices[4].position.x = 1.0f;
		mesh->vertices[4].position.y = -1.0f;
		mesh->vertices[4].position.z = -1.0f;
		mesh->vertices[4].uv.x = 1.0f-widthoff;
		mesh->vertices[4].uv.y = 1.0f-heightoff;
		
		mesh->vertices[5].position.x = 1.0f;
		mesh->vertices[5].position.y = 1.0f;
		mesh->vertices[5].position.z = -1.0f;
		mesh->vertices[5].uv.x = 1.0f-widthoff;
		mesh->vertices[5].uv.y = heightoff;
		
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
		
		mesh = new sgMesh;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = i;
		
		//back
		mesh->vertices[0].position.x = 1.0f;
		mesh->vertices[0].position.y = 1.0f;
		mesh->vertices[0].position.z = 1.0f;
		mesh->vertices[0].uv.x = 1.0f-widthoff;
		mesh->vertices[0].uv.y = heightoff;
		
		mesh->vertices[1].position.x = 1.0f;
		mesh->vertices[1].position.y = -1.0f;
		mesh->vertices[1].position.z = -1.0f;
		mesh->vertices[1].uv.x = widthoff;
		mesh->vertices[1].uv.y = 1.0f-heightoff;
		
		mesh->vertices[2].position.x = 1.0f;
		mesh->vertices[2].position.y = -1.0f;
		mesh->vertices[2].position.z = 1.0f;
		mesh->vertices[2].uv.x = 1.0f-widthoff;
		mesh->vertices[2].uv.y = 1.0f-heightoff;
		
		
		mesh->vertices[3].position.x = 1.0f;
		mesh->vertices[3].position.y = 1.0f;
		mesh->vertices[3].position.z = 1.0f;
		mesh->vertices[3].uv.x = 1.0f-widthoff;
		mesh->vertices[3].uv.y = heightoff;
		
		mesh->vertices[4].position.x = 1.0f;
		mesh->vertices[4].position.y = 1.0f;
		mesh->vertices[4].position.z = -1.0f;
		mesh->vertices[4].uv.x = widthoff;
		mesh->vertices[4].uv.y = heightoff;
		
		mesh->vertices[5].position.x = 1.0f;
		mesh->vertices[5].position.y = -1.0f;
		mesh->vertices[5].position.z = -1.0f;
		mesh->vertices[5].uv.x = widthoff;
		mesh->vertices[5].uv.y = 1.0f-heightoff;
		
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
		
		mesh = new sgMesh;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = i;
		
		//right
		mesh->vertices[0].position.x = 1.0f;
		mesh->vertices[0].position.y = 1.0f;
		mesh->vertices[0].position.z = 1.0f;
		mesh->vertices[0].uv.x = widthoff;
		mesh->vertices[0].uv.y = heightoff;
		
		mesh->vertices[1].position.x = 1.0f;
		mesh->vertices[1].position.y = -1.0f;
		mesh->vertices[1].position.z = 1.0f;
		mesh->vertices[1].uv.x = widthoff;
		mesh->vertices[1].uv.y = 1.0f-heightoff;
		
		mesh->vertices[2].position.x = -1.0f;
		mesh->vertices[2].position.y = -1.0f;
		mesh->vertices[2].position.z = 1.0f;
		mesh->vertices[2].uv.x = 1.0f-widthoff;
		mesh->vertices[2].uv.y = 1.0f-heightoff;
		
		
		mesh->vertices[3].position.x = 1.0f;
		mesh->vertices[3].position.y = 1.0f;
		mesh->vertices[3].position.z = 1.0f;
		mesh->vertices[3].uv.x = widthoff;
		mesh->vertices[3].uv.y = heightoff;
		
		mesh->vertices[4].position.x = -1.0f;
		mesh->vertices[4].position.y = -1.0f;
		mesh->vertices[4].position.z = 1.0f;
		mesh->vertices[4].uv.x = 1.0f-widthoff;
		mesh->vertices[4].uv.y = 1.0f-heightoff;
		
		mesh->vertices[5].position.x = -1.0f;
		mesh->vertices[5].position.y = 1.0f;
		mesh->vertices[5].position.z = 1.0f;
		mesh->vertices[5].uv.x = 1.0f-widthoff;
		mesh->vertices[5].uv.y = heightoff;
		
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
		
		mesh = new sgMesh;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = i;
		
		//front
		mesh->vertices[0].position.x = -1.0f;
		mesh->vertices[0].position.y = -1.0f;
		mesh->vertices[0].position.z = -1.0f;
		mesh->vertices[0].uv.x = 1.0f-widthoff;
		mesh->vertices[0].uv.y = 1.0f-heightoff;
		
		mesh->vertices[1].position.x = -1.0f;
		mesh->vertices[1].position.y = 1.0f;
		mesh->vertices[1].position.z = -1.0f;
		mesh->vertices[1].uv.x = 1.0f-widthoff;
		mesh->vertices[1].uv.y = heightoff;
		
		mesh->vertices[2].position.x = -1.0f;
		mesh->vertices[2].position.y = 1.0f;
		mesh->vertices[2].position.z = 1.0f;
		mesh->vertices[2].uv.x = widthoff;
		mesh->vertices[2].uv.y = heightoff;
		
		
		mesh->vertices[3].position.x = -1.0f;
		mesh->vertices[3].position.y = -1.0f;
		mesh->vertices[3].position.z = -1.0f;
		mesh->vertices[3].uv.x = 1.0f-widthoff;
		mesh->vertices[3].uv.y = 1.0f-heightoff;
		
		mesh->vertices[4].position.x = -1.0f;
		mesh->vertices[4].position.y = 1.0f;
		mesh->vertices[4].position.z = 1.0f;
		mesh->vertices[4].uv.x = widthoff;
		mesh->vertices[4].uv.y = heightoff;
		
		mesh->vertices[5].position.x = -1.0f;
		mesh->vertices[5].position.y = -1.0f;
		mesh->vertices[5].position.z = 1.0f;
		mesh->vertices[5].uv.x = widthoff;
		mesh->vertices[5].uv.y = 1.0-heightoff;
		
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
		
		mesh = new sgMesh;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = i;
		
		//bottom
		mesh->vertices[0].position.x = -1.0f;
		mesh->vertices[0].position.y = -1.0f;
		mesh->vertices[0].position.z = -1.0f;
		mesh->vertices[0].uv.x = 1.0f-widthoff;
		mesh->vertices[0].uv.y = heightoff;
		
		mesh->vertices[1].position.x = 1.0f;
		mesh->vertices[1].position.y = -1.0f;
		mesh->vertices[1].position.z = 1.0f;
		mesh->vertices[1].uv.x = widthoff;
		mesh->vertices[1].uv.y = 1.0f-heightoff;
		
		mesh->vertices[2].position.x = 1.0f;
		mesh->vertices[2].position.y = -1.0f;
		mesh->vertices[2].position.z = -1.0f;
		mesh->vertices[2].uv.x = 1.0f-widthoff;
		mesh->vertices[2].uv.y = 1.0f-heightoff;
		
		
		mesh->vertices[3].position.x = -1.0f;
		mesh->vertices[3].position.y = -1.0f;
		mesh->vertices[3].position.z = -1.0f;
		mesh->vertices[3].uv.x = 1.0f-widthoff;
		mesh->vertices[3].uv.y = heightoff;
		
		mesh->vertices[4].position.x = -1.0f;
		mesh->vertices[4].position.y = -1.0f;
		mesh->vertices[4].position.z = 1.0f;
		mesh->vertices[4].uv.x = widthoff;
		mesh->vertices[4].uv.y = heightoff;
		
		mesh->vertices[5].position.x = 1.0f;
		mesh->vertices[5].position.y = -1.0f;
		mesh->vertices[5].position.z = 1.0f;
		mesh->vertices[5].uv.x = widthoff;
		mesh->vertices[5].uv.y = 1.0f-heightoff;
		
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
		
		mesh = new sgMesh;
		
		mesh->vertexformat = 0;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		
		mesh->vertices = new sgVertex[mesh->vertexnum];
		mesh->indices = new unsigned short[mesh->indexnum];
		
		for(int i = 0; i < mesh->indexnum; i++)
			mesh->indices[i] = i;
		
		//top
		mesh->vertices[0].position.x = 1.0f;
		mesh->vertices[0].position.y = 1.0f;
		mesh->vertices[0].position.z = 1.0f;
		mesh->vertices[0].uv.x = widthoff;
		mesh->vertices[0].uv.y = heightoff;
		
		mesh->vertices[1].position.x = -1.0f;
		mesh->vertices[1].position.y = 1.0f;
		mesh->vertices[1].position.z = 1.0f;
		mesh->vertices[1].uv.x = widthoff;
		mesh->vertices[1].uv.y = 1.0f-heightoff;
		
		mesh->vertices[2].position.x = -1.0f;
		mesh->vertices[2].position.y = 1.0f;
		mesh->vertices[2].position.z = -1.0f;
		mesh->vertices[2].uv.x = 1.0f-widthoff;
		mesh->vertices[2].uv.y = 1.0f-heightoff;
		
		
		mesh->vertices[3].position.x = 1.0f;
		mesh->vertices[3].position.y = 1.0f;
		mesh->vertices[3].position.z = 1.0f;
		mesh->vertices[3].uv.x = widthoff;
		mesh->vertices[3].uv.y = heightoff;
		
		mesh->vertices[4].position.x = -1.0f;
		mesh->vertices[4].position.y = 1.0f;
		mesh->vertices[4].position.z = -1.0f;
		mesh->vertices[4].uv.x = 1.0f-widthoff;
		mesh->vertices[4].uv.y = 1.0f-heightoff;
		
		mesh->vertices[5].position.x = 1.0f;
		mesh->vertices[5].position.y = 1.0f;
		mesh->vertices[5].position.z = -1.0f;
		mesh->vertices[5].uv.x = 1.0f-widthoff;
		mesh->vertices[5].uv.y = heightoff;
		
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);
	}else
	{
		std::string filename(name);
		unsigned int found;
		
		found = filename.rfind(".sgm");
		if(found != std::string::npos)
		{
			if(!sgObjectFiles::loadSGM(cont, filename.c_str()))
			{
				delete cont;
				return NULL;
			}
		}else
		{
			delete cont;
			return NULL;
		}
	}
	
	sgResourceManager::addResource(name, cont);
	return createObject(name);
}

sgObject *sgObject::createPlane(unsigned int xverts, unsigned int zverts, float xtexscale, float ytexscale)
{
	if(xverts < 2 || zverts < 2)
		return NULL;
	
	next = new sgObject(prev, next);
	
	sgMesh *mesh = new sgMesh;
	
	mesh->vertexformat = 0;
	mesh->vertexnum = xverts*zverts;
	mesh->indexnum = 3*2*(xverts-1)*(zverts-1);
	
	mesh->vertices = new sgVertex[mesh->vertexnum];
	mesh->indices = new unsigned short[mesh->indexnum];
	
	int face = 0;
	for(unsigned int i = 0; i < xverts*zverts-xverts; i++)
	{
		if(i%xverts >= xverts-1)
		{
			continue;
		}
		
		mesh->indices[face*3+0] = i%xverts+((int)(i/xverts))*xverts;
		mesh->indices[face*3+1] = i+xverts+1;
		mesh->indices[face*3+2] = i+xverts;
		face++;
		
		mesh->indices[face*3+0] = i%xverts+((int)(i/xverts))*xverts;
		mesh->indices[face*3+1] = i%xverts+((int)(i/xverts))*xverts+1;
		mesh->indices[face*3+2] = i+xverts+1;
		face++;
	}
	
	for(int x = 0; x < xverts; x++)
	{
		for(int y = 0; y < zverts; y++)
		{
			mesh->vertices[x*zverts+y].position.x = (float)x-0.5f*(float)xverts;
			mesh->vertices[x*zverts+y].position.y = 0.0f;
			mesh->vertices[x*zverts+y].position.z = (float)y-0.5f*(float)zverts;
//			mesh->vertices[x*zverts+y].position.y = sin(mesh->vertices[x*zverts+y].position.x*0.33)*sin(mesh->vertices[x*zverts+y].position.z*0.33)*2.0f;
			mesh->vertices[x*zverts+y].uv.x = (float)x/(float)xverts*xtexscale;
			mesh->vertices[x*zverts+y].uv.y = (float)y/(float)zverts*ytexscale;
		}
	}
		
	mesh->calculateNormals();
	mesh->generateVBO();
	next->meshs.push_back(mesh);
	sgResourceManager::addResource(mesh);
	
	next->materials.push_back(sgMaterial::getMaterial());
	
	return next;
 return next;
}

void sgObject::cloneMaterial(unsigned int mat)
{
	if(mat >= materials.size())
		return;
	
	sgMaterial *temp = sgMaterial::getMaterial();
	*temp = *materials[mat];
	materials[mat] = temp;
}

void sgObject::setAnimation(const char *name, float percent)
{
	for(unsigned int i = 0; i < meshs.size(); i++)
	{
		meshs[i]->setAnimation(name, percent);
	}
}

void sgObject::initShadowVolume()
{
	if(shadowvolume == NULL && sgRenderer::oglversion > 1)
		shadowvolume = new sgShadowVolume(this);
}

void sgObject::updateModel()
{
	matmodel.makeTranslate(position);
	matmodel.scale(scale);
	
	matnormal = rotation.getMatrix();
	matmodel *= matnormal;
}

void sgObject::destroy()
{
	if(prev)
		prev->next = next;
	if(next)
		next->prev = prev;
	
	delete this;
}

void sgObject::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
}
