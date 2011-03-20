//
//	sgObjectFiles.cpp
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

#include "sgObjectFiles.h"

#include "sgXML.h"
#include "sgResourceManager.h"
#include "sgMaterial.h"
#include "sgMesh.h"
#include "sgObjectBody.h"
#include "sgDebug.h"

namespace sgObjectFiles
{
	bool loadSGM(sgObjectContainer *obj, const char *filename, unsigned long flags)
	{
		const char *filepath = sgResourceManager::getPath(filename);
		FILE *file = fopen(filepath, "rb");
		delete[] filepath;
		if(!file)
		{
			sgLog("could not open file: %s", filename);
			return false;
		}
		
		unsigned char version_id;
		fread(&version_id, 1, 1, file);
		if(version_id != 0)
		{
			sgLog("the file format is out of date or not supported: %s", filename);
			return false;
		}
		
		//Get materials
		unsigned char countmats = 0;
		fread(&countmats, 1, 1, file);
		sgSGMMaterial* materials = new sgSGMMaterial[countmats];
		for(unsigned int i = 0; i < countmats; i++)
		{
			fread(&materials[i].id_, 1, 1, file);
			unsigned char texcount = 0;
			fread(&texcount, 1, 1, file);
			for(unsigned int n = 0; n < texcount; n++)
			{
				unsigned short lentexfilename;
				fread(&lentexfilename, 2, 1, file);
				char *texfilename = new char[lentexfilename];
				fread(texfilename, 1, lentexfilename, file);
				materials[i].texnames.push_back(std::string(texfilename));
				delete[] texfilename;
			}
		}
		
		//Get meshes
		unsigned char countmeshs = 0;
		fread(&countmeshs, 1, 1, file);
		sgSGMMesh* meshes = new sgSGMMesh[countmeshs];
		for(int i = 0; i < countmeshs; i++)
		{
			fread(&meshes[i].id_, 1, 1, file);
			unsigned char matid = 0;
			fread(&matid, 1, 1, file);
			
			for(unsigned int n = 0; n < countmats; n++)
			{
				if(materials[n].id_ == matid)
				{
					meshes[i].material = &materials[n];
					break;
				}
			}
			
			unsigned short numverts = 0;
			fread(&numverts, 2, 1, file);
			unsigned char uvcount = 0;
			fread(&uvcount, 1, 1, file);
			unsigned char datacount = 0;
			fread(&datacount, 1, 1, file);
			unsigned char hastangent = 0;
			fread(&hastangent, 1, 1, file);
			
			unsigned short numfloats = 8;
			if((flags & GEN_TANGENT) > 0 || hastangent == 1)
			{
				meshes[i].vtxformat = TANGENT;
				meshes[i].vtxsize = SZ_TANGENT;
				if(uvcount > 1)
				{
					meshes[i].vtxformat = TANGENTSECONDUV;
					meshes[i].vtxsize = SZ_TANGENTSECONDUV;
					numfloats = 10;
					if(datacount == 4)
					{
						meshes[i].vtxformat = TANGENTSECONDUVCOLOR;
						meshes[i].vtxsize = SZ_TANGENTSECONDUVCOLOR;
						numfloats = 14;
					}
				}else if(datacount == 4)
				{
					meshes[i].vtxformat = TANGENTCOLOR;
					meshes[i].vtxsize = SZ_TANGENTCOLOR;
					numfloats = 12;
				}
			}else
			{
				meshes[i].vtxformat = BASIC;
				meshes[i].vtxsize = SZ_BASIC;
				if(uvcount > 1)
				{
					meshes[i].vtxformat = SECONDUV;
					meshes[i].vtxsize = SZ_SECONDUV;
					numfloats = 10;
					if(datacount == 4)
					{
						meshes[i].vtxformat = SECONDUVCOLOR;
						meshes[i].vtxsize = SZ_SECONDUVCOLOR;
						numfloats = 14;
					}
				}else if(datacount == 4)
				{
					meshes[i].vtxformat = COLOR;
					meshes[i].vtxsize = SZ_COLOR;
					numfloats = 12;
				}				
			}
			if(hastangent == 1)
				numfloats += 4;
			
			//Vertexdata
			meshes[i].vertexnum = numverts;
			meshes[i].vertices = (sgVertex*)malloc(meshes[i].vtxsize*numverts);
			
			if((flags & GEN_TANGENT) == 0)
			{
				fread(meshes[i].vertices, 4, numfloats*numverts, file);
			}else
			{
				float *vertdatasrc = new float[numfloats*numverts];
				fread(vertdatasrc, 4, numfloats*numverts, file);
				sgVertex *vertdatatarget = meshes[i].vertices;
				
				if(meshes[i].vtxformat == TANGENT)
				{
					for(int n = 0; n < numverts; n++)
					{
						memcpy(&((sgVertexTan*)vertdatatarget)[n].position, &vertdatasrc[n*numfloats], sizeof(float)*numfloats);
					}
				}else if(meshes[i].vtxformat == TANGENTSECONDUV)
				{
					for(int n = 0; n < numverts; n++)
					{
						memcpy(&((sgVertexTanUV*)vertdatatarget)[n].position, &vertdatasrc[n*numfloats], sizeof(float)*numfloats);
					}
				}else if(meshes[i].vtxformat == TANGENTCOLOR)
				{
					for(int n = 0; n < numverts; n++)
					{
						memcpy(&((sgVertexTanCol*)vertdatatarget)[n].position, &vertdatasrc[n*numfloats], sizeof(float)*numfloats);
					}
				}else if(meshes[i].vtxformat == TANGENTSECONDUVCOLOR)
				{
					for(int n = 0; n < numverts; n++)
					{
						memcpy(&((sgVertexTanUVCol*)vertdatatarget)[n].position, &vertdatasrc[n*numfloats], sizeof(float)*numfloats);
					}
				}
			}
			
			//Indices
			fread(&meshes[i].indexnum, 2, 1, file);
			meshes[i].indices = new unsigned short[meshes[i].indexnum];
			fread(meshes[i].indices, 2, meshes[i].indexnum, file);
		}
		fclose(file);
		
		for(int meshnum = 0; meshnum < countmeshs; meshnum++)
		{
			sgMesh *mesh_ = new sgMesh;
			
			mesh_->vtxform = meshes[meshnum].vtxformat;
			mesh_->vtxsize = meshes[meshnum].vtxsize;
			
			mesh_->vertexnum = meshes[meshnum].vertexnum;
			mesh_->vertices = meshes[meshnum].vertices;
			
			mesh_->indexnum = meshes[meshnum].indexnum;
			mesh_->indices = meshes[meshnum].indices;

//			mesh_->calculateNormals();
			if((flags & GEN_TANGENT) > 0)
				mesh_->calculateTangents();
			mesh_->generateVBO();
			obj->meshs.push_back(mesh_);
			
			sgResourceManager::addResource(mesh_);
			
			if(meshes[meshnum].material->texnames.size() > 0)
			{
				obj->materials.push_back(sgMaterial::getMaterial(meshes[meshnum].material->texnames[0].c_str()));
				for(int tex = 1; tex < meshes[meshnum].material->texnames.size(); tex++)
				{
					obj->materials[obj->materials.size()-1]->setTexture2D(-1, meshes[meshnum].material->texnames[tex].c_str());
				}
			}else
			{
				obj->materials.push_back(sgMaterial::getMaterial());
			}
		}
		
		delete[] meshes;
		delete[] materials;
		
		return true;
	}
}
