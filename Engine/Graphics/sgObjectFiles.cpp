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
			sgLog("Could not open file: %s", filename);
			return false;
		}
		
		unsigned char version_id;
		fread(&version_id, 1, 1, file);
		if(version_id != 1)
		{
			sgLog("The file format is out of date or not supported: %s", filename);
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
			unsigned char bonecount = 0;
			fread(&bonecount, 1, 1, file);
			
			meshes[i].vtxsize = 8;
			meshes[i].vtxfeatures = sgVertex::POSITION|sgVertex::NORMAL|sgVertex::UV0;
			if((flags & GEN_TANGENT) > 0 || hastangent == 1)
			{
				meshes[i].vtxfeatures |= sgVertex::TANGENT;
				meshes[i].vtxsize += 4;
			}
			if(uvcount > 1)
			{
				meshes[i].vtxfeatures |= sgVertex::UV1;
				meshes[i].vtxsize += 2;
			}
			if(datacount == 4)
			{
				meshes[i].vtxfeatures |= sgVertex::COLOR;
				meshes[i].vtxsize += 4;
			}
			if(bonecount > 0)
			{
				meshes[i].vtxfeatures |= sgVertex::BONES;
				meshes[i].vtxsize += 8;
				meshes[i].bonemapping = new unsigned short[bonecount];
				fread(meshes[i].bonemapping, 2, bonecount, file);
			}
			
			//Vertexdata
			meshes[i].vertexnum = numverts;
			meshes[i].vertices = (float*)malloc(meshes[i].vtxsize*numverts*sizeof(float));
			
			if((flags & GEN_TANGENT) == 0)
			{
				fread(meshes[i].vertices, 4, meshes[i].vtxsize*numverts, file);
			}else
			{
				float *vertdatasrc = new float[meshes[i].vtxsize*numverts];
				fread(vertdatasrc, 4, meshes[i].vtxsize*numverts, file);
				float *vertdatatarget = meshes[i].vertices;
				
				for(int n = 0; n < numverts; n++)
				{
					memcpy((vertdatatarget+meshes[i].vtxsize*n), &vertdatasrc[n*meshes[i].vtxsize], sizeof(float)*meshes[i].vtxsize);
				}
			}
			
			//Indices
			fread(&meshes[i].indexnum, 2, 1, file);
			meshes[i].indices = new unsigned short[meshes[i].indexnum];
			fread(meshes[i].indices, 2, meshes[i].indexnum, file);
		}
		
		//Animations
		unsigned char hasanimations = 0;
		fread(&hasanimations, 1, 1, file);
		if(hasanimations)
		{
			unsigned short lenanimfilename = 0;
			fread(&lenanimfilename, 2, 1, file);
			char *animfilename = new char[lenanimfilename];
			fread(animfilename, 1, lenanimfilename, file);
			printf("Animation filename: %s\n", animfilename);
			//meshesstd::string(animfilename);
		}
		
		fclose(file);
		
		for(int meshnum = 0; meshnum < countmeshs; meshnum++)
		{
			sgMesh *mesh_ = new sgMesh;
			
			mesh_->vtxfeatures = meshes[meshnum].vtxfeatures;
			mesh_->vtxsize = meshes[meshnum].vtxsize;
			
			mesh_->vertexnum = meshes[meshnum].vertexnum;
			mesh_->vertices = meshes[meshnum].vertices;
			
			mesh_->indexnum = meshes[meshnum].indexnum;
			mesh_->indices = meshes[meshnum].indices;

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
					obj->materials[obj->materials.size()-1]->setTexture(-1, meshes[meshnum].material->texnames[tex].c_str());
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
