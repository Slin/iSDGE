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
			unsigned int uvcount = atoi(sgXML::valueOfAttribute("texcoordcount", meshs));
			for(int i = 0; i < materials.size(); i++)
			{
				if(materials[i]->id_ == matid)
				{
					meshes[meshes.size()-1]->material = materials[i];
					break;
				}
			}
			
			meshes[meshes.size()-1]->vtxformat = BASIC;
			if(uvcount > 1)
				meshes[meshes.size()-1]->vtxformat = SECONDUV;
			
			//Positions
			TBXMLElement *vpos = sgXML::childElement("vertexpos", meshs);
			char *vposstr = (char*)sgXML::textForElement(vpos);
			char *vposlist = strtok(vposstr, " ");
			sgVertex *vert = (sgVertex*)malloc(meshes[meshes.size()-1]->vtxformat);
			while(vposlist != NULL)
			{
				vert->position.x = atof(vposlist);
				vposlist = strtok(NULL, " ");
				vert->position.y = atof(vposlist);
				vposlist = strtok(NULL, " ");
				vert->position.z = atof(vposlist);
				vposlist = strtok(NULL, " ");
				if(meshes[meshes.size()-1]->vtxformat == BASIC)
					meshes[meshes.size()-1]->vertices_.push_back(*vert);
				else if(meshes[meshes.size()-1]->vtxformat == SECONDUV)
					meshes[meshes.size()-1]->vertices_uv.push_back(*((sgVertexUV*)vert));
			}
			
			//Texcoords
			TBXMLElement *vuv;
			char *vuvstr;
			char *vuvlist;
			vuv = sgXML::childElement("vertextexcoord", meshs);
			for(int uvset = 0; uvset < uvcount; uvset += 1)
			{
				vuvstr = (char*)sgXML::textForElement(vuv);
				vuvlist = strtok(vuvstr, " ");
				if(meshes[meshes.size()-1]->vtxformat == BASIC)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_[i].uv.x = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_[i].uv.y = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == SECONDUV)
				{
					if(uvset == 0)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_uv.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_uv[i].uv.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_uv[i].uv.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}else if(uvset == 1)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_uv.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_uv[i].uv2.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_uv[i].uv2.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}
				}
				vuv = sgXML::nextSibling("vertextexcoord", vuv);
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
			
			meshs = sgXML::nextSibling("mesh", meshs);
		}
		
		delete tbxml;
		
		for(int meshnum = 0; meshnum < meshes.size(); meshnum++)
		{
			sgMesh *mesh_ = new sgMesh;
			
			mesh_->vtxform = meshes[meshnum]->vtxformat;
			if(meshes[meshes.size()-1]->vtxformat == BASIC)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_.size();
				mesh_->vertices = new sgVertex[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_[0], mesh_->vertexnum*BASIC);
			}else if(meshes[meshes.size()-1]->vtxformat == SECONDUV)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_uv.size();
				mesh_->vertices = (sgVertex*)new sgVertexUV[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_uv[0], mesh_->vertexnum*SECONDUV);
			}
			mesh_->indexnum = meshes[meshnum]->indices.size();
			mesh_->indices = new unsigned short[mesh_->indexnum];
			memcpy(mesh_->indices, &meshes[meshnum]->indices[0], mesh_->indexnum*sizeof(unsigned short));

			mesh_->calculateNormals();
			mesh_->generateVBO();
			obj->meshs.push_back(mesh_);
			
			sgResourceManager::addResource(mesh_);
			
			if(meshes[meshnum]->material->texnames.size() > 0)
			{
				obj->materials.push_back(sgMaterial::getMaterial(meshes[meshnum]->material->texnames[0].c_str()));
				for(int tex = 1; tex < meshes[meshnum]->material->texnames.size(); tex++)
				{
					obj->materials[obj->materials.size()-1]->setTexture2D(-1, meshes[meshnum]->material->texnames[tex].c_str());
				}
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
