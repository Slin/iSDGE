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

#if !defined __IOS__
	#include <cstdlib>
	#include <cstring>
#endif

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
			fread(&meshes[i].indexnum, 4, 1, file);
			meshes[i].indices = new unsigned short[meshes[i].indexnum];
			fread(meshes[i].indices, 2, meshes[i].indexnum, file);
		}

		//Animations
		unsigned char hasanimations = 0;
		fread(&hasanimations, 1, 1, file);
		char *animfilename = 0;
		if(hasanimations)
		{
			unsigned short lenanimfilename = 0;
			fread(&lenanimfilename, 2, 1, file);
			animfilename = new char[lenanimfilename];
			fread(animfilename, 1, lenanimfilename, file);
			printf("Animation filename: %s\n", animfilename);
			//meshesstd::string(animfilename);
		}

		fclose(file);
		
		//load skeleton from .sga file
		if(hasanimations)
		{
			filepath = sgResourceManager::getPath(animfilename);
			file = fopen(filepath, "rb");
			delete[] filepath;
			if(!file)
			{
				sgLog("Could not open file: %s", animfilename);
				return false;
			}
			
			unsigned long int magic;
			fread(&magic, 4, 1, file);
			if(magic != 383405658)
			{
				sgLog("The file format is not supported: %s", animfilename);
				return false;
			}
			
			unsigned char version_id;
			fread(&version_id, 1, 1, file);
			if(version_id != 1)
			{
				sgLog("The file format is out of date or not supported: %s", animfilename);
				return false;
			}
			
			unsigned short lenskeletonname;
			fread(&lenskeletonname, 2, 1, file);
			char *skeletonname = new char[lenskeletonname];
			fread(skeletonname, 1, lenskeletonname, file);
			delete[] skeletonname;
			
			obj->skeleton = new sgSkeleton();
			
			unsigned short numbones;
			fread(&numbones, 2, 1, file);
			
			for(int i = 0; i < numbones; i++)
			{
				unsigned short lenbonename;
				fread(&lenbonename, 2, 1, file);
				char *bonename = new char[lenbonename];
				fread(bonename, 1, lenbonename, file);
				
				sgVector3 bonepos;
				fread(&bonepos.x, 4, 3, file);
				
				unsigned char isroot;
				fread(&isroot, 1, 1, file);
				
				sgBone bone(bonepos, bonename, isroot);
				
				unsigned short numchildren;
				fread(&numchildren, 2, 1, file);
				
				unsigned short *children = new unsigned short[numchildren];
				for(int n = 0; n < numchildren; n++)
				{
					fread(&(children[n]), 2, 1, file);
					bone.tempchildren.push_back(children[n]);
				}
				delete[] children;
				delete[] bonename;
				
				obj->skeleton->bones.push_back(bone);
			}
			
			unsigned short numanims;
			fread(&numanims, 2, 1, file);
			for(int i = 0; i < numanims; i++)
			{
				unsigned short lenanimname;
				fread(&lenanimname, 2, 1, file);
				char *animname = new char[lenanimname];
				fread(animname, 1, lenanimname, file);
				sgAnimation *anim = new sgAnimation(animname);
				//todo: add filename to animname
				sgResourceManager::addResource(animname, anim);
				obj->skeleton->animations.insert(std::pair<std::string, sgAnimation*>(animname, anim));
				delete[] animname;
				
				unsigned short numanimbones;
				fread(&numanimbones, 2, 1, file);
				for(int n = 0; n < numanimbones; n++)
				{
					unsigned short boneid;
					fread(&boneid, 2, 1, file);
					
					sgAnimationBone *animbone = 0;
					
					unsigned long numframes;
					fread(&numframes, 4, 1, file);
					for(int k = 0; k < numframes; k++)
					{
						float frametime;
						fread(&frametime, 4, 1, file);
						
						sgVector3 framepos;
						fread(&framepos.x, 4, 3, file);
						
						sgVector3 framescal;
						fread(&framescal.x, 4, 3, file);
						
						sgQuaternion framerot;
						fread(&framerot.x, 4, 4, file);
						
						animbone = new sgAnimationBone(animbone, 0, frametime, framepos, framescal, framerot);
					}
					sgAnimationBone *lastbone = animbone;
					while(animbone->prevframe != 0)
					{
						animbone->prevframe->nextframe = animbone;
						animbone = animbone->prevframe;
					}
					animbone->prevframe = lastbone;
					lastbone->nextframe = animbone;
					anim->bones.insert(std::pair<int, sgAnimationBone*>(boneid, animbone));
				}
			}
			fclose(file);
			
			obj->skeleton->matrices = new float[16*obj->skeleton->bones.size()];
			for(int i = 0; i < obj->skeleton->bones.size(); i++)
			{
				for(int n = 0; n < obj->skeleton->bones[i].tempchildren.size(); n++)
				{
					obj->skeleton->bones[i].children.push_back(&(obj->skeleton->bones[obj->skeleton->bones[i].tempchildren[n]]));
				}
			}
		}

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
