/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */


// assimp include files. These three are usually needed.
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include <cstdio>

#include <iostream>
#include <sstream>
using namespace std;
#include "tinyxml.h"

bool DoTheSceneProcessing(const aiScene *scene, const char* oFile)
{
	if(scene->mNumMaterials == 0 || scene->mNumMeshes == 0)
	{
		return false;
	}
	
	char ttstr[100];
	
	aiMesh** rawmeshes = scene->mMeshes;
	aiMaterial** materials = scene->mMaterials;
	aiAnimation** animations = scene->mAnimations;
	
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement *object = new TiXmlElement("environment");
	doc.LinkEndChild(object);
	
	//Material
	for(int ms = 0; ms < scene->mNumMaterials; ms++)
	{
		TiXmlElement *material = new TiXmlElement("material");
		material->SetAttribute("id", ms);
		object->LinkEndChild(material);
	
		//Material textures
		TiXmlElement *textures = new TiXmlElement("textures");
		material->LinkEndChild(textures);
	
		std::string texnamestr;
		aiString ttstr;
		std::string str;
		for(int tm = 0; tm < materials[ms]->GetTextureCount(aiTextureType_DIFFUSE); tm++)
		{
			materials[ms]->GetTexture(aiTextureType_DIFFUSE, tm, &ttstr);
			str = std::string(ttstr.data);
			str = str.substr(str.find_last_of("/\\")+1);
			str = str.substr(0, str.find_last_of(".")+1);
			str += std::string("png");
			texnamestr += str;
			if(tm+1 < materials[ms]->GetTextureCount(aiTextureType_DIFFUSE))
			{
				texnamestr += std::string(" ");
			}
		}

		//Texture filename list
		TiXmlText *texnames = new TiXmlText(texnamestr.c_str());
		textures->LinkEndChild(texnames);
	}
	
	
	//Object
	for(int os = 0; os < scene->mNumMeshes; os++)
	{
		//Mesh
		TiXmlElement *mesh = new TiXmlElement("mesh");
		mesh->SetAttribute("id", os);
		mesh->SetAttribute("material", rawmeshes[os]->mMaterialIndex);
		mesh->SetAttribute("texcoordcount", rawmeshes[os]->GetNumUVChannels());
		object->LinkEndChild(mesh);
		
		//Vertex positions
		TiXmlElement *vpos = new TiXmlElement("vertexpos");
		mesh->LinkEndChild(vpos);
		std::string vposstr;
		for(int i = 0; i < rawmeshes[os]->mNumVertices; i++)
		{
			sprintf(ttstr, "%f ", rawmeshes[os]->mVertices[i].x);
			vposstr += std::string(ttstr);
			
			sprintf(ttstr, "%f ", rawmeshes[os]->mVertices[i].z);
			vposstr += std::string(ttstr);
			
			sprintf(ttstr, "%f ", rawmeshes[os]->mVertices[i].y);
			vposstr += std::string(ttstr);
		}
		TiXmlText *vpos_array = new TiXmlText(vposstr.c_str());
		vpos->LinkEndChild(vpos_array);
		
		for(int n = 0; n < rawmeshes[os]->GetNumUVChannels(); n++)
		{
			//Vertex uv
			TiXmlElement *vuv = new TiXmlElement("vertextexcoord");
			vuv->SetAttribute("id", n);
			mesh->LinkEndChild(vuv);
			vposstr.clear();
			for(int i = 0; i < rawmeshes[os]->mNumVertices; i++)
			{
				sprintf(ttstr, "%f ", rawmeshes[os]->mTextureCoords[n][i].x);
				vposstr += std::string(ttstr);
				
				sprintf(ttstr, "%f ", 1.0f-rawmeshes[os]->mTextureCoords[n][i].y);
				vposstr += std::string(ttstr);
			}
			TiXmlText *vuv_array = new TiXmlText(vposstr.c_str());
			vuv->LinkEndChild(vuv_array);
		}
		
		//Vertex indices
		TiXmlElement *vind = new TiXmlElement("indices");
		mesh->LinkEndChild(vind);
		vposstr.clear();
		for(int i = 0; i < rawmeshes[os]->mNumFaces; i++)
		{
			sprintf(ttstr, "%i ", rawmeshes[os]->mFaces[i].mIndices[2]);
			vposstr += std::string(ttstr);
			
			sprintf(ttstr, "%i ", rawmeshes[os]->mFaces[i].mIndices[1]);
			vposstr += std::string(ttstr);
			
			sprintf(ttstr, "%i ", rawmeshes[os]->mFaces[i].mIndices[0]);
			vposstr += std::string(ttstr);
		}
		TiXmlText *vind_array = new TiXmlText(vposstr.c_str());
		vind->LinkEndChild(vind_array);
		
		//Bones
		TiXmlElement *bone;
		TiXmlElement *element;
		for(int b = 0; b < rawmeshes[os]->mNumBones; b++)
		{
			bone = new TiXmlElement("bone");
			bone->SetAttribute("name", rawmeshes[os]->mBones[b]->mName.data);
			mesh->LinkEndChild(bone);
			
			element = new TiXmlElement("matrix");
			bone->LinkEndChild(element);
			vposstr.clear();
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.a1);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.b1);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.c1);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.d1);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.a2);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.b2);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.c2);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.d2);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.a3);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.b3);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.c3);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.d3);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.a4);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.b4);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.c4);
			vposstr += std::string(ttstr);
			sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mOffsetMatrix.d4);
			vposstr += std::string(ttstr);
			
			TiXmlText *bone_array = new TiXmlText(vposstr.c_str());
			element->LinkEndChild(bone_array);
			
			element = new TiXmlElement("vertsweights");
			bone->LinkEndChild(element);
			vposstr.clear();
			for(int i = 0; i < rawmeshes[os]->mBones[b]->mNumWeights; i++)
			{
				sprintf(ttstr, "%i ", rawmeshes[os]->mBones[b]->mWeights[i].mVertexId);
				vposstr += std::string(ttstr);
				
				sprintf(ttstr, "%f ", rawmeshes[os]->mBones[b]->mWeights[i].mWeight);
				vposstr += std::string(ttstr);
			}
			bone_array = new TiXmlText(vposstr.c_str());
			element->LinkEndChild(bone_array);
		}
	}
	
	//Animations
	for(int as = 0; as < scene->mNumAnimations; as++)
	{
		TiXmlElement *anm = new TiXmlElement("animation");
		anm->SetAttribute("name", animations[as]->mName.data);
		anm->SetAttribute("duration", animations[as]->mDuration);
		object->LinkEndChild(anm);
		
		//Single bones animation frames
		for(int i = 0; i < animations[as]->mNumChannels; i++)
		{
			TiXmlElement *bones = new TiXmlElement("bone");
			bones->SetAttribute("name", animations[as]->mChannels[i]->mNodeName.data);
			anm->LinkEndChild(bones);
			
			TiXmlElement *element = new TiXmlElement("positions");
			bones->LinkEndChild(element);
			std::string anmkey;
			for(int n = 0; n < animations[as]->mChannels[i]->mNumPositionKeys; n++)
			{
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mPositionKeys[n].mTime);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mPositionKeys[n].mValue.x);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mPositionKeys[n].mValue.z);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mPositionKeys[n].mValue.y);
				anmkey += std::string(ttstr);
			}
			
			TiXmlText *anm_array = new TiXmlText(anmkey.c_str());
			element->LinkEndChild(anm_array);
			
			
			element = new TiXmlElement("scales");
			bones->LinkEndChild(element);
			anmkey.clear();
			for(int n = 0; n < animations[as]->mChannels[i]->mNumScalingKeys; n++)
			{
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mScalingKeys[n].mTime);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mScalingKeys[n].mValue.x);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mScalingKeys[n].mValue.z);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mScalingKeys[n].mValue.y);
				anmkey += std::string(ttstr);
			}
			
			anm_array = new TiXmlText(anmkey.c_str());
			element->LinkEndChild(anm_array);
			
			element = new TiXmlElement("rotations");
			bones->LinkEndChild(element);
			anmkey.clear();
			for(int n = 0; n < animations[as]->mChannels[i]->mNumRotationKeys; n++)
			{
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mRotationKeys[n].mTime);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mRotationKeys[n].mValue.w);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mRotationKeys[n].mValue.x);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mRotationKeys[n].mValue.y);
				anmkey += std::string(ttstr);
				
				sprintf(ttstr, "%f ", animations[as]->mChannels[i]->mRotationKeys[n].mValue.z);
				anmkey += std::string(ttstr);
			}
			
			anm_array = new TiXmlText(anmkey.c_str());
			element->LinkEndChild(anm_array);
		}
	}
	
	doc.SaveFile(oFile);
	return true;
}

bool DoTheImportThing(const char* pFile, const char* oFile)
{
	const aiScene* scene = aiImportFile( pFile, aiProcess_RemoveRedundantMaterials|aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType|aiProcess_OptimizeMeshes|aiProcess_OptimizeGraph);
	if(!scene)
	{
		cout << "Couldn´t load file \n";
		return false;
	}
	
	if(!DoTheSceneProcessing(scene, oFile))
	{
		cout << "Couldn´t convert file \n";
		return false;
	}
	
	aiReleaseImport(scene);
	return true;
}

int main (int argc, char * const argv[])
{
	if(argc > 2)
		DoTheImportThing(argv[1], argv[2]);
    return 0;
}