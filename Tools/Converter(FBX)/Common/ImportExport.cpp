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

#include "ImportExport.h"
#include "sgmWriterReader.h"

// declare global
KFbxSdkManager*   gSdkManager = NULL;

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(gSdkManager->GetIOSettings()))
#endif


// to read and write a file using the FBXSDK readers/writers
//
// const char *ImportFileName : the full path of the file to be read
// const char* ExportFileName : the full path of the file to be written
// int pWriteFileFormat       : the specific file format number
//                                  for the writer

void ImportExport(const char *ImportFileName, const char* ExportFileName, int pWriteFileFormat)
{
	// Create a scene
	KFbxScene* lScene = KFbxScene::Create(gSdkManager,"");

    printf("------- Import started ---------------------------\n");

    // Load the scene.
    bool r = LoadScene(gSdkManager, lScene, ImportFileName);
    if(r)
        printf("------- Import succeeded -------------------------\n");
    else
    {
        printf("------- Import failed ----------------------------\n");

        // Destroy the scene
		lScene->Destroy();
        return;
    }

    printf("\r\n"); // add a blank line

    printf("------- Export started ---------------------------\n");

    // Save the scene.
    r = SaveScene(gSdkManager, 
        lScene,               // to export this scene...
        ExportFileName,       // to this path/filename...
        pWriteFileFormat,     // using this file format.
        false);               // Don't embed media files, if any.

    if(r) printf("------- Export succeeded -------------------------\n");
    else  printf("------- Export failed ----------------------------\n");

	// destroy the scene
	lScene->Destroy();
}

// Creates an instance of the SDK manager.
void InitializeSdkManager()
{
    // Create the FBX SDK memory manager object.
    // The SDK Manager allocates and frees memory
    // for almost all the classes in the SDK.
    gSdkManager = KFbxSdkManager::Create();

	// create an IOSettings object
	KFbxIOSettings * ios = KFbxIOSettings::Create(gSdkManager, IOSROOT );
	gSdkManager->SetIOSettings(ios);

	int lRegisteredCount;
	int lPluginId;
	gSdkManager->GetIOPluginRegistry()->RegisterWriter(CreatesgmWriter, GetsgmWriterInfo, lPluginId, lRegisteredCount, FillsgmWriterIOSettings);
	gSdkManager->GetIOPluginRegistry()->RegisterReader(CreatesgmReader, GetsgmReaderInfo, lPluginId, lRegisteredCount, FillsgmReaderIOSettings);
}

// Destroys an instance of the SDK manager
void DestroySdkObjects()
{
    // Delete the FBX SDK manager.
    // All the objects that
    // (1) have been allocated by the memory manager, AND that
    // (2) have not been explicitly destroyed
    // will be automatically destroyed.
    if (gSdkManager) gSdkManager->Destroy();
}

// Creates an importer object, and uses it to
// import a file into a scene.
bool LoadScene(
               KFbxSdkManager* pSdkManager,  // Use this memory manager...
               KFbxScene* pScene,            // to import into this scene
               const char* pFilename         // the data from this file.
               )
{
    int lFileMajor, lFileMinor, lFileRevision;
    int lSDKMajor,  lSDKMinor,  lSDKRevision;
    int i, lAnimStackCount;
    bool lStatus;
    char lPassword[1024];

    // Get the version number of the FBX files generated by the
    // version of FBX SDK that you are using.
    KFbxSdkManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

    // Create an importer.
    KFbxImporter* lImporter = KFbxImporter::Create(pSdkManager,"");
    
    // Initialize the importer by providing a filename.
    const bool lImportStatus = lImporter->Initialize(pFilename, -1, pSdkManager->GetIOSettings() );

    // Get the version number of the FBX file format.
    lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

    if( !lImportStatus )  // Problem with the file to be imported
    {
        printf("Call to KFbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n", lImporter->GetLastErrorString());

        if (lImporter->GetLastErrorID() ==
            KFbxIO::eFILE_VERSION_NOT_SUPPORTED_YET ||
            lImporter->GetLastErrorID() ==
            KFbxIO::eFILE_VERSION_NOT_SUPPORTED_ANYMORE)
        {
            printf("FBX version number for this FBX SDK is %d.%d.%d\n",
                lSDKMajor, lSDKMinor, lSDKRevision);
            printf("FBX version number for file %s is %d.%d.%d\n",
                pFilename, lFileMajor, lFileMinor, lFileRevision);
        }

        return false;
    }

    printf("FBX version number for this FBX SDK is %d.%d.%d\n",
        lSDKMajor, lSDKMinor, lSDKRevision);

    if (lImporter->IsFBX())
    {
        printf("FBX version number for file %s is %d.%d.%d\n",
            pFilename, lFileMajor, lFileMinor, lFileRevision);

        // In FBX, a scene can have one or more "animation stack". An animation stack is a
        // container for animation data.
        // You can access a file's animation stack information without
        // the overhead of loading the entire file into the scene.

        printf("Animation Stack Information\n");

        lAnimStackCount = lImporter->GetAnimStackCount();

        printf("    Number of animation stacks: %d\n", lAnimStackCount);
/*        printf("    Active animation stack: \"%s\"",
            lImporter->GetActiveAnimStackName());*/

        for(i = 0; i < lAnimStackCount; i++)
        {
            KFbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

            printf("    Animation Stack %d\n", i);
            printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
            printf("         Description: \"%s\"\n",
                lTakeInfo->mDescription.Buffer());

            // Change the value of the import name if the animation stack should
            // be imported under a different name.
            printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

            // Set the value of the import state to false
            // if the animation stack should be not be imported.
            printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
        }

        // Import options determine what kind of data is to be imported.
        // The default is true, but here we set the options explictly.

        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene.
    lStatus = lImporter->Import(pScene);

    if(lStatus == false &&     // The import file may have a password
        lImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR)
    {
        printf("Please enter password: ");

        lPassword[0] = '\0';

        scanf("%s", lPassword);
        KString lString(lPassword);
        
        IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
        IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);


        lStatus = lImporter->Import(pScene);

        if(lStatus == false && lImporter->GetLastErrorID() ==
            KFbxIO::ePASSWORD_ERROR)
        {
            printf("Incorrect password: file not imported.\n");
        }
    }

    // Destroy the importer
    lImporter->Destroy();

    return lStatus;
}

