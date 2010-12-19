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

namespace sgObjectFiles
{
	bool loadSGM(sgObjectContainer *obj, const char *filename)
	{
		sgXML *tbxml = new sgXML(filename);
		TBXMLElement *root = tbxml->rootXMLElement;
		
		std::vector<sgSGMMaterial*> materials;
		std::vector<sgSGMMesh*> meshes;
		
		//Get materials
		TBXMLElement *mats = sgXML::childElement("material", root);
		while(mats != NULL)
		{
			materials.push_back(new sgSGMMaterial);
			materials[materials.size()-1]->id_ = atoi(sgXML::valueOfAttribute("id", mats));
			TBXMLElement *texs = sgXML::childElement("textures", mats);
			char *texstr = (char*)sgXML::textForElement(texs);
			char *texlist = strtok(texstr, " ");
			while(texlist != NULL)
			{
				materials[materials.size()-1]->texnames.push_back(std::string(texlist));
				texlist = strtok(NULL, " ");
			}
			mats = sgXML::nextSibling("material", mats);
		}
		
		//Get meshes
		TBXMLElement *meshs = sgXML::childElement("mesh", root);
		while(meshs != NULL)
		{
			//Header
			meshes.push_back(new sgSGMMesh);
			meshes[meshes.size()-1]->id_ = atoi(sgXML::valueOfAttribute("id", meshs));
			unsigned int matid = atoi(sgXML::valueOfAttribute("material", meshs));
//			unsigned int uvcount = [[TBXML valueOfAttributeNamed:@"texcoordcount" forElement:meshs] intValue];
			for(int i = 0; i < materials.size(); i++)
			{
				if(materials[i]->id_ == matid)
				{
					meshes[meshes.size()-1]->material = materials[i];
					break;
				}
			}
			
			//Positions
			TBXMLElement *vpos = sgXML::childElement("vertexpos", meshs);
			char *vposstr = (char*)sgXML::textForElement(vpos);
			char *vposlist = strtok(vposstr, " ");
			sgVertex vert;
			while(vposlist != NULL)
			{
				vert.position.x = atof(vposlist);
				vposlist = strtok(NULL, " ");
				vert.position.y = atof(vposlist);
				vposlist = strtok(NULL, " ");
				vert.position.z = atof(vposlist);
				vposlist = strtok(NULL, " ");
				meshes[meshes.size()-1]->vertices.push_back(vert);
			}
			
			//Texcoords
			TBXMLElement *vuv = sgXML::childElement("vertextexcoord", meshs);
			char *vuvstr = (char*)sgXML::textForElement(vuv);
			char *vuvlist = strtok(vuvstr, " ");
			for(int i = 0; i < meshes[meshes.size()-1]->vertices.size(); i++)
			{
				meshes[meshes.size()-1]->vertices[i].uv.x = atof(vuvlist);
				vuvlist = strtok(NULL, " ");
				meshes[meshes.size()-1]->vertices[i].uv.y = atof(vuvlist);
				vuvlist = strtok(NULL, " ");
			}
			
			//Indices
			TBXMLElement *vind = sgXML::childElement("indices", meshs);
			char *vindstr = (char*)sgXML::textForElement(vind);
			char *vindlist = strtok(vindstr, " ");
			while(vindlist != NULL)
			{
				meshes[meshes.size()-1]->indices.push_back(atoi(vindlist));
				vindlist = strtok(NULL, " ");
			}
			
/*			//Bones
			TBXMLElement *vbone = [TBXML childElementNamed:@"bone" parentElement:meshs];
			while(vbone != NULL)
			{
				sgBone bone;
				bone.keepdata = TRUE;
				bone.name = std::string([[TBXML valueOfAttributeNamed:@"name" forElement:vbone] UTF8String]);
				TBXMLElement *vbel = [TBXML childElementNamed:@"matrix" parentElement:vbone];
				float mat[16];
				NSString *vbelstr = [TBXML textForElement:vbel];
				NSArray *vbellist = [vbelstr componentsSeparatedByString:@" "];
				for(int i = 0; i < [vbellist count]; i++)
				{
					mat[i] = [[vbellist objectAtIndex:i] floatValue];
				}
				bone.matbase.setArray(mat);
				
				vbel = [TBXML childElementNamed:@"vertsweights" parentElement:vbone];
				vbelstr = [TBXML textForElement:vbel];
				vbellist = [vbelstr componentsSeparatedByString:@" "];
				bone.indnum = [vbellist count]*0.5;
				bone.indices = new unsigned int[bone.indnum];
				bone.weights = new float[bone.indnum];
				for(int i = 0; i < [vbellist count]; i += 2)
				{
					bone.indices[i/2] = [[vbellist objectAtIndex:i+0] intValue];
					bone.weights[i/2] = [[vbellist objectAtIndex:i+1] floatValue];
				}
				
				meshes[meshes.size()-1]->bones.push_back(bone);
				vbone = [TBXML nextSiblingNamed:@"bone" searchFromElement:vbone];
			}*/
			
			meshs = sgXML::nextSibling("mesh", meshs);
		}
		
		//Get animations
/*		TBXMLElement *vanm = [TBXML childElementNamed:@"animation" parentElement:root];
		while(vanm != NULL)
		{
			TBXMLElement *vael = [TBXML childElementNamed:@"bone" parentElement:vanm];
			while(vael != NULL)
			{
				sgBoneAnimation *animation = new sgBoneAnimation;
				sgRessourceManager::addRessource(animation);
				
				animation->name = std::string([[TBXML valueOfAttributeNamed:@"name" forElement:vanm] UTF8String]);
				animation->duration = [[TBXML valueOfAttributeNamed:@"duration" forElement:vanm] floatValue];
				
				for(int i = 0; i < meshes.size(); i++)
				{
					unsigned int *ptr = new unsigned int[2];
					for(int n = 0; n < meshes[i]->bones.size(); n++)
					{
						if(meshes[i]->bones[n].name.compare(std::string([[TBXML valueOfAttributeNamed:@"name" forElement:vael] UTF8String])) == 0)
						{
							meshes[i]->bones[n].animations.push_back(animation);
							ptr[0] = n;
							ptr[1] = meshes[i]->bones[n].animations.size()-1;
							meshes[i]->animations.insert(std::pair<std::string, unsigned int*>(animation->name, ptr));
							break;
						}
					}
				}
				
				TBXMLElement *vabel = [TBXML childElementNamed:@"positions" parentElement:vael];
				NSString *vabelstr = [TBXML textForElement:vabel];
				NSArray *vabellist = [vabelstr componentsSeparatedByString:@" "];
				animation->posnum = [vabellist count]/4;
				animation->postimes = new float[animation->posnum];
				animation->positions = new sgVector3[animation->posnum];
				for(int i = 0; i < [vabellist count]; i += 4)
				{
					animation->postimes[i/4] = [[vabellist objectAtIndex:i+0] floatValue];
					animation->positions[i/4].x = [[vabellist objectAtIndex:i+1] floatValue];
					animation->positions[i/4].y = [[vabellist objectAtIndex:i+2] floatValue];
					animation->positions[i/4].z = [[vabellist objectAtIndex:i+3] floatValue];
				}
				
				vabel = [TBXML childElementNamed:@"scales" parentElement:vael];
				vabelstr = [TBXML textForElement:vabel];
				vabellist = [vabelstr componentsSeparatedByString:@" "];
				animation->scalenum = [vabellist count]/4;
				animation->scaletimes = new float[animation->scalenum];
				animation->scales = new sgVector3[animation->scalenum];
				for(int i = 0; i < [vabellist count]; i += 4)
				{
					animation->scaletimes[i/4] = [[vabellist objectAtIndex:i+0] floatValue];
					animation->scales[i/4].x = [[vabellist objectAtIndex:i+1] floatValue];
					animation->scales[i/4].y = [[vabellist objectAtIndex:i+2] floatValue];
					animation->scales[i/4].z = [[vabellist objectAtIndex:i+3] floatValue];
				}
				
				vabel = [TBXML childElementNamed:@"rotations" parentElement:vael];
				vabelstr = [TBXML textForElement:vabel];
				vabellist = [vabelstr componentsSeparatedByString:@" "];
				animation->rotnum = [vabellist count]/5;
				animation->rottimes = new float[animation->rotnum];
				animation->rotations = new sgQuaternion[animation->rotnum];
				for(int i = 0; i < [vabellist count]; i += 5)
				{
					animation->rottimes[i/5] = [[vabellist objectAtIndex:i+0] floatValue];
					animation->rotations[i/5].w = [[vabellist objectAtIndex:i+1] floatValue];
					animation->rotations[i/5].x = [[vabellist objectAtIndex:i+2] floatValue];
					animation->rotations[i/5].y = [[vabellist objectAtIndex:i+3] floatValue];
					animation->rotations[i/5].z = [[vabellist objectAtIndex:i+4] floatValue];
				}
				
				vael = [TBXML nextSiblingNamed:@"bone" searchFromElement:vael];
			}
			vanm = [TBXML nextSiblingNamed:@"animation" searchFromElement:vanm];
		}*/
		
		delete tbxml;
		
		for(int meshnum = 0; meshnum < meshes.size(); meshnum++)
		{
			sgMesh *mesh_ = new sgMesh;
			
			mesh_->vtxform = BASIC;
			mesh_->vertexnum = meshes[meshnum]->vertices.size();
			mesh_->indexnum = meshes[meshnum]->indices.size();
			
			mesh_->vertices = new sgVertex[mesh_->vertexnum];
			mesh_->indices = new unsigned short[mesh_->indexnum];

			memcpy(mesh_->vertices, &meshes[meshnum]->vertices[0], mesh_->vertexnum*sizeof(sgVertex));
			memcpy(mesh_->indices, &meshes[meshnum]->indices[0], mesh_->indexnum*sizeof(unsigned short));
			
/*			mesh_->bonenum = meshes[meshnum]->bones.size();
			if(mesh_->bonenum > 0)
			{
				mesh_->bones = new sgBone[mesh_->bonenum];
				
				for(int i = 0; i < mesh_->bonenum; i++)
				{
					mesh_->bones[i].name = meshes[meshnum]->bones[i].name;
					mesh_->bones[i].indnum = meshes[meshnum]->bones[i].indnum;
					mesh_->bones[i].indices = meshes[meshnum]->bones[i].indices;
					mesh_->bones[i].weights = meshes[meshnum]->bones[i].weights;
					mesh_->bones[i].matbase = meshes[meshnum]->bones[i].matbase;
					
					for(int n = 0; n < meshes[meshnum]->bones[i].animations.size(); n++)
					{
						mesh_->bones[i].animations.push_back(meshes[meshnum]->bones[i].animations[n]);
					}
				}
				
				mesh_->animations = meshes[meshnum]->animations;
				mesh_->prepareAnimation();
			}*/

			mesh_->calculateNormals();
			mesh_->generateVBO();
			obj->meshs.push_back(mesh_);
			
			sgResourceManager::addResource(mesh_);
			
			if(meshes[meshnum]->material->texnames.size() > 0)
			{
				obj->materials.push_back(sgMaterial::getMaterial(meshes[meshnum]->material->texnames[0].c_str()));
			}else
			{
				obj->materials.push_back(sgMaterial::getMaterial());
			}
		}
		
		for(int i = 0; i < meshes.size(); i++)
		{
			delete meshes[i];
		}
		
		for(int i = 0; i < materials.size(); i++)
		{
			delete materials[i];
		}
		
		return true;
	}
}
