//
//	sgObject.cpp
//	iSDGE
//
//	Created by Nils Daumann on 17.04.10.
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

#include "sgObject.h"

#include "sgResourceManager.h"
#include "sgObjectFiles.h"
#include "sgMaterial.h"
#include "sgMesh.h"
#include "sgLight.h"
#include "sgShadowVolume.h"
#include "sgRenderer.h"
#include "sgDebug.h"

#include <stdlib.h>
#include <math.h>s

sgObject::sgObject(sgObject* p, sgObject *n)
{
	prev = p;
	next = n;
	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
	shadow = false;
	sky = false;
	shadowvolume = NULL;
	tag = 0;
	culled = false;
	body = NULL;
	currbody = NULL;
}

sgObject::~sgObject()
{
	if(shadowvolume != NULL)
	{
		delete shadowvolume;
		shadowvolume = NULL;
	}
	
	if(body != NULL)
		body->destroyAll();
}

sgObject *sgObject::createObject(const char *name)
{
	next = new sgObject(prev, next);
	next->body = new sgObjectBody;
	next->body->makeObject(name);
	next->currbody = next->body;
	return next;
}

sgObject *sgObject::createTerrain(unsigned int xverts, unsigned int zverts, unsigned char xchunks, unsigned char zchunks, unsigned int lodsteps, const char *hmp, sgVector4 hmpscale)
{
	if(xverts == 0 || zverts == 0)
		return NULL;
	
	lodsteps += 1;
	if(xchunks == 0)
		xchunks = 1;
	if(zchunks == 0)
		zchunks = 1;
	
	next = new sgObject(prev, next);
	sgMaterial *mat = sgMaterial::getMaterial();
	
	sgTexture *tex = NULL;
	float width = 0;
	float height = 0;
	if(hmp != NULL)
	{
		tex = sgTexture::getTexture2D(hmp, false, true);
		width = (tex->width-1)/xchunks;
		height = (tex->height-1)/zchunks;
	}
	
	unsigned int realx;
	unsigned int realz;

	float scl = 1.0;
	bool islod = false;
	for(int i = 0; i < lodsteps; i++)
	{
		realx = xverts/xchunks;
		if(xchunks > 1)
			realx += 1;
		
		realz = zverts/zchunks;
		if(zchunks > 1)
			realz += 1;
		
		if(i == 0)
		{
			islod = false;
			next->body = new sgObjectBody;
			next->currbody = next->body;
		}else
		{
			islod = true;
			next->currbody->nextbody = new sgObjectBody;
			next->currbody = next->currbody->nextbody;
		}
		
		for(int x = 0; x < xchunks; x++)
		{
			for(int y = 0; y < zchunks; y++)
			{
				next->currbody->addTerrainPlane(realx, realz,
									 sgVector3((float)((xverts-1.0)/xchunks*(float)x-(float)(xverts-1.0)*0.5+0.5f*(realx-1.0))*scl, 0.0,
									(float)((zverts-1.0)/zchunks*(float)y-(float)(zverts-1.0)*0.5+0.5f*(realz-1.0))*scl),
									 mat, sgVector2(scl, scl), x, y,
												sgVector2(1.0/xverts/scl, 1.0/zverts/scl), tex, sgVector2(width, height), hmpscale, islod);
			}
		}
		
		next->currbody->loddist = 50.0+50.0*i;
		
		scl *= 2.0;
		xverts /= 2;
		zverts /= 2;
	}
	
	next->currbody = new sgObjectBody;
	for(int i = 0; i < next->body->meshs.size(); i++)
	{
		next->currbody->meshs.push_back(next->body->meshs[i]);
		next->currbody->materials.push_back(next->body->materials[i]);
	}
	
	if(tex != NULL)
		tex->destroy();

	next->calcCullSphere();
	
	return next;
}

void sgObject::initShadowVolume()
{
	if(shadowvolume == NULL && sgRenderer::oglversion > 1)
		shadowvolume = new sgShadowVolume(this);
}

void sgObject::calcCullSphere()
{
	sgObjectBody *tb = body;
	while(tb != NULL)
	{
		tb->calcCullSphere();
		tb = tb->nextbody;
	}
	
	//Find center
	sgVector3 vmax(-1000000000.0, -1000000000.0, -1000000000.0);
	sgVector3 vmin(1000000000.0, 1000000000.0, 1000000000.0);
	sgVector3 center;
	for(int m = 0; m < body->meshs.size(); m++)
	{
		center += sgVector3(body->meshs[m]->cullsphere);
		
		if(body->meshs[m]->cullsphere.x > vmax.x)
			vmax.x = body->meshs[m]->cullsphere.x;
		if(body->meshs[m]->cullsphere.y > vmax.y)
			vmax.y = body->meshs[m]->cullsphere.y;
		if(body->meshs[m]->cullsphere.z > vmax.z)
			vmax.z = body->meshs[m]->cullsphere.z;
		
		if(body->meshs[m]->cullsphere.x < vmin.x)
			vmin.x = body->meshs[m]->cullsphere.x;
		if(body->meshs[m]->cullsphere.y < vmin.y)
			vmin.y = body->meshs[m]->cullsphere.y;
		if(body->meshs[m]->cullsphere.z < vmin.z)
			vmin.z = body->meshs[m]->cullsphere.z;
	}
	center = vmin+vmax;
	center *= 0.5f;
	cullsphere = center;
	
	//Find radius
	float radius = 0;
	float temp;
	sgVector3 diff;
	for(int m = 0; m < body->meshs.size(); m++)
	{
		diff = sgVector3(body->meshs[m]->cullsphere)-center;
		temp = diff.length();
		if(temp > body->meshs[m]->cullsphere.w*2.0)
		{
			radius = -1.0;
			break;
		}
			
		temp += body->meshs[m]->cullsphere.w;
		if(temp > radius)
			radius = temp;
	}
	cullsphere.w = radius;
}

void sgObject::updateModel()
{
	matmodel.makeTranslate(position);
	matmodel.scale(scale);
	
	matnormal = rotation.getMatrix();
	matmodel *= matnormal;
}

void sgObject::destroy()
{
	if(prev)
		prev->next = next;
	if(next)
		next->prev = prev;
	
	delete this;
}

void sgObject::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
}
