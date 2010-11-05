/**************************************************************************************

 Copyright (C) 2001 - 2010 Autodesk, Inc. and/or its licensors.
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
#include "sgmReader.h"

#include <fbxfilesdk/fbxfilesdk_nsuse.h>

sgmReader::sgmReader(KFbxSdkManager &pFbxSdkManager, int pID):
KFbxReader(pFbxSdkManager, pID),
mFilePointer(NULL),
mManager(&pFbxSdkManager)
{
}

sgmReader::~sgmReader()
{
    FileClose();
}

void sgmReader::GetVersion(int& pMajor, int& pMinor, int& pRevision) const

{
    pMajor = 1;
    pMinor = 0;
    pRevision=0;
}

bool sgmReader::FileOpen(char* pFileName)
{
	printf("*.sgm is currently only available for export!");
	return false;
	
/*    if(mFilePointer != NULL)
        FileClose();
    mFilePointer = fopen(pFileName, "r");
    if(mFilePointer == NULL)
        return false;
    return true;*/
}
bool sgmReader::FileClose()
{
    if(mFilePointer!=NULL)
        fclose(mFilePointer);
    return true;
    
}
bool sgmReader::IsFileOpen()
{
    if(mFilePointer != NULL)
        return true;
    return false;
}

bool sgmReader::GetReadOptions(bool pParseFileAsNeeded)
{
    return true;
}

//Read the custom file and reconstruct node hierarchy.
bool sgmReader::Read(KFbxDocument* pDocument)
{
    if (!pDocument)
    {
        GetError().SetLastErrorID(eINVALID_DOCUMENT_HANDLE);
        return false;
    }
    KFbxScene*      lScene = KFbxCast<KFbxScene>(pDocument);
    bool            lIsAScene = (lScene != NULL);
    bool            lResult = false;

    if(lIsAScene)
    {
        KFbxNode* lRootNode = lScene->GetRootNode();
        KFbxNodeAttribute * lRootNodeAttribute = KFbxNull::Create(lScene,"");
        lRootNode->SetNodeAttribute(lRootNodeAttribute);

        int lSize;
        char* lBuffer = NULL;    
        if(mFilePointer != NULL)
        {
            //To obtain file size
            fseek (mFilePointer , 0 , SEEK_END);
            lSize = ftell (mFilePointer);
            rewind (mFilePointer);

            //Read file content to a string.
            lBuffer = (char*) malloc (sizeof(char)*lSize + 1);
            size_t lRead = fread(lBuffer, 1, lSize, mFilePointer);
            lBuffer[lRead]='\0';
            KString lString(lBuffer);

            //Parse the string to get name and relation of Nodes. 
            KString lSubString, lChildName, lParentName;
            KFbxNode* lChildNode;
            KFbxNode* lParentNode;
            KFbxNodeAttribute* lChildAttribute;
            int lEndTokenCount = lString.GetTokenCount("\n");

            for (int i = 0; i < lEndTokenCount; i++)
            {
                lSubString = lString.GetToken(i, "\n");
                KString lNodeString;
                lChildName = lSubString.GetToken(0, "\"");
                lParentName = lSubString.GetToken(2, "\"");

                //Build node hierarchy.
                if(lParentName == "RootNode")
                {
                    lChildNode = KFbxNode::Create(lScene,lChildName.Buffer());
                    lChildAttribute = KFbxNull::Create(mManager,"");
                    lChildNode->SetNodeAttribute(lChildAttribute);

                    lRootNode->AddChild(lChildNode);
                }
                else
                {
                    lChildNode = KFbxNode::Create(lScene,lChildName.Buffer());
                    lChildAttribute = KFbxNull::Create(lScene,"");
                    lChildNode->SetNodeAttribute(lChildAttribute);

                    lParentNode = lRootNode->FindChild(lParentName.Buffer());
                    lParentNode->AddChild(lChildNode);
                }
            }
            free(lBuffer);
        }
        lResult = true;
    }    
    return lResult;
}
