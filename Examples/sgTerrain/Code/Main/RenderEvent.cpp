//
//	RenderEvent.cpp
//	iSDGE
//
//	Created by Nils Daumann on 04.12.10.
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

#include "RenderEvent.h"
#include "sgRenderer.h"
#include "sgCamera.h"

RenderEvent::RenderEvent(sgEntity *terr, const char *base, const char *clip)
{
	initTerrShads(terr, base, clip);
}

void RenderEvent::initTerrShads(sgEntity *terr, const char *base, const char *clip)
{
	terrent = terr;
	baseshad = sgShader::getShader(base, base);
	terrent->obj->body->materials[0]->shader = baseshad;
	terrent->obj->body->materials[0]->getUniforms();
	clipshad = sgShader::getShader(clip, clip);
}

void RenderEvent::onDrawCam(sgCamera *cam, sgRenderer *rend)
{
	//Change shader depending on the camera
	if(cam == rend->first_cam->next)	//Reflection camera
	{
		terrent->obj->body->materials[0]->shader = clipshad;
	}else
	{
		terrent->obj->body->materials[0]->shader = baseshad;
	}
}