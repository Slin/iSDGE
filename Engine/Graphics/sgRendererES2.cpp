//
//	sgRendererES2.cpp
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

#include "sgRendererES2.h"
#include "sgShadowVolume.h"
#include "sgDebug.h"

sgRendererES2::sgRendererES2()
{
	oglversion = 2;
	msaasamples = 0;
	
	mainFramebuffer = -1;
	colorRenderbuffer = -1;
	depthRenderbuffer = -1;
	stencilRenderbuffer = -1;
	msaaFramebuffer = -1;
	msaaRenderbuffer = -1;
	
	// Create main framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
	glGenFramebuffers(1, &mainFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
	
	glGenRenderbuffers(1, &colorRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	if(supportpackedstencil)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	else
	{
		glGenRenderbuffers(1, &stencilRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, stencilRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderbuffer);
	}
	
	glEnable(GL_TEXTURE_2D);
	
	shadowvolume = sgMaterial::getMaterial(2);
	shadowquad = sgMaterial::getMaterial(3);
}

sgRendererES2::~sgRendererES2()
{
	if(mainFramebuffer != -1)
	{
		glDeleteFramebuffers(1, &mainFramebuffer);
		mainFramebuffer = -1;
	}
	
	if(colorRenderbuffer != -1)
	{
		glDeleteRenderbuffers(1, &colorRenderbuffer);
		colorRenderbuffer = -1;
	}
	
	if(depthRenderbuffer != -1) 
	{
		glDeleteRenderbuffers(1, &depthRenderbuffer);
		depthRenderbuffer = -1;
	}
	
	if(stencilRenderbuffer != -1) 
	{
		glDeleteRenderbuffers(1, &stencilRenderbuffer);
		stencilRenderbuffer = -1;
	}
	
	if(msaaFramebuffer != -1) 
	{
		glDeleteFramebuffers(1, &msaaFramebuffer);
		msaaFramebuffer = -1;
	}
	
	if(msaaRenderbuffer != -1) 
	{
		glDeleteRenderbuffers(1, &msaaRenderbuffer);
		msaaRenderbuffer = -1;
	}
}

void sgRendererES2::setMultisampling(unsigned short samples)
{
	int maxSamplesAllowed;
	glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
	int samplesToUse = (samples > maxSamplesAllowed) ? maxSamplesAllowed : samples;
	
	if(msaasamples == samplesToUse || !supportmultisampling)
		return;
	
	if(samplesToUse > 0)
	{
		if(msaasamples == 0)
		{
			//Detach depthbuffer
			glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
			
			//Create the MSAA framebuffer (offscreen)
			glGenFramebuffers(1, &msaaFramebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);

			//Create the offscreen MSAA color buffer.
			glGenRenderbuffers(1, &msaaRenderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderbuffer);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaRenderbuffer);
			
			//Attach depthbuffer
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
			if(supportpackedstencil)
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
			else
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderbuffer);
		}
	}else
	{
		//Detach depthbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
		
		if(msaaFramebuffer) 
		{
			glDeleteFramebuffers(1, &msaaFramebuffer);
			msaaFramebuffer = 0;
		}
		
		if(msaaRenderbuffer) 
		{
			glDeleteRenderbuffers(1, &msaaRenderbuffer);
			msaaRenderbuffer = 0;
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
		if(supportpackedstencil)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
		else
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderbuffer);
	}
	
	msaasamples = samplesToUse;
	resizeBuffers();
}

