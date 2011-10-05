//
//	sgRenderer.cpp
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
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHERF
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include "sgRenderer.h"
#include "sgDebug.h"
#include "math.h"

#include <string>
#include "sgResourceManager.h"

#ifdef __APPLE__
	#include "TargetConditionals.h"
#endif

unsigned int sgRenderer::oglversion = 0;
unsigned int sgRenderer::maxbones = 30;
int sgRenderer::backingWidth = 640;
int sgRenderer::backingHeight = 480;

sgRenderer::sgRenderer()
{
	currenttime = 0.0;
	timestep = 0.0;
	
	supportmultisampling = checkForExtension((char*)"APPLE_framebuffer_multisample");
	supportdiscard = true;//checkForExtension((char*)"EXT_discard_framebuffer");
	supportpackedstencil = true;//true;//false;//checkForExtension((char*)"EXT_packed_depth_stencil");
	
	clearcolor = sgFColorA(0.5f, 0.7f, 1.0f, 1.0f);
	
	first_sky = new sgObject(NULL, NULL);
	first_solid = new sgObject(NULL, NULL);
	first_cam = new sgCamera(NULL, NULL);
	first_light = new sgLight(NULL, NULL);
	first_panel = new sgPanel(NULL, NULL);
	first_partemitter = new sgParticleEmitter(NULL, NULL);
	
	first_optimized = NULL;
	
	quadposuv[0] = 0.0; quadposuv[1] = 0.0; quadposuv[2] = 0.0;
	quadposuv[3] = 0.0; quadposuv[4] = 1.0;
	quadposuv[5] = 1.0; quadposuv[6] = 0.0; quadposuv[7] = 0.0;
	quadposuv[8] = 1.0; quadposuv[9] = 1.0;
	quadposuv[10] = 0.0; quadposuv[11] = 1.0; quadposuv[12] = 0.0;
	quadposuv[13] = 0.0; quadposuv[14] = 0.0;
	quadposuv[15] = 1.0; quadposuv[16] = 1.0; quadposuv[17] = 0.0;
	quadposuv[18] = 1.0; quadposuv[19] = 0.0;
	
	glGenBuffers(1, &quadvbo);
	glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
	glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), quadposuv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	currfbo = -1;
	
	lastmat = NULL;
	event = NULL;
}

sgRenderer::~sgRenderer()
{
	if(event != NULL)
		delete event;
	
	first_sky->destroyAll();
	first_solid->destroyAll();
	first_cam->destroyAll();
	first_light->destroyAll();
	first_panel->destroyAll();
	first_partemitter->destroyAll();
	sgResourceManager::destroyAll();
	
	sgOptimizedMesh *mt = NULL;
	for(sgOptimizedMesh *m = first_optimized; m != NULL; m = m->next)
	{
		if(mt != NULL)
			delete mt;
		mt = m;
	}
	if(mt != NULL)
		delete mt;
}

bool sgRenderer::resizeBuffers()
{
	return true;
}

void sgRenderer::render()
{
	
}

void sgRenderer::setOrientation(unsigned int ori)
{
	orientation = ori;
	updateOrientation();
}

void sgRenderer::updateOrientation()
{
	if(orientation == 0)
	{
		matglobal3d.makeRotate(sgVector3(0, 0, 0));
		matglobal2d.makeRotate(sgVector3(0, 0, 0));
	}
	if(orientation == 1)
	{
		matglobal3d.makeRotate(sgVector3(0, 180, 0));
		
		matglobal2d.makeTranslate(sgVector3(backingWidth*0.5, backingHeight*0.5, 0.0));
		matglobal2d.rotate(sgVector3(0, 180, 0));
		matglobal2d.translate(sgVector3(-backingWidth*0.5, -backingHeight*0.5, 0.0));
	}
	if(orientation == 2)
	{
		matglobal3d.makeRotate(sgVector3(0, 90, 0));
		
		matglobal2d.makeTranslate(sgVector3(backingWidth*0.5, backingHeight*0.5, 0.0));
		matglobal2d.rotate(sgVector3(0, 90, 0));
		matglobal2d.translate(sgVector3(-backingWidth*0.5-(backingHeight-backingWidth)*0.5, -backingHeight*0.5+(backingHeight-backingWidth)*0.5, 0.0));
	}
	if(orientation == 3)
	{
		matglobal3d.makeRotate(sgVector3(0, -90, 0));
		
		matglobal2d.makeTranslate(sgVector3(backingWidth*0.5, backingHeight*0.5, 0.0));
		matglobal2d.rotate(sgVector3(0, -90, 0));
		matglobal2d.translate(sgVector3(-backingWidth*0.5-(backingHeight-backingWidth)*0.5, -backingHeight*0.5+(backingHeight-backingWidth)*0.5, 0.0));
	}
}