// Exports a scene to a file
bool SaveScene(
               KFbxSdkManager* pSdkManager,
               KFbxScene* pScene,
               const char* pFilename,
               int pFileFormat,
               bool pEmbedMedia
               )
{
    int lMajor, lMinor, lRevision;
    bool lStatus = true;

    // Create an exporter.
    KFbxExporter* lExporter = KFbxExporter::Create(pSdkManager, "");

/*    if( pFileFormat < 0 ||
        pFileFormat >=
        pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount() )
    {
        // Write in fall back format if pEmbedMedia is true
        pFileFormat =
            pSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();
     
        if (!pEmbedMedia)
        {
            //Try to export in ASCII if possible
            int lFormatIndex, lFormatCount =
                pSdkManager->GetIOPluginRegistry()->
                GetWriterFormatCount();

            for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
            {
                if (pSdkManager->GetIOPluginRegistry()->
                    WriterIsFBX(lFormatIndex))
                {
                    KString lDesc = pSdkManager->GetIOPluginRegistry()->
                        GetWriterFormatDescription(lFormatIndex);
                    if (lDesc.Find("ascii")>=0)
                    {
                        pFileFormat = lFormatIndex;
                        break;
                    }
                }
            }
        }
    }*/

    // Initialize the exporter by providing a filename.
    if(lExporter->Initialize(pFilename, -1, pSdkManager->GetIOSettings()) == false)
    {
        printf("Call to KFbxExporter::Initialize() failed.\n");
        printf("Error returned: %s", lExporter->GetLastErrorString());
        return false;
    }

    KFbxSdkManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
    printf("FBX version number for this FBX SDK is %d.%d.%d\n",
        lMajor, lMinor, lRevision);

    if (pSdkManager->GetIOPluginRegistry()->WriterIsFBX(pFileFormat))
    {
        // Export options determine what kind of data is to be imported.
        // The default (except for the option eEXPORT_TEXTURE_AS_EMBEDDED)
        // is true, but here we set the options explicitly.
        IOS_REF.SetBoolProp(EXP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(EXP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED,        pEmbedMedia);
        IOS_REF.SetBoolProp(EXP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(EXP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(EXP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);
    }

    // Export the scene.
    lStatus = lExporter->Export(pScene);

    // Destroy the exporter.
    lExporter->Destroy();

    return lStatus;
}

// Get the filters for the <Open file> dialog
// (description + file extention)
const char *GetReaderOFNFilters()
{
    int nbReaders =
		gSdkManager->GetIOPluginRegistry()->GetReaderFormatCount();

    KString s;
    int i = 0;

    for(i=0; i < nbReaders; i++)
    {
        s += gSdkManager->GetIOPluginRegistry()->
            GetReaderFormatDescription(i);
        s += "|*.";
        s += gSdkManager->GetIOPluginRegistry()->
            GetReaderFormatExtension(i);
        s += "|";
    }

    // replace | by \0
    int nbChar   = int(strlen(s.Buffer())) + 1;
    char *filter = new char[ nbChar ];
    memset(filter, 0, nbChar);

    strcpy(filter, s.Buffer());

    for(i=0; i < int(strlen(s.Buffer())); i++)
    {
        if(filter[i] == '|')
        {
            filter[i] = 0;
        }
    }

    // the caller must delete this allocated memory
    return filter;
}

// Get the filters for the <Save file> dialog
// (description + file extention)
const char *GetWriterSFNFilters()
{
    int nbWriters =
        gSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

    KString s;
    int i=0;

    for(i=0; i < nbWriters; i++)
    {
        s += gSdkManager->GetIOPluginRegistry()->
            GetWriterFormatDescription(i);
        s += "|*.";
        s += gSdkManager->GetIOPluginRegistry()->
            GetWriterFormatExtension(i);
        s += "|";
    }

    // replace | by \0
    int nbChar   = int(strlen(s.Buffer())) + 1;
    char *filter = new char[ nbChar ];
    memset(filter, 0, nbChar);

    strcpy(filter, s.Buffer());

    for(i=0; i < int(strlen(s.Buffer())); i++)
    {
        if(filter[i] == '|')
        {
            filter[i] = 0;
        }
    }

    // the caller must delete this allocated memory
    return filter;
}

// to get a file extention for a WriteFileFormat
const char *GetFileFormatExt(
                             const int pWriteFileFormat
                             )
{
    char *buf = new char[10];
    memset(buf, 0, 10);

    // add a starting point .
    buf[0] = '.';
    const char * ext = gSdkManager->GetIOPluginRegistry()->
        GetWriterFormatExtension(pWriteFileFormat);
    strcat(buf, ext);

    // the caller must delete this allocated memory
    return buf;
}
