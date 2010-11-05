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
#include "sgmWriterReader.h"
#include <fbxfilesdk/fbxfilesdk_nsuse.h>

// Create your own writer.
// And your writer will get a pPluginID and pSubID. 
KFbxWriter* CreatesgmWriter(KFbxSdkManager& pManager, KFbxExporter& pExporter, int pSubID, int pPluginID)
{
    return FbxSdkNew<sgmWriter>(pManager, pPluginID);
}

// Get extension, description or version info about sgmWriter
void* GetsgmWriterInfo(KFbxWriter::KInfoRequest pRequest, int pId)
{
    static char const* sExt[] = 
    {
        "sgm",
        0
    };

    static char const* sDesc[] = 
    {
        "iSDGE FileFormat (*.sgm)",
        0
    };

    switch (pRequest)
    {
    case KFbxWriter::eInfoExtension:
        return sExt;
    case KFbxWriter::eInfoDescriptions:
        return sDesc;
    case KFbxWriter::eInfoVersions:
        return 0;
    default:
        return 0;
    }
}

void FillsgmWriterIOSettings(KFbxIOSettings& pIOS)
{
    // Here you can write your own KFbxIOSettings and parse them.
}


// Creates a sgmReader in the Sdk Manager
KFbxReader* CreatesgmReader(KFbxSdkManager& pManager, KFbxImporter& pImporter, int pSubID, int pPluginID)
{
    return FbxSdkNew<sgmReader>(pManager, pPluginID);
}

// Get extension, description or version info about sgmReader
void *GetsgmReaderInfo(KFbxReader::KInfoRequest pRequest, int pId)
{
    switch (pRequest)
    {
    case KFbxReader::eInfoExtension:
        return GetsgmWriterInfo(KFbxWriter::eInfoExtension, pId);
    case KFbxReader::eInfoDescriptions:
        return GetsgmWriterInfo(KFbxWriter::eInfoDescriptions, pId);
    default:
        return 0;
    }
}

void FillsgmReaderIOSettings(KFbxIOSettings& pIOS)
{    
    // Here you can write your own KFbxIOSettings and parse them.
}