void sgRenderer::setMultisampling(unsigned short samples)
{
	
}

bool sgRenderer::checkForExtension(char *name)
{
	std::string extensionsString((const char*)glGetString(GL_EXTENSIONS));
    return (extensionsString.rfind(name) != std::string::npos);
}

void sgRenderer::chooseObjLOD(sgObject *obj, float dist)
{
	sgObjectBody *bod = obj->body;
	
	if(obj->body->nextbody != NULL)
	{
		while(dist > bod->loddist && bod->nextbody != NULL)
		{
			bod = bod->nextbody;
		}
		
		obj->currbody = bod;
	}
}

void sgRenderer::chooseMeshLOD(sgObject *obj, unsigned int mesh, float dist)
{
	sgObjectBody *bod = obj->body;
	sgObjectBody *currbod = obj->currbody;
	
	if(obj->body->nextbody != NULL)
	{
		while(dist > bod->loddist && bod->nextbody != NULL)
		{
			bod = bod->nextbody;
		}
		
		currbod->meshs.push_back(bod->meshs[mesh]);
		currbod->materials.push_back(bod->materials[mesh]);
	}
}

void sgRenderer::culling(sgCamera *cam, sgObject *first)
{
	sgVector3 pos;
	float dist;
	float radius;
	
	cam->updateFrustum();
	
	for(sgObject *obj = first->next; obj != NULL; obj = obj->next)
	{
		obj->updateModel();
		pos = obj->cullsphere;
		pos = obj->matmodel*pos;
		radius = fabs(obj->cullsphere.w)*fmax(obj->scale.x, fmax(obj->scale.y, obj->scale.z));
		
		if(cam->inFrustum(pos, radius))
		{
			obj->culled = false;
			
			if(obj->cullsphere.w < 0)
			{
				//Reset LOD
				if(obj->body->nextbody != NULL)
				{
					obj->currbody->meshs.clear();
					obj->currbody->materials.clear();
				}
				
				for(int i = 0; i < obj->body->meshs.size(); i++)
				{
					pos = obj->body->meshs[i]->cullsphere;
					pos = obj->matmodel*pos;
					
					radius = obj->body->meshs[i]->cullsphere.w*fmax(obj->scale.x, fmax(obj->scale.y, obj->scale.z));
					
					if(cam->inFrustum(pos, radius))
					{
						obj->body->meshs[i]->culled = false;
						
						dist = sgVector3(pos).dist(cam->position);
						chooseMeshLOD(obj, i, dist-radius+cam->lodshift);
					}else
					{
						obj->body->meshs[i]->culled = true;
					}
				}
			}else
			{
				dist = sgVector3(pos).dist(cam->position);
				chooseObjLOD(obj, dist+cam->lodshift);
			}
		}else
		{
			obj->culled = true;
		}
	}
}

void sgRenderer::sorting()
{
	//delete previous ones
/*	sgOptimizedMesh *mt = NULL;
	for(sgOptimizedMesh *m = first_optimized; m != NULL; m = m->next)
	{
		if(mt != NULL)
			delete mt;
		mt = m;
	}
	if(mt != NULL)
		delete mt;
	
	//create new ones
	first_optimized = new sgOptimizedMesh;
	sgOptimizedMesh *curr = first_optimized;
	sgOptimizedMesh *tempmesh;
	sgOptimizedMesh *tempmesh2;
	sgMaterial *tempmat;
	first_optimized->prev = NULL;
	for(sgObject *obj = first_solid->next; obj != NULL; obj = obj->next)
	{
		for(unsigned int i = 0; i < obj->meshs.size(); i++)
		{
			curr->next = new sgOptimizedMesh;
			curr->next->prev = curr;
			curr = curr->next;
			curr->mesh = obj->meshs[i];
			curr->material = obj->materials[i];
			curr->object = obj;
		}
	}
	curr->next = NULL;
	
	//sort
	curr = first_optimized->next;
	while(curr != NULL)
	{
		tempmat = curr->material;
		
		for(tempmesh = curr->next; tempmesh != NULL; tempmesh = tempmesh2)
		{
			tempmesh2 = tempmesh->next;
			if(tempmesh->material == tempmat || *(tempmesh->material) == *tempmat)
			{
				if(tempmesh->next != NULL)
				{
					tempmesh->next->prev = tempmesh->prev;
				}

				tempmesh->prev->next = tempmesh->next;
				
				tempmesh->prev = curr->prev;
				tempmesh->next = curr;
				curr->prev->next = tempmesh;
				curr->prev = tempmesh;
			}
		}
		
		curr = curr->next;
	}*/
}

void sgRenderer::optimizing()
{
	
}