bool sgRendererES2::resizeBuffers()
{
	// Allocate color buffer backing based on the current layer size
	/*	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	 [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
	 glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	 glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);*/
	
	if(msaasamples == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
		if(supportpackedstencil)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, backingWidth, backingHeight);
		}else
		{
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, backingWidth, backingHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, stencilRenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, backingWidth, backingHeight);
		}
	}else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
		
		if(supportpackedstencil)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaasamples, GL_DEPTH24_STENCIL8_OES, backingWidth, backingHeight);
		}else
		{
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaasamples, GL_DEPTH_COMPONENT24_OES, backingWidth, backingHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, stencilRenderbuffer);
			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaasamples, GL_STENCIL_INDEX8, backingWidth, backingHeight);
		}
		
		glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderbuffer);
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaasamples, GL_RGBA8_OES, backingWidth, backingHeight);
	}
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		if(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == glCheckFramebufferStatus(GL_FRAMEBUFFER))
			sgLog("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		if(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS == glCheckFramebufferStatus(GL_FRAMEBUFFER))
			sgLog("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
		if(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT == glCheckFramebufferStatus(GL_FRAMEBUFFER))
			sgLog("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		if(GL_FRAMEBUFFER_UNSUPPORTED == glCheckFramebufferStatus(GL_FRAMEBUFFER))
			sgLog("GL_FRAMEBUFFER_UNSUPPORTED");
	}
	
	updateOrientation();
	
	return true;
}

void sgRendererES2::setMaterial(sgMaterial *mat)
{
	if(lastmat == mat)
		return;
	else if(lastmat != NULL)
	{
		if(*lastmat == *mat)
			return;
	}
	
	// Set shader textures
	int n;
	int l;
	sgLight *light;
	for(n = 0; n < mat->texlocation.size(); n++)
	{
		if(mat->texlocation[n] == -1)
			break;
		
		glActiveTexture(GL_TEXTURE0+n);
		glBindTexture(GL_TEXTURE_2D, mat->textures[n]->texid);
		glUniform1i(mat->texlocation[n], n);
	}
	
	// Set material specific stuff
	if(mat->culling)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(mat->cullmode);
	}else
	{
		glDisable(GL_CULL_FACE);
	}
	
	if(mat->blending)
	{
		glEnable(GL_BLEND);
		glBlendFunc(mat->blendsource, mat->blenddestination);
	}else
	{
		glDisable(GL_BLEND);
	}
	
	if(mat->lighting)
	{
		//Set shader lights
		l = 0;
		float *lightambient = (float*)calloc(mat->shader->maxlights*4, sizeof(float));
		float *lightdiffuse = (float*)calloc(mat->shader->maxlights*4, sizeof(float));
		float *lightspecular = (float*)calloc(mat->shader->maxlights*4, sizeof(float));
		float *lightpos = (float*)calloc(mat->shader->maxlights*4, sizeof(float));
		float *lightatt = (float*)calloc(mat->shader->maxlights, sizeof(float));
		for(light = first_light->next; light != NULL && l < mat->shader->maxlights; light = light->next)
		{
			memcpy(&lightambient[l*4], &light->ambient.r, sizeof(float)*4);
			memcpy(&lightdiffuse[l*4], &light->diffuse.r, sizeof(float)*4);
			memcpy(&lightspecular[l*4], &light->specular.r, sizeof(float)*4);
			memcpy(&lightpos[l*4], &light->position.x, sizeof(float)*4);
			memcpy(&lightatt[l*4], &light->attenuation, sizeof(float));
			l++;
		}
		if(mat->shader->lambientloc != -1)
		{
			glUniform4fv(mat->shader->lambientloc, mat->shader->maxlights, lightambient);
		}
		if(mat->shader->ldiffuseloc != -1)
		{
			glUniform4fv(mat->shader->ldiffuseloc, mat->shader->maxlights, lightdiffuse);
		}
		if(mat->shader->lspecularloc != -1)
		{
			glUniform4fv(mat->shader->lspecularloc, mat->shader->maxlights, lightspecular);
		}
		if(mat->shader->lpositionloc != -1)
		{
			glUniform4fv(mat->shader->lpositionloc, mat->shader->maxlights, lightpos);
		}
		if(mat->shader->lattenuationloc != -1)
		{
			glUniform1fv(mat->shader->lattenuationloc, mat->shader->maxlights, lightatt);
		}
		
		free(lightambient);
		free(lightdiffuse);
		free(lightspecular);
		free(lightpos);
		free(lightatt);
	}
	
	if(mat->colors)
	{
		if(mat->shader->mambientloc != -1)
		{
			glUniform4fv(mat->shader->mambientloc, 1, &mat->ambient.r);
		}
		if(mat->shader->mdiffuseloc != -1)
		{
			glUniform4fv(mat->shader->mdiffuseloc, 1, &mat->diffuse.r);
		}
		if(mat->shader->mspecularloc != -1)
		{
			glUniform4fv(mat->shader->mspecularloc, 1, &mat->specular.r);
		}
		if(mat->shader->mshininessloc != -1)
		{
			glUniform1f(mat->shader->mshininessloc, mat->shininess);
		}
		if(mat->shader->memissiveloc != -1)
		{
			glUniform4fv(mat->shader->memissiveloc, 1, &mat->emissive.r);
		}
	}
	
	if(mat->shader->malphatestvalue != -1)
	{
		if(mat->alphatest)
		{
			glUniform1f(mat->shader->malphatestvalue, mat->alphatestvalue);
		}else
		{
			glUniform1f(mat->shader->malphatestvalue, 0.0f);
		}
	}
	
	if(mat->depthtest)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(mat->depthtestmode);
	}else
	{
		glDisable(GL_DEPTH_TEST);
	}
	
	for(n = 0; n < mat->parameters.size(); n++)
	{
		if(mat->parameters[n].location != -1)
		{
			switch(mat->parameters[n].datatype)
			{
				case 0:
				{
					switch(mat->parameters[n].size)
					{
						case 0:
						{
							glUniform1fv(mat->parameters[n].location, mat->parameters[n].count, (float*)mat->parameters[n].parameter);
							break;
						}
						case 1:
						{
							glUniform2fv(mat->parameters[n].location, mat->parameters[n].count, (float*)mat->parameters[n].parameter);
							break;
						}
						case 2:
						{
							glUniform3fv(mat->parameters[n].location, mat->parameters[n].count, (float*)mat->parameters[n].parameter);
							break;
						}
						case 3:
						{
							glUniform4fv(mat->parameters[n].location, mat->parameters[n].count, (float*)mat->parameters[n].parameter);
							break;
						}
						case 4:
						{
							glUniformMatrix2fv(mat->parameters[n].location, mat->parameters[n].count, GL_FALSE, (float*)mat->parameters[n].parameter);
							break;
						}
						case 5:
						{
							glUniformMatrix3fv(mat->parameters[n].location, mat->parameters[n].count, GL_FALSE, (float*)mat->parameters[n].parameter);
							break;
						}
						case 6:
						{
							glUniformMatrix4fv(mat->parameters[n].location, mat->parameters[n].count, GL_FALSE, (float*)mat->parameters[n].parameter);
							break;
						}
					}
					break;
				}
				case 1:
				{
					switch(mat->parameters[n].size)
					{
						case 0:
						{
							glUniform1iv(mat->parameters[n].location, mat->parameters[n].count, (int*)mat->parameters[n].parameter);
							break;
						}
						case 1:
						{
							glUniform2iv(mat->parameters[n].location, mat->parameters[n].count, (int*)mat->parameters[n].parameter);
							break;
						}
						case 2:
						{
							glUniform3iv(mat->parameters[n].location, mat->parameters[n].count, (int*)mat->parameters[n].parameter);
							break;
						}
						case 3:
						{
							glUniform4iv(mat->parameters[n].location, mat->parameters[n].count, (int*)mat->parameters[n].parameter);
							break;
						}
					}
					break;
				}
			}
		}
	}
	
	lastmat = mat;
}

