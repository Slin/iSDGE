//
//	sgParticleEmitter.cpp
//	iSDGE
//
//	Created by Nils Daumann on 13.02.11.
//	Copyright (c) 2011 Nils Daumann

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

#include "sgParticleEmitter.h"
#include "sgParticle.h"
#include "sgMaterial.h"
#include "sgCamera.h"

sgParticleEmitter::sgParticleEmitter(sgParticleEmitter* p, sgParticleEmitter *n)
{
	prev = p;
	next = n;
	tag = 0;
	
	material = NULL;
	vertices = NULL;
	indices = NULL;
	
	indexnum = 0;
}

sgParticleEmitter::~sgParticleEmitter()
{
	for(int i = particles.size()-1; i >= 0; i--)
		delete particles[i];
	
	particles.empty();
	
	free(vertices);
	free(indices);
}

sgParticleEmitter *sgParticleEmitter::createEmitter(const char *texfile)
{
	next = new sgParticleEmitter(this, next);
	next->material = sgMaterial::getMaterial(texfile, true, -3);
	next->material->blending = true;
	next->material->depthwrite = false;
	return next;
}

void sgParticleEmitter::emitPart(sgParticle *p)
{
	particles.push_back(p);
}

void sgParticleEmitter::updateMesh(sgCamera *cam, float timestep)
{
	for(int i = particles.size()-1; i >= 0; i--)
	{
		particles[i]->onDraw(timestep);
		
		if(particles[i]->destroy)
		{
			delete particles[i];
			particles.erase(particles.begin()+i);
		}
	}
	
	if(indexnum != particles.size()*6)
	{
		indexnum = particles.size()*6;
		if(vertices == NULL)
		{
			vertices = (float*)malloc(sizeof(float)*particles.size()*9*4);
			indices = (unsigned short*)malloc(sizeof(unsigned short)*indexnum);
		}else
		{
			vertices = (float*)realloc(vertices, sizeof(float)*particles.size()*9*4);
			indices = (unsigned short*)realloc(indices, sizeof(unsigned short)*indexnum);
		}
	}
	
	sgVector3 vert1(-0.5, -0.5, 0.0);
	sgVector3 vert2(-0.5, 0.5, 0.0);
	sgVector3 vert3(0.5, -0.5, 0.0);
	sgVector3 vert4(0.5, 0.5, 0.0);
	
	vert1 = cam->rotation.rotate(vert1);
	vert2 = cam->rotation.rotate(vert2);
	vert3 = cam->rotation.rotate(vert3);
	vert4 = cam->rotation.rotate(vert4);
	
	for(int i = 0; i < particles.size(); i++)
	{
		//Position
		vertices[i*4*9+0*9+0] = vert1.x*particles[i]->scale+particles[i]->position.x;	//bottom left
		vertices[i*4*9+0*9+1] = vert1.y*particles[i]->scale+particles[i]->position.y;
		vertices[i*4*9+0*9+2] = vert1.z*particles[i]->scale+particles[i]->position.z;
		
		vertices[i*4*9+1*9+0] = vert2.x*particles[i]->scale+particles[i]->position.x;	//top left
		vertices[i*4*9+1*9+1] = vert2.y*particles[i]->scale+particles[i]->position.y;
		vertices[i*4*9+1*9+2] = vert2.z*particles[i]->scale+particles[i]->position.z;
		
		vertices[i*4*9+2*9+0] = vert3.x*particles[i]->scale+particles[i]->position.x;	//bottom right
		vertices[i*4*9+2*9+1] = vert3.y*particles[i]->scale+particles[i]->position.y;
		vertices[i*4*9+2*9+2] = vert3.z*particles[i]->scale+particles[i]->position.z;
		
		vertices[i*4*9+3*9+0] = vert4.x*particles[i]->scale+particles[i]->position.x;	//top right
		vertices[i*4*9+3*9+1] = vert4.y*particles[i]->scale+particles[i]->position.y;
		vertices[i*4*9+3*9+2] = vert4.z*particles[i]->scale+particles[i]->position.z;
		
		//Color
		vertices[i*4*9+0*9+3] = particles[i]->color.x;
		vertices[i*4*9+0*9+4] = particles[i]->color.y;
		vertices[i*4*9+0*9+5] = particles[i]->color.z;
		vertices[i*4*9+0*9+6] = particles[i]->color.w;
		
		vertices[i*4*9+1*9+3] = particles[i]->color.x;
		vertices[i*4*9+1*9+4] = particles[i]->color.y;
		vertices[i*4*9+1*9+5] = particles[i]->color.z;
		vertices[i*4*9+1*9+6] = particles[i]->color.w;
		
		vertices[i*4*9+2*9+3] = particles[i]->color.x;
		vertices[i*4*9+2*9+4] = particles[i]->color.y;
		vertices[i*4*9+2*9+5] = particles[i]->color.z;
		vertices[i*4*9+2*9+6] = particles[i]->color.w;
		
		vertices[i*4*9+3*9+3] = particles[i]->color.x;
		vertices[i*4*9+3*9+4] = particles[i]->color.y;
		vertices[i*4*9+3*9+5] = particles[i]->color.z;
		vertices[i*4*9+3*9+6] = particles[i]->color.w;
		
		//UV
		vertices[i*4*9+0*9+7] = 0.0;
		vertices[i*4*9+0*9+8] = 1.0;
		
		vertices[i*4*9+1*9+7] = 0.0;
		vertices[i*4*9+1*9+8] = 0.0;
		
		vertices[i*4*9+2*9+7] = 1.0;
		vertices[i*4*9+2*9+8] = 1.0;
		
		vertices[i*4*9+3*9+7] = 1.0;
		vertices[i*4*9+3*9+8] = 0.0;
		
		//indices
		indices[i*6+0] = i*4+2;
		indices[i*6+1] = i*4+1;
		indices[i*6+2] = i*4+0;
		
		indices[i*6+3] = i*4+1;
		indices[i*6+4] = i*4+2;
		indices[i*6+5] = i*4+3;
	}
}

void sgParticleEmitter::destroy()
{
	if(prev)
		prev->next = next;
	if(next)
		next->prev = prev;
	
	delete this;
}

void sgParticleEmitter::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
}
