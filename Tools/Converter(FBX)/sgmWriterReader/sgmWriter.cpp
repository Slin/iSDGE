/**************************************************************************************

 Copyright (C) 2001 - 2009 Autodesk, Inc. and/or its licensors.
 All Rights Reserved.

 The coded instructions, statements, computer programs, and/or related material 
 (collectively the "Data") in these files contain unpublished information 
 proprietary to Autodesk, Inc. and/or its licensors, which is protected by 
 Canada and United States of America federal copyright law and by international 
 treaties. 
 
 The Data may not be disclosed or distributed to third parties, in whole or in
 part, without the prior written consent of Autodesk, Inc. ("Autodesk").

 THE DATA IS PROVIDED "AS IS" AND WITHOUT WARRANTY.
 ALL WARRANTIES ARE EXPRESSLY EXCLUDED AND DISCLAIMED. AUTODESK MAKES NO
 WARRANTY OF ANY KIND WITH RESPECT TO THE DATA, EXPRESS, IMPLIED OR ARISING
 BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED WARRANTIES OF TITLE, 
 NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR USE. 
 WITHOUT LIMITING THE FOREGOING, AUTODESK DOES NOT WARRANT THAT THE OPERATION
 OF THE DATA WILL BE UNINTERRUPTED OR ERROR FREE. 
 
 IN NO EVENT SHALL AUTODESK, ITS AFFILIATES, PARENT COMPANIES, LICENSORS
 OR SUPPLIERS ("AUTODESK GROUP") BE LIABLE FOR ANY LOSSES, DAMAGES OR EXPENSES
 OF ANY KIND (INCLUDING WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER
 SPECIAL, DIRECT, INDIRECT, EXEMPLARY, INCIDENTAL, LOSS OF PROFITS, REVENUE
 OR DATA, COST OF COVER OR CONSEQUENTIAL LOSSES OR DAMAGES OF ANY KIND),
 HOWEVER CAUSED, AND REGARDLESS OF THE THEORY OF LIABILITY, WHETHER DERIVED
 FROM CONTRACT, TORT (INCLUDING, BUT NOT LIMITED TO, NEGLIGENCE), OR OTHERWISE,
 ARISING OUT OF OR RELATING TO THE DATA OR ITS USE OR ANY OTHER PERFORMANCE,
 WHETHER OR NOT AUTODESK HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS
 OR DAMAGE. 

**************************************************************************************/
#include "sgmWriter.h"
#include <fbxfilesdk/fbxfilesdk_nsuse.h>
#include "tinyxml.h"

sgmWriter::sgmWriter(KFbxSdkManager &pFbxSdkManager, int pID):
KFbxWriter(pFbxSdkManager, pID),
mManager(&pFbxSdkManager)
{

}

sgmWriter::~sgmWriter()
{
    FileClose();
}

// Create a file stream with pFileName
bool sgmWriter::FileCreate(char* pFileName)
{
	filename = pFileName;
/*    if(mFilePointer != NULL)
    {
        FileClose();
    }
    mFilePointer = fopen(pFileName,"w");
    if(mFilePointer == NULL)
    {
        return false;
    }*/
    return true;
}

// Close the file stream
bool sgmWriter::FileClose()
{
	return true;
/*    if(mFilePointer != NULL)
    {
        fclose(mFilePointer);
        return true;
    }
    return false;*/
}

// Check whether the file stream is open.
bool sgmWriter::IsFileOpen()
{
	return true;
/*    if(mFilePointer != NULL)
        return true;
    return false;*/
}

// Get the file stream options
void sgmWriter::GetWriteOptions()
{
}

// Write file with stream options
bool sgmWriter::Write(KFbxDocument* pDocument)
{
    if (!pDocument)
    {
        GetError().SetLastErrorID(eINVALID_DOCUMENT_HANDLE);
        return false;
    }

    KFbxScene* lScene = KFbxCast<KFbxScene>(pDocument);
    bool lIsAScene = (lScene != NULL);
    bool lResult = false;

    if(lIsAScene)
    {
        PreprocessScene(*lScene);

 /*       KFbxNode* lRootNode = lScene->GetRootNode();
        PrintHierarchy(lRootNode);*/
		WriteFile();
		
        PostprocessScene(*lScene);
        lResult = true;        
    }
    return lResult;
}

// Write out Node Hierarchy recursively
void sgmWriter::PrintHierarchy(KFbxNode* pStartNode)
{
/*    KFbxNode* lChildNode;
    char const* lParentName = pStartNode->GetName();
    for(int i = 0; i<pStartNode->GetChildCount(); i++)
    {
        lChildNode = pStartNode->GetChild(i);
        char const* lChildName = lChildNode->GetName();
        fprintf(mFilePointer,"%s%s%s%s%s%s%s","\"",lChildName,"\"",", parent is ","\"",lParentName,"\"\n");
    }

    int lNodeChildCount = pStartNode->GetChildCount ();
    while (lNodeChildCount > 0)
    {
        lNodeChildCount--;
        lChildNode = pStartNode->GetChild (lNodeChildCount);
        PrintHierarchy(lChildNode);        
    }*/
}

// Pre-process the scene before write it out 
bool sgmWriter::PreprocessScene(KFbxScene &pScene)
{
 //   printf("I'm in pre-process\n");
    return true;
}

// Post-process the scene after write it out 
bool sgmWriter::PostprocessScene(KFbxScene &pScene)
{
 //   printf("I'm in post process\n");
    return true;
}

void sgmWriter::WriteFile()
{
	printf("Create document and header.\n");
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	
	TiXmlElement *object = new TiXmlElement("environment");
	doc.LinkEndChild(object);
	
	//Material
	printf("Create materials.\n");
/*	for(int ms = 0; ms < scene->mNumMaterials; ms++)
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
	}*/
	
	printf("Write file.\n");
	doc.SaveFile(filename);
}