void sgRendererES2::renderObjects(sgCamera *cam, sgObject *first)
{
	cam->updateView();
	cam->matview = matglobal3d*cam->matview;
	
	sgObject *curr;
	int i;
	for(curr = first->next; curr != NULL; curr = curr->next)
	{
		if(curr->tag == cam->tag && cam->tag != 0)
			continue;
		
		curr->updateModel();
		
		for(i = 0; i < curr->meshs.size(); i++)
		{
			// Use shader program
			glUseProgram(curr->materials[i]->shader->program);
			
			// Set shader matrices
			if(curr->materials[i]->shader->matproj != -1)
			{
				glUniformMatrix4fv(curr->materials[i]->shader->matproj, 1, GL_FALSE, cam->matproj.mat);
			}
			if(curr->materials[i]->shader->matview != -1)
			{
				if(curr->sky)
				{
					sgMatrix4x4 viewmat = cam->rotation.getMatrix();
					viewmat.transpose();
					viewmat = matglobal3d*viewmat;
					glUniformMatrix4fv(curr->materials[i]->shader->matview, 1, GL_FALSE, viewmat.mat);
				}else
				{
					glUniformMatrix4fv(curr->materials[i]->shader->matview, 1, GL_FALSE, cam->matview.mat);
				}
			}
			if(curr->materials[i]->shader->matmodel != -1)
			{
				glUniformMatrix4fv(curr->materials[i]->shader->matmodel, 1, GL_FALSE, curr->matmodel.mat);
			}
			if(curr->materials[i]->shader->matnormal != -1)
			{
				glUniformMatrix4fv(curr->materials[i]->shader->matnormal, 1, GL_FALSE, curr->matnormal.mat);
			}
			if(curr->materials[i]->shader->mattex != -1)
			{
				glUniformMatrix4fv(curr->materials[i]->shader->mattex, 1, GL_FALSE, curr->materials[i]->mattex.mat);
			}
			
			setMaterial(curr->materials[i]);
			
			if(curr->meshs[i]->vbo != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, curr->meshs[i]->vbo);
				if(curr->materials[i]->shader->position != -1)
				{
					glEnableVertexAttribArray(curr->materials[i]->shader->position);
					glVertexAttribPointer(curr->materials[i]->shader->position, 3, GL_FLOAT, 0, sizeof(sgVertex), 0);
				}
				if(curr->materials[i]->shader->normal != -1)
				{
					glEnableVertexAttribArray(curr->materials[i]->shader->normal);
					glVertexAttribPointer(curr->materials[i]->shader->normal, 3, GL_FLOAT, 0, sizeof(sgVertex), (const void*)12);
				}
				if(curr->materials[i]->shader->texcoord0 != -1)
				{
					glEnableVertexAttribArray(curr->materials[i]->shader->texcoord0);
					glVertexAttribPointer(curr->materials[i]->shader->texcoord0, 2, GL_FLOAT, 0, sizeof(sgVertex), (const void*)24);
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}else
			{
				if(curr->materials[i]->shader->position != -1)
				{
					glVertexAttribPointer(curr->materials[i]->shader->position, 3, GL_FLOAT, 0, sizeof(sgVertex), &curr->meshs[i]->vertices->position.x);
					glEnableVertexAttribArray(curr->materials[i]->shader->position);
				}
				if(curr->materials[i]->shader->normal != -1)
				{
					glVertexAttribPointer(curr->materials[i]->shader->normal, 3, GL_FLOAT, 0, sizeof(sgVertex), &curr->meshs[i]->vertices->normal.x);
					glEnableVertexAttribArray(curr->materials[i]->shader->normal);
				}
				if(curr->materials[i]->shader->texcoord0 != -1)
				{
					glVertexAttribPointer(curr->materials[i]->shader->texcoord0, 2, GL_FLOAT, 0, sizeof(sgVertex), &curr->meshs[i]->vertices->uv.x);
					glEnableVertexAttribArray(curr->materials[i]->shader->texcoord0);
				}
			}
			
			if(curr->meshs[i]->ivbo != -1)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr->meshs[i]->ivbo);
				glDrawElements(GL_TRIANGLES, curr->meshs[i]->indexnum, GL_UNSIGNED_SHORT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}else
			{
				glDrawElements(GL_TRIANGLES, curr->meshs[i]->indexnum, GL_UNSIGNED_SHORT, curr->meshs[i]->indices);
			}
		}
	}
}

