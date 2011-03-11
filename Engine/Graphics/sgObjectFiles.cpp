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
			const char *datacountstr = sgXML::valueOfAttribute("datachannels", meshs);
			unsigned int datacount = 0;
			if(datacountstr != NULL)
				datacount = atoi(datacountstr);
			for(int i = 0; i < materials.size(); i++)
			{
				if(materials[i]->id_ == matid)
				{
					meshes[meshes.size()-1]->material = materials[i];
					break;
				}
			}
			
			if((flags & GEN_TANGENT) > 0)
			{
				meshes[meshes.size()-1]->vtxformat = TANGENT;
				meshes[meshes.size()-1]->vtxsize = SZ_TANGENT;
				if(uvcount > 1)
				{
					meshes[meshes.size()-1]->vtxformat = TANGENTSECONDUV;
					meshes[meshes.size()-1]->vtxsize = SZ_TANGENTSECONDUV;
					if(datacount == 4)
					{
						meshes[meshes.size()-1]->vtxformat = TANGENTSECONDUVCOLOR;
						meshes[meshes.size()-1]->vtxsize = SZ_TANGENTSECONDUVCOLOR;
					}
				}else if(datacount == 4)
				{
					meshes[meshes.size()-1]->vtxformat = TANGENTCOLOR;
					meshes[meshes.size()-1]->vtxsize = SZ_TANGENTCOLOR;
				}
			}else
			{
				meshes[meshes.size()-1]->vtxformat = BASIC;
				meshes[meshes.size()-1]->vtxsize = SZ_BASIC;
				if(uvcount > 1)
				{
					meshes[meshes.size()-1]->vtxformat = SECONDUV;
					meshes[meshes.size()-1]->vtxsize = SZ_SECONDUV;
					if(datacount == 4)
					{
						meshes[meshes.size()-1]->vtxformat = SECONDUVCOLOR;
						meshes[meshes.size()-1]->vtxsize = SZ_SECONDUVCOLOR;
					}
				}else if(datacount == 4)
				{
					meshes[meshes.size()-1]->vtxformat = COLOR;
					meshes[meshes.size()-1]->vtxsize = SZ_COLOR;
				}				
			}
			
			//Vertexdata
