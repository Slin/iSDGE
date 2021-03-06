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

#if defined __ANDROID__
	#include <zip.h>
#endif

namespace sgObjectFiles
{
#if defined __ANDROID__
	zip *ZIPArchive = NULL;
	void loadZIP(const char* path)
	{
		sgLog("Loading APK %s", path);
		ZIPArchive = zip_open(path, 0, NULL);
		if(ZIPArchive == NULL)
		{
			sgLog("Error loading APK");
			return;
		}

		//Just for debug, print APK contents
		int numFiles = zip_get_num_files(ZIPArchive);
		for(int i=0; i<numFiles; i++)
		{
			const char* name = zip_get_name(ZIPArchive, i, 0);
			if(name == NULL)
			{
				sgLog("Error reading zip file name at index %i : %s", zip_strerror(ZIPArchive));
				return;
			}
			sgLog("File %i : %s\n", i, name);
		}
	}
#endif

	bool loadSGM(sgObjectContainer *obj, const char *filename, unsigned long flags)
	{
		const char *filepath = sgResourceManager::getPath(filename);
#if defined __ANDROID__
		zip_file *file = zip_fopen(ZIPArchive, filepath, 0);
#else
		FILE *file = fopen(filepath, "rb");
#endif
		delete[] filepath;
		if(!file)
		{
			sgLog("Could not open file: %s", filename);
			return false;
		}
		
		unsigned long int magic;
#if defined __ANDROID__
		zip_fread(file, &magic, 4);
#else
		fread(&magic, 4, 1, file);
#endif
		if(magic != 352658064)
		{
			sgLog("The file format is not supported: %s", filename);
			return false;
		}

		unsigned char version_id;
#if defined __ANDROID__
		zip_fread(file, &version_id, 1);
#else
		fread(&version_id, 1, 1, file);
#endif
		if(version_id != 1)
		{
			sgLog("The file format is out of date or not supported: %s", filename);
			return false;
		}

		//Get materials
		unsigned char countmats = 0;
#if defined __ANDROID__
		zip_fread(file, &countmats, 1);
#else
		fread(&countmats, 1, 1, file);
#endif
		sgSGMMaterial* materials = new sgSGMMaterial[countmats];
		for(unsigned int i = 0; i < countmats; i++)
		{
#if defined __ANDROID__
			zip_fread(file, &materials[i].id_, 1);
#else
			fread(&materials[i].id_, 1, 1, file);
#endif
			unsigned char texcount = 0;
#if defined __ANDROID__
			zip_fread(file, &texcount, 1);
#else
			fread(&texcount, 1, 1, file);
#endif
			for(unsigned int n = 0; n < texcount; n++)
			{
				unsigned short lentexfilename;
#if defined __ANDROID__
				zip_fread(file, &lentexfilename, 2);
#else
				fread(&lentexfilename, 2, 1, file);
#endif
				char *texfilename = new char[lentexfilename];
#if defined __ANDROID__
				zip_fread(file, texfilename, lentexfilename);
#else
				fread(texfilename, 1, lentexfilename, file);
#endif
				materials[i].texnames.push_back(std::string(texfilename));
				delete[] texfilename;
			}
		}

		//Get meshes
		unsigned char countmeshs = 0;
#if defined __ANDROID__
		zip_fread(file, &countmeshs, 1);
#else
		fread(&countmeshs, 1, 1, file);
#endif
		sgSGMMesh* meshes = new sgSGMMesh[countmeshs];
		for(int i = 0; i < countmeshs; i++)
		{
#if defined __ANDROID__
			zip_fread(file, &meshes[i].id_, 1);
#else
			fread(&meshes[i].id_, 1, 1, file);
#endif
			unsigned char matid = 0;
#if defined __ANDROID__
			zip_fread(file, &matid, 1);
#else
			fread(&matid, 1, 1, file);
#endif

			for(unsigned int n = 0; n < countmats; n++)
			{
				if(materials[n].id_ == matid)
				{
					meshes[i].material = &materials[n];
					break;
				}
			}

			unsigned short numverts = 0;
#if defined __ANDROID__
			zip_fread(file, &numverts, 2);
#else
			fread(&numverts, 2, 1, file);
#endif
			unsigned char uvcount = 0;
#if defined __ANDROID__
			zip_fread(file, &uvcount, 1);
#else
			fread(&uvcount, 1, 1, file);
#endif
			unsigned char datacount = 0;
#if defined __ANDROID__
			zip_fread(file, &datacount, 1);
#else
			fread(&datacount, 1, 1, file);
#endif
			unsigned char hastangent = 0;
#if defined __ANDROID__
			zip_fread(file, &hastangent, 1);
#else
			fread(&hastangent, 1, 1, file);
#endif
			unsigned char hasbones = 0;
#if defined __ANDROID__
			zip_fread(file, &hasbones, 1);
#else
			fread(&hasbones, 1, 1, file);
#endif

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
			if(hasbones > 0)
			{
				meshes[i].vtxfeatures |= sgVertex::BONES;
				meshes[i].vtxsize += 8;
			}

			//Vertexdata
			meshes[i].vertexnum = numverts;
			meshes[i].vertices = (float*)malloc(meshes[i].vtxsize*numverts*sizeof(float));

			if((flags & GEN_TANGENT) == 0)
			{
#if defined __ANDROID__
				zip_fread(file, meshes[i].vertices, meshes[i].vtxsize*numverts*4);
#else
				fread(meshes[i].vertices, 4, meshes[i].vtxsize*numverts, file);
#endif
			}
			else
			{
				float *vertdatasrc = new float[meshes[i].vtxsize*numverts];
#if defined __ANDROID__
				zip_fread(file, vertdatasrc, meshes[i].vtxsize*numverts*4);
#else
				fread(vertdatasrc, 4, meshes[i].vtxsize*numverts, file);
#endif
				float *vertdatatarget = meshes[i].vertices;

				for(int n = 0; n < numverts; n++)
				{
					memcpy((vertdatatarget+meshes[i].vtxsize*n), &vertdatasrc[n*meshes[i].vtxsize], sizeof(float)*meshes[i].vtxsize);
				}
			}

			//Indices
#if defined __ANDROID__
			zip_fread(file, &meshes[i].indexnum, 4);
			zip_fread(file, &meshes[i].indexsize, 1);
#else
			fread(&meshes[i].indexnum, 4, 1, file);
			fread(&meshes[i].indexsize, 1, 1, file);
#endif

			if(meshes[i].indexsize == 2)
				meshes[i].indices = new unsigned short[meshes[i].indexnum];
			else
				meshes[i].indices = new unsigned long[meshes[i].indexnum];

#if defined __ANDROID__
			zip_fread(file, meshes[i].indices, meshes[i].indexnum*meshes[i].indexsize);
#else
			fread(meshes[i].indices, meshes[i].indexsize, meshes[i].indexnum, file);
#endif
		} //loop through meshs

		//Animations
		unsigned char hasanimations = 0;
#if defined __ANDROID__
		zip_fread(file, &hasanimations, 1);
#else
		fread(&hasanimations, 1, 1, file);
#endif
		
		char *animfilename = 0;
		if(hasanimations)
		{
			unsigned short lenanimfilename = 0;
#if defined __ANDROID__
			zip_fread(file, &lenanimfilename, 2);
#else
			fread(&lenanimfilename, 2, 1, file);
#endif
			animfilename = new char[lenanimfilename];
#if defined __ANDROID__
			zip_fread(file, animfilename, lenanimfilename);
#else
			fread(animfilename, 1, lenanimfilename, file);
#endif
		}

#if defined __ANDROID__
		zip_fclose(file);
#else
		fclose(file);
#endif

		obj->skeleton = NULL;
		//load skeleton from .sga file
		if(hasanimations)
		{
			filepath = sgResourceManager::getPath(animfilename);
			
#if defined __ANDROID__
			file = zip_fopen(ZIPArchive, filepath, 0);
#else
			file = fopen(filepath, "rb");
#endif
			delete[] filepath;
			if(!file)
			{
				sgLog("Could not open file: %s", animfilename);
				return false;
			}
			
			unsigned long int magic;
#if defined __ANDROID__
			zip_fread(file, &magic, 4);
#else
			fread(&magic, 4, 1, file);
#endif
			if(magic != 383405658)
			{
				sgLog("The file format is not supported: %s, %li", animfilename, magic);
				return false;
			}
			
			unsigned char version_id;
#if defined __ANDROID__
			zip_fread(file, &version_id, 1);
#else
			fread(&version_id, 1, 1, file);
#endif
			if(version_id != 1)
			{
				sgLog("The file format is out of date or not supported: %s", animfilename);
				return false;
			}
			
			unsigned short lenskeletonname;
#if defined __ANDROID__
			zip_fread(file, &lenskeletonname, 2);
#else
			fread(&lenskeletonname, 2, 1, file);
#endif
			char *skeletonname = new char[lenskeletonname];
#if defined __ANDROID__
			zip_fread(file, skeletonname, lenskeletonname);
#else
			fread(skeletonname, 1, lenskeletonname, file);
#endif
			delete[] skeletonname;
			
			obj->skeleton = new sgSkeleton();
			sgResourceManager::addResource(obj->skeleton);
			
			unsigned short numbones;
#if defined __ANDROID__
			zip_fread(file, &numbones, 2);
#else
			fread(&numbones, 2, 1, file);
#endif
			
			for(int i = 0; i < numbones; i++)
			{
				unsigned short lenbonename;
#if defined __ANDROID__
				zip_fread(file, &lenbonename, 2);
#else
				fread(&lenbonename, 2, 1, file);
#endif
				char *bonename = new char[lenbonename];
#if defined __ANDROID__
				zip_fread(file, bonename, lenbonename);
#else
				fread(bonename, 1, lenbonename, file);
#endif

				sgVector3 bonepos;
#if defined __ANDROID__
				zip_fread(file, &bonepos.x, 4*3);
#else
				fread(&bonepos.x, 4, 3, file);
#endif
				
				unsigned char isroot;
#if defined __ANDROID__
				zip_fread(file, &isroot, 1);
#else
				fread(&isroot, 1, 1, file);
#endif
				
				sgBone bone(bonepos, bonename, isroot);
				
				unsigned short numchildren;
#if defined __ANDROID__
				zip_fread(file, &numchildren, 2);
#else
				fread(&numchildren, 2, 1, file);
#endif
				
				unsigned short *children = new unsigned short[numchildren];
				for(int n = 0; n < numchildren; n++)
				{
#if defined __ANDROID__
					zip_fread(file, &(children[n]), 2);
#else
					fread(&(children[n]), 2, 1, file);
#endif
					bone.tempchildren.push_back(children[n]);
				}
				delete[] children;
				delete[] bonename;
				
				obj->skeleton->bones.push_back(bone);
			}
			
			unsigned short numanims;
#if defined __ANDROID__
			zip_fread(file, &numanims, 2);
#else
			fread(&numanims, 2, 1, file);
#endif
			
			for(int i = 0; i < numanims; i++)
			{
				unsigned short lenanimname;
#if defined __ANDROID__
				zip_fread(file, &lenanimname, 2);
#else
				fread(&lenanimname, 2, 1, file);
#endif
				
				char *animname = new char[lenanimname];
#if defined __ANDROID__
				zip_fread(file, animname, lenanimname);
#else
				fread(animname, 1, lenanimname, file);
#endif
				
				sgAnimation *anim = new sgAnimation(animname);
				std::string resname = std::string(animfilename)+std::string(animname);
				sgResourceManager::addResource(resname.c_str(), anim);
				obj->skeleton->animations.insert(std::pair<std::string, sgAnimation*>(animname, anim));
				delete[] animname;
				
				unsigned short numanimbones;
#if defined __ANDROID__
				zip_fread(file, &numanimbones, 2);
#else
				fread(&numanimbones, 2, 1, file);
#endif
				
				for(int n = 0; n < numanimbones; n++)
				{
					unsigned short boneid;
#if defined __ANDROID__
					zip_fread(file, &boneid, 2);
#else
					fread(&boneid, 2, 1, file);
#endif
					
					sgAnimationBone *animbone = 0;
					
					unsigned long numframes;
#if defined __ANDROID__
					zip_fread(file, &numframes, 4);
#else
					fread(&numframes, 4, 1, file);
#endif
					
					for(int k = 0; k < numframes; k++)
					{
						float frametime;
#if defined __ANDROID__
						zip_fread(file, &frametime, 4);
#else
						fread(&frametime, 4, 1, file);
#endif
						
						sgVector3 framepos;
#if defined __ANDROID__
						zip_fread(file, &framepos.x, 4*3);
#else
						fread(&framepos.x, 4, 3, file);
#endif
						
						sgVector3 framescal;
#if defined __ANDROID__
						zip_fread(file, &framescal.x, 4*3);
#else
						fread(&framescal.x, 4, 3, file);
#endif
						
						sgQuaternion framerot;
#if defined __ANDROID__
						zip_fread(file, &framerot.x, 4*4);
#else
						fread(&framerot.x, 4, 4, file);
#endif
						
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
#if defined __ANDROID__
			zip_fclose(file);
#else
			fclose(file);
#endif
			
			obj->skeleton->matrices = NULL;
		}


		for(int meshnum = 0; meshnum < countmeshs; meshnum++)
		{
			sgMesh *mesh_ = new sgMesh;

			mesh_->vtxfeatures = meshes[meshnum].vtxfeatures;
			mesh_->vtxsize = meshes[meshnum].vtxsize;

			mesh_->vertexnum = meshes[meshnum].vertexnum;
			mesh_->vertices = meshes[meshnum].vertices;

			mesh_->indexnum = meshes[meshnum].indexnum;
			mesh_->indexsize = meshes[meshnum].indexsize;
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