void sgRendererES2::renderPanels(sgPanel *first)
{
	glViewport(0, 0, backingWidth, backingHeight);
	
	sgMatrix4x4 matproj;
	sgMatrix4x4 matview;
	sgMatrix4x4 matmodel;
	sgMatrix4x4 mattex;
	matproj.makeProjectionOrtho(0, backingWidth, 0, backingHeight, -1, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
	
	sgImage *img;
	sgText *txt;
	for(sgPanel *curr = first->next; curr != NULL; curr = curr->next)
	{
		if(curr->fixorientation)
		{
			matview.makeTranslate(sgVector3(curr->pos.x, curr->pos.y, 0.0));
		}else
		{
			matview = matglobal2d;
			matview.translate(sgVector3(curr->pos.x, curr->pos.y, 0.0));
		}
		
		if(curr->rendertarget != NULL && currfbo != curr->rendertarget->fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, curr->rendertarget->fbo);
			currfbo = curr->rendertarget->fbo;
		}else
		{
			if(msaasamples > 0)
			{
				if(currfbo != msaaFramebuffer)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
					currfbo = msaaFramebuffer;
				}
			}else
			{
				if(currfbo != mainFramebuffer)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
					currfbo = mainFramebuffer;
				}
			}
		}
		
		glClearDepthf(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		for(int i = 0; i < curr->elements.size(); i++)
		{
			// Use shader program
			glUseProgram(curr->elements[i]->mat->shader->program);
			
			// Set shader matrices
			if(curr->elements[i]->mat->shader->matproj != -1)
			{
				glUniformMatrix4fv(curr->elements[i]->mat->shader->matproj, 1, GL_FALSE, matproj.mat);
			}
			if(curr->elements[i]->mat->shader->matview != -1)
			{
				glUniformMatrix4fv(curr->elements[i]->mat->shader->matview, 1, GL_FALSE, matview.mat);
			}
			
			if(curr->elements[i]->type == 0)
			{
				//draw image elements
				img = (sgImage*)curr->elements[i];
			
				if(img->mat->shader->matmodel != -1)
				{
					matmodel.makeTranslate(sgVector3(img->pos.x, img->pos.y, 0.0));
					matmodel.scale(sgVector3(img->size.x, img->size.y, 1.0));
					matmodel.rotate(sgVector3(0.0, img->ang, 0.0));
					glUniformMatrix4fv(img->mat->shader->matmodel, 1, GL_FALSE, matmodel.mat);
				}
				
				if(img->mat->shader->mattex != -1)
				{
					mattex.makeIdentity();
					glUniformMatrix4fv(img->mat->shader->mattex, 1, GL_FALSE, mattex.mat);
				}
				
				setMaterial(img->mat);
				
				if(img->mat->shader->position != -1)
				{
					glVertexAttribPointer(img->mat->shader->position, 3, GL_FLOAT, 0, 20, 0);
					glEnableVertexAttribArray(img->mat->shader->position);
				}
				
				if(img->mat->shader->texcoord0 != -1)
				{
					glVertexAttribPointer(img->mat->shader->texcoord0, 2, GL_FLOAT, 0, 20, (const void*)12);
					glEnableVertexAttribArray(img->mat->shader->texcoord0);
				}

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}else
			{
				//draw text elements
				txt = (sgText*)curr->elements[i];
				for(int n = 0; n < txt->str.str.size(); n++)
				{
					if(txt->mat->shader->matmodel != -1)
					{
						matmodel.makeTranslate(sgVector3(curr->pos.x+txt->pos.x+txt->size.x*n, curr->pos.y+txt->pos.y, 0.0));
						matmodel.scale(sgVector3(txt->size.x, txt->size.y, 1.0));
						glUniformMatrix4fv(txt->mat->shader->matmodel, 1, GL_FALSE, matmodel.mat);
					}
					
					if(txt->mat->shader->mattex != -1)
					{
						mattex.makeTranslate(sgVector3((float)txt->str.translatedx[n]*(float)txt->charsize.x/(float)txt->mat->textures[0]->width, (float)txt->str.translatedy[n]*(float)txt->charsize.y/(float)txt->mat->textures[0]->height, 0));
						mattex.scale(sgVector3((float)txt->charsize.x/(float)txt->mat->textures[0]->width, (float)txt->charsize.y/(float)txt->mat->textures[0]->height, 1));
						glUniformMatrix4fv(txt->mat->shader->mattex, 1, GL_FALSE, mattex.mat);
					}

					setMaterial(txt->mat);
					
					if(txt->mat->shader->position != -1)
					{
						glVertexAttribPointer(txt->mat->shader->position, 3, GL_FLOAT, 0, 20, 0);
						glEnableVertexAttribArray(txt->mat->shader->position);
					}
					
					if(txt->mat->shader->texcoord0 != -1)
					{
						glVertexAttribPointer(txt->mat->shader->texcoord0, 2, GL_FLOAT, 0, 20, (const void*)12);
						glEnableVertexAttribArray(txt->mat->shader->texcoord0);
					}
					
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sgRendererES2::renderShadowVolumes(sgObject *first)
{
	// Use shader program
	glUseProgram(shadowvolume->shader->program);
	
	sgCamera *cam;
	for(cam = first_cam->next; cam != NULL; cam = cam->next)
	{
		if(cam->rendertarget != NULL)
			continue;
		
		// Set shader matrices
		if(shadowvolume->shader->matproj != -1)
		{
			glUniformMatrix4fv(shadowvolume->shader->matproj, 1, GL_FALSE, cam->matproj.mat);
		}
		if(shadowvolume->shader->matview != -1)
		{
			glUniformMatrix4fv(shadowvolume->shader->matview, 1, GL_FALSE, cam->matview.mat);
		}
		
		sgObject *curr;
		for(curr = first->next; curr != NULL; curr = curr->next)
		{
			if(!curr->shadow || curr->shadowvolume == NULL)
				continue;
			
			// Set shader matrices
			if(shadowvolume->shader->matmodel != -1)
			{
				glUniformMatrix4fv(shadowvolume->shader->matmodel, 1, GL_FALSE, curr->matmodel.mat);
			}
			if(shadowvolume->shader->matnormal != -1)
			{
				glUniformMatrix4fv(shadowvolume->shader->matnormal, 1, GL_FALSE, curr->matnormal.mat);
			}
			
			if(curr->shadowvolume->mesh->vbo != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, curr->shadowvolume->mesh->vbo);
				if(shadowvolume->shader->position != -1)
				{
					glEnableVertexAttribArray(shadowvolume->shader->position);
					glVertexAttribPointer(shadowvolume->shader->position, 3, GL_FLOAT, 0, sizeof(sgVertex), 0);
				}
				if(shadowvolume->shader->normal != -1)
				{
					glEnableVertexAttribArray(shadowvolume->shader->normal);
					glVertexAttribPointer(shadowvolume->shader->normal, 3, GL_FLOAT, 0, sizeof(sgVertex), (const void*)12);
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}else
			{
				if(shadowvolume->shader->position != -1)
				{
					glEnableVertexAttribArray(shadowvolume->shader->position);
					glVertexAttribPointer(shadowvolume->shader->position, 3, GL_FLOAT, 0, sizeof(sgVertex), &(curr->shadowvolume->mesh->vertices->position.x));
				}
				if(shadowvolume->shader->normal != -1)
				{
					glEnableVertexAttribArray(shadowvolume->shader->normal);
					glVertexAttribPointer(shadowvolume->shader->normal, 3, GL_FLOAT, 0, sizeof(sgVertex), &(curr->shadowvolume->mesh->vertices->normal.x));
				}
			}
			
			if(curr->shadowvolume->mesh->ivbo != -1)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr->shadowvolume->mesh->ivbo);
				glDrawElements(GL_TRIANGLES, curr->shadowvolume->mesh->indexnum, GL_UNSIGNED_SHORT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}else
			{
				glDrawElements(GL_TRIANGLES, curr->shadowvolume->mesh->indexnum, GL_UNSIGNED_SHORT, curr->shadowvolume->mesh->indices);
			}
		}
	}
}

void sgRendererES2::renderShadows(sgObject *first)
{
	for(sgLight *light = first_light->next; light != NULL; light = light->next)
	{
		if(light->shadow)
		{
			for(sgObject *curr = first->next; curr != NULL; curr = curr->next)
			{
				if(curr->shadow && curr->shadowvolume != NULL)
				{
					curr->shadowvolume->update(light);
				}
			}
			break;
		}
	}
	
/*	for(sgObject *curr = first->next; curr != NULL; curr = curr->next)
	{
		if(curr->shadow && curr->shadowvolume != NULL)
		{
			if(curr->shadowvolume->mesh != NULL)
				curr->shadowvolume->mesh->updateVBO();
		}
	}*/
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
	
	glDisable(GL_CULL_FACE);
	renderShadowVolumes(first);
	glEnable(GL_CULL_FACE);
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render fullscreen quad
	glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
	glUseProgram(shadowquad->shader->program);
	if(shadowquad->shader->position != -1)
	{
		glVertexAttribPointer(shadowquad->shader->position, 3, GL_FLOAT, 0, 20, 0);
		glEnableVertexAttribArray(shadowquad->shader->position);
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
}

void sgRendererES2::render()
{
	//Clear color buffer
	if(msaasamples > 0)
	{
		if(currfbo != msaaFramebuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
			currfbo = msaaFramebuffer;
		}
	}else
	{
		if(currfbo != mainFramebuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
			currfbo = mainFramebuffer;
		}
	}
	glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, clearcolor.a);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Draw cameras
	sgCamera *cam;
	for(cam = first_cam->next; cam != NULL; cam = cam->next)
	{
		if(cam->rendertarget != NULL && currfbo != cam->rendertarget->fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, cam->rendertarget->fbo);
			currfbo = cam->rendertarget->fbo;
		}else
		{
			if(msaasamples > 0)
			{
				if(currfbo != msaaFramebuffer)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
					currfbo = msaaFramebuffer;
				}
			}else
			{
				if(currfbo != mainFramebuffer)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
					currfbo = mainFramebuffer;
				}
			}
		}
		
		//Clear depth and stencil buffer
		glClearStencil(0);
		glClearDepthf(1.0f);
		glClear(GL_STENCIL_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Draw objects
		glViewport(cam->screenpos.x, cam->screenpos.y, cam->size.x, cam->size.y);
		renderObjects(cam, first_sky);
		renderObjects(cam, first_solid);
		renderShadows(first_solid);
	}
	
	//Draw panels
	renderPanels(first_panel);
	
	if(msaasamples > 0)
	{
		//Resolve from msaaFramebuffer to resolveFramebuffer
		glDisable(GL_SCISSOR_TEST);     
		glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, mainFramebuffer);
		glResolveMultisampleFramebufferAPPLE();
	
		if(supportdiscard)
		{
			//discard framebuffer (a simple to use performence boost)
			GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT};
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 3, attachments);
		}
	}else
	{
		if(supportdiscard)
		{
			//discard framebuffer (a simple to use performence boost)
			GLenum attachments[] = {GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT};
			glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, attachments);
		}
	}
	
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
}