/*			TBXMLElement *vdata = sgXML::childElement("vertexdata", meshs);
			unsigned int vertnum = atoi(sgXML::valueOfAttribute("vertnum", vdata));
			
			meshes[meshes.size()-1]->vertices = (sgVertex*)malloc(meshes[meshes.size()-1]->vtxsize*vertnum);
			unsigned char *vertdatatarget = meshes[meshes.size()-1]->vertices;
			for(int i = 0; i < vertnum; i++)
			{
				vertdatatarget[]
			}*/
			
			
			//Positions
			TBXMLElement *vpos = sgXML::childElement("vertexpos", meshs);
			char *vposstr = (char*)sgXML::textForElement(vpos);
			char *vposlist = strtok(vposstr, " ");
			sgVertex *vert = (sgVertex*)malloc(meshes[meshes.size()-1]->vtxsize);
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
				else if(meshes[meshes.size()-1]->vtxformat == COLOR)
					meshes[meshes.size()-1]->vertices_col.push_back(*((sgVertexCol*)vert));
				else if(meshes[meshes.size()-1]->vtxformat == SECONDUVCOLOR)
					meshes[meshes.size()-1]->vertices_uvcol.push_back(*((sgVertexUVCol*)vert));
				else if(meshes[meshes.size()-1]->vtxformat == TANGENT)
					meshes[meshes.size()-1]->vertices_tan.push_back(*((sgVertexTan*)vert));
				else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUV)
					meshes[meshes.size()-1]->vertices_tanuv.push_back(*((sgVertexTanUV*)vert));
				else if(meshes[meshes.size()-1]->vtxformat == TANGENTCOLOR)
					meshes[meshes.size()-1]->vertices_tancol.push_back(*((sgVertexTanCol*)vert));
				else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUVCOLOR)
					meshes[meshes.size()-1]->vertices_tanuvcol.push_back(*((sgVertexTanUVCol*)vert));
			}
			free(vert);
			
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
				}else if(meshes[meshes.size()-1]->vtxformat == COLOR)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_col.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_col[i].uv.x = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_col[i].uv.y = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == SECONDUVCOLOR)
				{
					if(uvset == 0)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_uvcol.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_uvcol[i].uv.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_uvcol[i].uv.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}else if(uvset == 1)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_uvcol.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_uvcol[i].uv2.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_uvcol[i].uv2.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}
				}else if(meshes[meshes.size()-1]->vtxformat == TANGENT)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_tan.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_tan[i].uv.x = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tan[i].uv.y = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUV)
				{
					if(uvset == 0)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_tanuv.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_tanuv[i].uv.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_tanuv[i].uv.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}else if(uvset == 1)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_tanuv.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_tanuv[i].uv2.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_tanuv[i].uv2.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}
				}else if(meshes[meshes.size()-1]->vtxformat == TANGENTCOLOR)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_tancol.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_tancol[i].uv.x = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tancol[i].uv.y = atof(vuvlist);
						vuvlist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUVCOLOR)
				{
					if(uvset == 0)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_tanuvcol.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_tanuvcol[i].uv.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_tanuvcol[i].uv.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}else if(uvset == 1)
					{
						for(int i = 0; i < meshes[meshes.size()-1]->vertices_tanuvcol.size(); i++)
						{
							meshes[meshes.size()-1]->vertices_tanuvcol[i].uv2.x = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
							meshes[meshes.size()-1]->vertices_tanuvcol[i].uv2.y = atof(vuvlist);
							vuvlist = strtok(NULL, " ");
						}
					}
				}
				vuv = sgXML::nextSibling("vertextexcoord", vuv);
			}
			
			//Colors
			if(datacount == 4)
			{
				TBXMLElement *vcol = sgXML::childElement("vertexdata", meshs);
				char *vcolstr = (char*)sgXML::textForElement(vcol);
				char *vcollist = strtok(vcolstr, " ");
				if(meshes[meshes.size()-1]->vtxformat == COLOR)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_col.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_col[i].color.x = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_col[i].color.y = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_col[i].color.z = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_col[i].color.w = atof(vcollist);
						vcollist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == SECONDUVCOLOR)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_uvcol.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_uvcol[i].color.x = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_uvcol[i].color.y = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_uvcol[i].color.z = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_uvcol[i].color.w = atof(vcollist);
						vcollist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == TANGENTCOLOR)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_tancol.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_tancol[i].color.x = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tancol[i].color.y = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tancol[i].color.z = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tancol[i].color.w = atof(vcollist);
						vcollist = strtok(NULL, " ");
					}
				}else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUVCOLOR)
				{
					for(int i = 0; i < meshes[meshes.size()-1]->vertices_tanuvcol.size(); i++)
					{
						meshes[meshes.size()-1]->vertices_tanuvcol[i].color.x = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tanuvcol[i].color.y = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tanuvcol[i].color.z = atof(vcollist);
						vcollist = strtok(NULL, " ");
						meshes[meshes.size()-1]->vertices_tanuvcol[i].color.w = atof(vcollist);
						vcollist = strtok(NULL, " ");
					}
				}
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
			mesh_->vtxsize = meshes[meshnum]->vtxsize;
			if(meshes[meshes.size()-1]->vtxformat == BASIC)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_.size();
				mesh_->vertices = new sgVertex[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == SECONDUV)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_uv.size();
				mesh_->vertices = (sgVertex*)new sgVertexUV[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_uv[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == COLOR)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_col.size();
				mesh_->vertices = (sgVertex*)new sgVertexCol[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_col[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == SECONDUVCOLOR)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_uvcol.size();
				mesh_->vertices = (sgVertex*)new sgVertexUVCol[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_uvcol[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == TANGENT)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_tan.size();
				mesh_->vertices = (sgVertex*)new sgVertexTan[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_tan[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUV)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_tanuv.size();
				mesh_->vertices = (sgVertex*)new sgVertexTanUV[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_tanuv[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == TANGENTCOLOR)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_tancol.size();
				mesh_->vertices = (sgVertex*)new sgVertexTanCol[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_tancol[0], mesh_->vertexnum*mesh_->vtxsize);
			}else if(meshes[meshes.size()-1]->vtxformat == TANGENTSECONDUVCOLOR)
			{
				mesh_->vertexnum = meshes[meshnum]->vertices_tanuvcol.size();
				mesh_->vertices = (sgVertex*)new sgVertexTanUVCol[mesh_->vertexnum];
				memcpy(mesh_->vertices, &meshes[meshnum]->vertices_tanuvcol[0], mesh_->vertexnum*mesh_->vtxsize);
			}
			mesh_->indexnum = meshes[meshnum]->indices.size();
			mesh_->indices = new unsigned short[mesh_->indexnum];
			memcpy(mesh_->indices, &meshes[meshnum]->indices[0], mesh_->indexnum*sizeof(unsigned short));

			mesh_->calculateNormals();
			if((flags & GEN_TANGENT) > 0)
				mesh_->calculateTangents();
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
