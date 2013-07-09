//
//	sgObjectBody.cpp
//	iSDGE
//
//	Created by Nils Daumann on 24.11.10.
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

#include "sgObjectBody.h"
#include "sgObjectFiles.h"
#include "sgMaterial.h"
#include "sgMesh.h"
#include "sgColor.h"
#include "sgTexture.h"
#include "sgResourceManager.h"
#include "sgDebug.h"


sgObjectBody::sgObjectBody()
{
	nextbody = NULL;
}

sgObjectBody::~sgObjectBody()
{

}

sgObjectBody *sgObjectBody::makeObject(const char *name, unsigned long flags)
{
	sgObjectContainer *cont = (sgObjectContainer*)sgResourceManager::getResource(name);
	if(cont != NULL)
	{
		for(int i = 0; i < cont->meshs.size(); i++)
		{
			meshs.push_back(cont->meshs[i]);
			materials.push_back(cont->materials[i]);
		}
		return this;
	}

	cont = new sgObjectContainer;
	cont->skeleton = NULL;

	if(std::string(name).compare(std::string("box")) == 0)
	{
		sgMesh *mesh = new sgMesh;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 36;
		mesh->indexnum = 36;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = mesh->indexnum-i-1;

		//left
		mesh->vertices[0*mesh->vtxsize+0] = -1.0f;	//pos x
		mesh->vertices[0*mesh->vtxsize+1] = -1.0f;	//pos y
		mesh->vertices[0*mesh->vtxsize+2] = -1.0f;	//pos z
		mesh->vertices[0*mesh->vtxsize+6] = 0.0f;	//uv x
		mesh->vertices[0*mesh->vtxsize+7] = 0.0f;	//uv y

		mesh->vertices[1*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[2*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[2*mesh->vtxsize+7] = 1.0f;


		mesh->vertices[3*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[3*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[4*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[5*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+7] = 1.0f;

		//back
		mesh->vertices[6*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[6*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[6*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[6*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[6*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[7*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[7*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[7*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[7*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[7*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[8*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[8*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[8*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[8*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[8*mesh->vtxsize+7] = 0.0f;


		mesh->vertices[9*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[9*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[9*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[9*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[9*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[10*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[10*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[10*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[10*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[10*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[11*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[11*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[11*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[11*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[11*mesh->vtxsize+7] = 0.0f;

		//right
		mesh->vertices[12*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[12*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[12*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[12*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[12*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[13*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[13*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[13*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[13*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[13*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[14*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[14*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[14*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[14*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[14*mesh->vtxsize+7] = 0.0f;


		mesh->vertices[15*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[15*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[15*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[15*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[15*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[16*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[16*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[16*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[16*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[16*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[17*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[17*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[17*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[17*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[17*mesh->vtxsize+7] = 1.0f;

		//front
		mesh->vertices[18*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[18*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[18*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[18*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[18*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[19*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[19*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[19*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[19*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[19*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[20*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[20*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[20*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[20*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[20*mesh->vtxsize+7] = 1.0f;


		mesh->vertices[21*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[21*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[21*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[21*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[21*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[22*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[22*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[22*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[22*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[22*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[23*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[23*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[23*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[23*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[23*mesh->vtxsize+7] = 0.0f;

		//bottom
		mesh->vertices[24*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[24*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[24*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[24*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[24*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[25*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[25*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[25*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[25*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[25*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[26*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[26*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[26*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[26*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[26*mesh->vtxsize+7] = 0.0f;


		mesh->vertices[27*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[27*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[27*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[27*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[27*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[28*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[28*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[28*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[28*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[28*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[29*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[29*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[29*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[29*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[29*mesh->vtxsize+7] = 0.0f;

		//top
		mesh->vertices[30*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[30*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[30*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[30*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[30*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[31*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[31*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[31*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[31*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[31*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[32*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[32*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[32*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[32*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[32*mesh->vtxsize+7] = 1.0f;


		mesh->vertices[33*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[33*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[33*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[33*mesh->vtxsize+6] = 1.0f;
		mesh->vertices[33*mesh->vtxsize+7] = 0.0f;

		mesh->vertices[34*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[34*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[34*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[34*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[34*mesh->vtxsize+7] = 1.0f;

		mesh->vertices[35*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[35*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[35*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[35*mesh->vtxsize+6] = 0.0f;
		mesh->vertices[35*mesh->vtxsize+7] = 0.0f;

		mesh->calculateNormals();
		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);

		cont->materials.push_back(sgMaterial::getMaterial());
	}else if(std::string(name).compare(std::string("skycube")) == 0)
	{
		sgMesh *mesh = new sgMesh;

		cont->materials.push_back(sgMaterial::getMaterial(sgShader::BIS_TEXTURE));
		cont->materials.push_back(sgMaterial::getMaterial(sgShader::BIS_TEXTURE));
		cont->materials.push_back(sgMaterial::getMaterial(sgShader::BIS_TEXTURE));
		cont->materials.push_back(sgMaterial::getMaterial(sgShader::BIS_TEXTURE));
		cont->materials.push_back(sgMaterial::getMaterial(sgShader::BIS_TEXTURE));
		cont->materials.push_back(sgMaterial::getMaterial(sgShader::BIS_TEXTURE));

		float widthoff = 0;
		float heightoff = 0;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = i;

		//left
		mesh->vertices[0*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+6] = widthoff;
		mesh->vertices[0*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[1*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[1*mesh->vtxsize+7] = heightoff;

		mesh->vertices[2*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = widthoff;
		mesh->vertices[2*mesh->vtxsize+7] = heightoff;


		mesh->vertices[3*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = widthoff;
		mesh->vertices[3*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[4*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[4*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[5*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[5*mesh->vtxsize+7] = heightoff;

		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);

		mesh = new sgMesh;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = i;

		//back
		mesh->vertices[0*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[0*mesh->vtxsize+7] = heightoff;

		mesh->vertices[1*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = widthoff;
		mesh->vertices[1*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[2*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[2*mesh->vtxsize+7] = 1.0f-heightoff;


		mesh->vertices[3*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[3*mesh->vtxsize+7] = heightoff;

		mesh->vertices[4*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = widthoff;
		mesh->vertices[4*mesh->vtxsize+7] = heightoff;

		mesh->vertices[5*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = widthoff;
		mesh->vertices[5*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);

		mesh = new sgMesh;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = i;

		//right
		mesh->vertices[0*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+6] = widthoff;
		mesh->vertices[0*mesh->vtxsize+7] = heightoff;

		mesh->vertices[1*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = widthoff;
		mesh->vertices[1*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[2*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[2*mesh->vtxsize+7] = 1.0f-heightoff;


		mesh->vertices[3*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = widthoff;
		mesh->vertices[3*mesh->vtxsize+7] = heightoff;

		mesh->vertices[4*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[4*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[5*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[5*mesh->vtxsize+7] = heightoff;

		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);

		mesh = new sgMesh;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = i;

		//front
		mesh->vertices[0*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[0*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[1*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[1*mesh->vtxsize+7] = heightoff;

		mesh->vertices[2*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = widthoff;
		mesh->vertices[2*mesh->vtxsize+7] = heightoff;


		mesh->vertices[3*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[3*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[4*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = widthoff;
		mesh->vertices[4*mesh->vtxsize+7] = heightoff;

		mesh->vertices[5*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = widthoff;
		mesh->vertices[5*mesh->vtxsize+7] = 1.0-heightoff;

		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);

		mesh = new sgMesh;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = i;

		//bottom
		mesh->vertices[0*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[0*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[0*mesh->vtxsize+7] = heightoff;

		mesh->vertices[1*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = widthoff;
		mesh->vertices[1*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[2*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[2*mesh->vtxsize+7] = 1.0f-heightoff;


		mesh->vertices[3*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[3*mesh->vtxsize+7] = heightoff;

		mesh->vertices[4*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = widthoff;
		mesh->vertices[4*mesh->vtxsize+7] = heightoff;

		mesh->vertices[5*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = widthoff;
		mesh->vertices[5*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->generateVBO();
		cont->meshs.push_back(mesh);
		sgResourceManager::addResource(mesh);

		mesh = new sgMesh;

		mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
		mesh->vtxsize = 8;
		mesh->vertexnum = 6;
		mesh->indexnum = 6;
		mesh->indexsize = 2;

		mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
		mesh->indices = new unsigned short[mesh->indexnum];

		for(int i = 0; i < mesh->indexnum; i++)
			((unsigned short*)mesh->indices)[i] = i;

		//top
		mesh->vertices[0*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[0*mesh->vtxsize+6] = widthoff;
		mesh->vertices[0*mesh->vtxsize+7] = heightoff;

		mesh->vertices[1*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[1*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[1*mesh->vtxsize+6] = widthoff;
		mesh->vertices[1*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[2*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[2*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[2*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[2*mesh->vtxsize+7] = 1.0f-heightoff;


		mesh->vertices[3*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+2] = 1.0f;
		mesh->vertices[3*mesh->vtxsize+6] = widthoff;
		mesh->vertices[3*mesh->vtxsize+7] = heightoff;

		mesh->vertices[4*mesh->vtxsize+0] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[4*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[4*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[4*mesh->vtxsize+7] = 1.0f-heightoff;

		mesh->vertices[5*mesh->vtxsize+0] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+1] = 1.0f;
		mesh->vertices[5*mesh->vtxsize+2] = -1.0f;
		mesh->vertices[5*mesh->vtxsize+6] = 1.0f-widthoff;
		mesh->vertices[5*mesh->vtxsize+7] = heightoff;

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
			if(!sgObjectFiles::loadSGM(cont, filename.c_str(), flags))
			{
				sgLog("Can´t load object file: %s", filename.c_str());
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
	return makeObject(name);
}

void sgObjectBody::addTerrainPlane(unsigned int xverts, unsigned int zverts, sgVector3 posoffset, sgMaterial *mat, sgVector2 scale, unsigned char xchunk, unsigned char zchunk, sgVector2 uvfac, sgTexture *hmp, sgVector2 hmppartsize, sgVector4 hmpscale, bool addborder)
{
	if(xverts < 2 || zverts < 2)
		return;

	sgMesh *mesh = new sgMesh;

	mesh->vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
	mesh->vtxsize = 8;
	mesh->vertexnum = xverts*zverts;
	mesh->indexnum = 3*2*(xverts-1)*(zverts-1);
	mesh->indexsize = 2;
	if(addborder)
	{
		mesh->vertexnum += 2*(xverts+zverts);
		mesh->indexnum += 12*(xverts+zverts-2);
	}
	mesh->vertices = new float[mesh->vertexnum*mesh->vtxsize];
	mesh->indices = new unsigned short[mesh->indexnum];

	int face = 0;
	for(unsigned int i = 0; i < xverts*zverts-xverts; i++)
	{
		if(i%xverts >= xverts-1)
		{
			continue;
		}

		((unsigned short*)mesh->indices)[face*3+0] = i%xverts+((int)(i/xverts))*xverts;
		((unsigned short*)mesh->indices)[face*3+1] = i+xverts+1;
		((unsigned short*)mesh->indices)[face*3+2] = i+xverts;
		face++;

		((unsigned short*)mesh->indices)[face*3+0] = i%xverts+((int)(i/xverts))*xverts;
		((unsigned short*)mesh->indices)[face*3+1] = i%xverts+((int)(i/xverts))*xverts+1;
		((unsigned short*)mesh->indices)[face*3+2] = i+xverts+1;
		face++;
	}

	float widthfac = 0;
	float heightfac = 0;
	if(hmp != NULL)
	{
		widthfac = hmppartsize.x/(xverts-1);
		heightfac = hmppartsize.y/(zverts-1);
	}

	float coordx = 0;
	float coordy = 0;
	sgColorA color;
	sgVector3 norm = sgVector3(0, 1, 0);
	float height = 0.0;
	sgVector3 pos;
	sgVector3 pos_A;
	sgVector3 pos_B;
	sgVector3 pos_C;
	sgVector3 pos_D;
	sgVector3 dir_A;
	sgVector3 dir_B;
	sgVector3 dir_C;
	sgVector3 dir_D;
	sgVector3 norm_AB;
	sgVector3 norm_BC;
	sgVector3 norm_CD;
	sgVector3 norm_DA;

	for(int x = 0; x < xverts; x++)
	{
		for(int y = 0; y < zverts; y++)
		{
			if(hmp != NULL)
			{
				//Get height
				coordx = hmppartsize.x*xchunk+x*widthfac;
				coordy = hmppartsize.y*zchunk+y*heightfac;
				color = hmp->getPixel(coordx, coordy);
				height = ((float)color.r)*hmpscale.x+((float)color.g)*hmpscale.y+((float)color.b)*hmpscale.z+((float)color.a)*hmpscale.w;
				pos.y = height;

				//Calculate normal
				coordx = hmppartsize.x*xchunk+(x-1)*widthfac;
				coordy = hmppartsize.y*zchunk+(y-1)*heightfac;
				color = hmp->getPixel(coordx, coordy);
				pos_A.x = -1.0;
				pos_A.y = ((float)color.r)*hmpscale.x+((float)color.g)*hmpscale.y+((float)color.b)*hmpscale.z+((float)color.a)*hmpscale.w;
				pos_A.z = -1.0;

				coordx = hmppartsize.x*xchunk+(x+1)*widthfac;
				coordy = hmppartsize.y*zchunk+(y-1)*heightfac;
				color = hmp->getPixel(coordx, coordy);
				pos_B.x = 1.0;
				pos_B.y = ((float)color.r)*hmpscale.x+((float)color.g)*hmpscale.y+((float)color.b)*hmpscale.z+((float)color.a)*hmpscale.w;
				pos_B.z = -1.0;

				coordx = hmppartsize.x*xchunk+(x+1)*widthfac;
				coordy = hmppartsize.y*zchunk+(y+1)*heightfac;
				color = hmp->getPixel(coordx, coordy);
				pos_C.x = 1.0;
				pos_C.y = ((float)color.r)*hmpscale.x+((float)color.g)*hmpscale.y+((float)color.b)*hmpscale.z+((float)color.a)*hmpscale.w;
				pos_C.z = 1.0;

				coordx = hmppartsize.x*xchunk+(x-1)*widthfac;
				coordy = hmppartsize.y*zchunk+(y+1)*heightfac;
				color = hmp->getPixel(coordx, coordy);
				pos_D.x = -1.0;
				pos_D.y = ((float)color.r)*hmpscale.x+((float)color.g)*hmpscale.y+((float)color.b)*hmpscale.z+((float)color.a)*hmpscale.w;
				pos_D.z = 1.0;

				dir_A = pos_A-pos;
				dir_B = pos-pos_B;
				dir_C = pos_C-pos;
				dir_D = pos-pos_D;

				norm_AB = dir_A.cross(dir_B);
				norm_BC = dir_B.cross(dir_C);
				norm_CD = dir_C.cross(dir_D);
				norm_DA = dir_D.cross(dir_A);
				norm_AB.normalize();
				norm_BC.normalize();
				norm_CD.normalize();
				norm_DA.normalize();

				norm = norm_AB+norm_BC+norm_CD+norm_DA;
				norm.normalize();
			}else
			{
				color.a = 0;
			}

			mesh->vertices[(x*zverts+y)*mesh->vtxsize+0] = ((float)x-0.5f*(float)(xverts-1.0))*scale.x+posoffset.x;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+1] = height+posoffset.y;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+2] = ((float)y-0.5f*(float)(zverts-1.0))*scale.y+posoffset.z;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+3] = norm.x*-1;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+4] = norm.y*-1;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+5] = norm.z*-1;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+6] = mesh->vertices[(x*zverts+y)*mesh->vtxsize+0]*uvfac.x+0.5f;
			mesh->vertices[(x*zverts+y)*mesh->vtxsize+7] = mesh->vertices[(x*zverts+y)*mesh->vtxsize+2]*uvfac.y+0.5f;
		}
	}

	if(addborder)
	{
		int startverts = xverts*zverts;
		for(int x = 0; x < xverts-1; x++)
		{
			//top row
			((unsigned short*)mesh->indices)[face*3+0] = x*zverts;
			((unsigned short*)mesh->indices)[face*3+1] = (x+1)*zverts;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+x;
			face++;

			((unsigned short*)mesh->indices)[face*3+0] = (x+1)*zverts;
			((unsigned short*)mesh->indices)[face*3+1] = startverts+x+1;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+x;
			face++;

			//copy position
			mesh->vertices[(startverts+x)*mesh->vtxsize+0] = mesh->vertices[x*zverts*mesh->vtxsize+0];
			mesh->vertices[(startverts+x)*mesh->vtxsize+1] = mesh->vertices[x*zverts*mesh->vtxsize+1]-10.0;
			mesh->vertices[(startverts+x)*mesh->vtxsize+2] = mesh->vertices[x*zverts*mesh->vtxsize+2];

			//copy normal
			mesh->vertices[(startverts+x)*mesh->vtxsize+3] = mesh->vertices[x*zverts*mesh->vtxsize+3];
			mesh->vertices[(startverts+x)*mesh->vtxsize+4] = mesh->vertices[x*zverts*mesh->vtxsize+4];
			mesh->vertices[(startverts+x)*mesh->vtxsize+5] = mesh->vertices[x*zverts*mesh->vtxsize+5];

			//copy texcoords
			mesh->vertices[(startverts+x)*mesh->vtxsize+6] = mesh->vertices[x*zverts*mesh->vtxsize+6];
			mesh->vertices[(startverts+x)*mesh->vtxsize+7] = mesh->vertices[x*zverts*mesh->vtxsize+7];

			//bottom row
			((unsigned short*)mesh->indices)[face*3+0] = (x+1)*zverts+zverts-1;
			((unsigned short*)mesh->indices)[face*3+1] = x*zverts+zverts-1;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+xverts+x;
			face++;

			((unsigned short*)mesh->indices)[face*3+0] = startverts+xverts+x+1;
			((unsigned short*)mesh->indices)[face*3+1] = (x+1)*zverts+zverts-1;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+xverts+x;
			face++;

			//copy position
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+0] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+0];
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+1] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+1]-10.0;
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+2] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+2];

			//copy normal
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+3] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+3];
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+4] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+4];
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+5] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+5];

			//copy texcoords
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+6] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+6];
			mesh->vertices[(startverts+x+xverts)*mesh->vtxsize+7] = mesh->vertices[(x*zverts+zverts-1)*mesh->vtxsize+7];
		}

		//copy position
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+0] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+0];
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+1] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+1]-10.0;
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+2] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+2];

		//copy normal
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+3] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+3];
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+4] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+4];
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+5] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+5];

		//copy texcoords
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+6] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+6];
		mesh->vertices[(startverts+xverts-1)*mesh->vtxsize+7] = mesh->vertices[(xverts-1)*zverts*mesh->vtxsize+7];

		//copy position
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+0] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+0];
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+1] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+1]-10.0;
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+2] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+2];

		//copy normal
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+3] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+3];
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+4] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+4];
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+5] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+5];

		//copy texcoords
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+6] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+6];
		mesh->vertices[(startverts+xverts+xverts-1)*mesh->vtxsize+7] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+7];

		for(int y = 0; y < zverts-1; y++)
		{
			//left row
			((unsigned short*)mesh->indices)[face*3+0] = y+1;
			((unsigned short*)mesh->indices)[face*3+1] = y;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+2*xverts+y;
			face++;

			((unsigned short*)mesh->indices)[face*3+0] = startverts+2*xverts+y+1;
			((unsigned short*)mesh->indices)[face*3+1] = y+1;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+2*xverts+y;
			face++;

			//copy position
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+0] = mesh->vertices[y*mesh->vtxsize+0];
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+1] = mesh->vertices[y*mesh->vtxsize+1]-10.0;
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+2] = mesh->vertices[y*mesh->vtxsize+2];

			//copy normal
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+3] = mesh->vertices[y*mesh->vtxsize+3];
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+4] = mesh->vertices[y*mesh->vtxsize+4];
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+5] = mesh->vertices[y*mesh->vtxsize+5];

			//copy texcoords
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+6] = mesh->vertices[y*mesh->vtxsize+6];
			mesh->vertices[(startverts+2*xverts+y)*mesh->vtxsize+7] = mesh->vertices[y*mesh->vtxsize+7];

			//right row
			((unsigned short*)mesh->indices)[face*3+0] = (xverts-1)*zverts+y;
			((unsigned short*)mesh->indices)[face*3+1] = (xverts-1)*zverts+y+1;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+2*xverts+zverts+y;
			face++;

			((unsigned short*)mesh->indices)[face*3+0] = (xverts-1)*zverts+y+1;
			((unsigned short*)mesh->indices)[face*3+1] = startverts+2*xverts+zverts+y+1;
			((unsigned short*)mesh->indices)[face*3+2] = startverts+2*xverts+zverts+y;
			face++;

			//copy position
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+0] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+0];
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+1] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+1]-10.0f;
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+2] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+2];

			//copy normal
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+3] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+3];
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+4] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+4];
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+5] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+5];

			//copy texcoords
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+6] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+6];
			mesh->vertices[(startverts+2*xverts+zverts+y)*mesh->vtxsize+7] = mesh->vertices[((xverts-1)*zverts+y)*mesh->vtxsize+7];
		}

		//copy position
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+0] = mesh->vertices[(zverts-1)*mesh->vtxsize+0];
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+1] = mesh->vertices[(zverts-1)*mesh->vtxsize+1]-10.0;
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+2] = mesh->vertices[(zverts-1)*mesh->vtxsize+2];

		//copy normal
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+3] = mesh->vertices[(zverts-1)*mesh->vtxsize+3];
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+4] = mesh->vertices[(zverts-1)*mesh->vtxsize+4];
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+5] = mesh->vertices[(zverts-1)*mesh->vtxsize+5];

		//copy texcoords
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+6] = mesh->vertices[(zverts-1)*mesh->vtxsize+6];
		mesh->vertices[(startverts+2*xverts+zverts-1)*mesh->vtxsize+7] = mesh->vertices[(zverts-1)*mesh->vtxsize+7];

		//copy position
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+0] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+0];
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+1] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+1]-10.0;
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+2] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+2];

		//copy normal
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+3] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+3];
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+4] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+4];
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+5] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+5];

		//copy texcoords
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+6] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+6];
		mesh->vertices[(startverts+2*xverts+zverts+zverts-1)*mesh->vtxsize+7] = mesh->vertices[((xverts-1)*zverts+zverts-1)*mesh->vtxsize+7];
	}

	mesh->generateVBO();
	meshs.push_back(mesh);
	sgResourceManager::addResource(mesh);

	if(mat == NULL)
	{
		materials.push_back(sgMaterial::getMaterial());
	}else
	{
		materials.push_back(mat);
	}
}

void sgObjectBody::addMesh(sgMesh *m, sgMaterial *mat)
{
	meshs.push_back(m);
	sgResourceManager::addResource(m);
	if(mat == NULL)
		materials.push_back(sgMaterial::getMaterial());
	else
		materials.push_back(mat);
}

void sgObjectBody::calcCullSphere()
{
	for(int m = 0; m < meshs.size(); m++)
	{
		meshs[m]->calcCullSphere();
	}
}

void sgObjectBody::cloneMaterial(unsigned int mat)
{
	if(mat >= materials.size())
		return;

	sgMaterial *temp = sgMaterial::getMaterial();
	*temp = *materials[mat];
	materials[mat] = temp;
}

void sgObjectBody::destroy()
{
	delete this;
}

void sgObjectBody::destroyAll()
{
	if(nextbody != NULL)
		nextbody->destroyAll();

	destroy();
}
