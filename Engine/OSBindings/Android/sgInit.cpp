//
//  sgInit.mm
//  iSDGE
//
//	Created by Nils Daumann on 17.06.12.
//	Copyright (c) 2012 Nils Daumann

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

#include "sgInit.h"
#include "sgMain.h"
#include "sgEvents.h"
#include "sgResourceManager.h"
#include "sgTextureFiles.h"
#include <cstdio>
#include <jni.h>

sgMain *sgmain = NULL;

int sgInit(sgEvents *event, int res_x, int res_y)
{
	sgMain::eventhandler = event;

    sgRenderer::backingWidth = res_x;
    sgRenderer::backingHeight = res_y;

	//Initialize the renderer
	sgmain = new sgMain(2);

    return 0;
}

void sgDraw()
{
    sgmain->drawView();
}

void sgDestroy()
{
    delete sgmain;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgDraw(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgDestroy(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgSetAPK(JNIEnv * env, jobject obj, jstring path);
	JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgTouchBegan(JNIEnv * env, jobject obj, jint posx, jint posy);
	JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgTouchMoved(JNIEnv * env, jobject obj, jint posx, jint posy);
	JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgTouchEnded(JNIEnv * env, jobject obj, jint posx, jint posy);
};

JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgDraw(JNIEnv * env, jobject obj)
{
    sgDraw();
}

JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgDestroy(JNIEnv * env, jobject obj)
{
    sgDestroy();
}

JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgSetAPK(JNIEnv * env, jobject obj, jstring path)
{
	const char *fnameptr = env->GetStringUTFChars(path, NULL);
	sgResourceManager::loadZIP(fnameptr);
	sgTextureFiles::loadZIP(fnameptr);
	sgObjectFiles::loadZIP(fnameptr);
	env->ReleaseStringUTFChars(path, fnameptr); 
}


//Handle player input
JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgTouchBegan(JNIEnv * env, jobject obj, jint posx, jint posy)
{
	switch(sgmain->orientation)
	{
		case 0:
			sgTouches::addTouch(sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(posx, posy));
			break;
			
		case 1:
			sgTouches::addTouch(sgVector2(posx, sgRenderer::backingHeight/sgRenderer::scaleFactor-posy), sgVector2(posx, posy));
			break;
			
		case 2:
			sgTouches::addTouch(sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-posy, sgRenderer::backingWidth/sgRenderer::scaleFactor-posx), sgVector2(posx, posy));
			break;
			
		case 3:
			sgTouches::addTouch(sgVector2(posy, posx), sgVector2(posx, posy));
			break;
			
		default:
			sgTouches::addTouch(sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(posx, posy));
			break;
	}
}

JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgTouchMoved(JNIEnv * env, jobject obj, jint posx, jint posy)
{
	switch(sgmain->orientation)
	{
		case 0:
			sgTouches::updateTouch(0, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(posx, posy));
			break;
			
		case 1:
			sgTouches::updateTouch(0, sgVector2(posx, sgRenderer::backingHeight/sgRenderer::scaleFactor-posy), sgVector2(posx, sgRenderer::backingHeight/sgRenderer::scaleFactor-posy), sgVector2(posx, posy));
			break;
			
		case 2:
			sgTouches::updateTouch(0, sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-posy, sgRenderer::backingWidth/sgRenderer::scaleFactor-posx), sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-posy, sgRenderer::backingWidth/sgRenderer::scaleFactor-posx), sgVector2(posx, posy));
			break;
			
		case 3:
			sgTouches::updateTouch(0, sgVector2(posy, posx), sgVector2(posy, posx), sgVector2(posx, posy));
			break;
			
		default:
			sgTouches::updateTouch(0, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(posx, posy));
			break;
	}
}

JNIEXPORT void JNICALL Java_com_android_isdge_ISDGELib_sgTouchEnded(JNIEnv * env, jobject obj, jint posx, jint posy)
{
	sgTouches::removeTouch(sgTouches::touches.size()-1);
/*	switch([sgView main]->orientation)
	{
		case 0:
			sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(posx, posy));
			break;
			
		case 1:
			sgTouches::updateTouch(i, sgVector2(posx, sgRenderer::backingHeight/sgRenderer::scaleFactor-posy), sgVector2(posx, sgRenderer::backingHeight/sgRenderer::scaleFactor-posy), sgVector2(posx, posy));
			break;
			
		case 2:
			sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-posy, sgRenderer::backingWidth/sgRenderer::scaleFactor-posx), sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-posy, sgRenderer::backingWidth/sgRenderer::scaleFactor-posx), sgVector2(posx, posy));
			break;
			
		case 3:
			sgTouches::updateTouch(i, sgVector2(posy, posx), sgVector2(posy, posx), sgVector2(posx, posy));
			break;
			
		default:
			sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-posx, posy), sgVector2(posx, posy));
			break;
	}*/
}
