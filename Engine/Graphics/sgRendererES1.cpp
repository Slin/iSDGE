//
//	sgRendererES1.cpp
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

#include "sgRendererES1.h"
#include "sgDebug.h"

sgRendererES1::sgRendererES1()
{
	oglversion = 1;
	msaasamples = 0;
	
	mainFramebuffer = 0;
	colorRenderbuffer = 0;
	depthRenderbuffer = 0;
	msaaFramebuffer = 0;
	msaaRenderbuffer = 0;
	
	// Create default framebuffer object. Initializing will be finished by resizeBuffers.
	glGenFramebuffersOES(1, &mainFramebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mainFramebuffer);
	
	glGenRenderbuffersOES(1, &colorRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
	
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEnable(GL_TEXTURE_2D);
	
	float amb[4];
	amb[0] = 1.0; amb[1] = 1.0; amb[2] = 1.0; amb[3] = 1.0;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

sgRendererES1::~sgRendererES1()
{
	// Tear down GL
    if(mainFramebuffer)
    {
        glDeleteFramebuffersOES(1, &mainFramebuffer);
        mainFramebuffer = 0;
    }
	
    if(colorRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
	
	if(depthRenderbuffer) 
	{
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
	
	if(msaaFramebuffer) 
	{
		glDeleteFramebuffersOES(1, &msaaFramebuffer);
		msaaFramebuffer = 0;
	}
	
	if(msaaRenderbuffer) 
	{
		glDeleteRenderbuffersOES(1, &msaaRenderbuffer);
		msaaRenderbuffer = 0;
	}
}

void sgRendererES1::setMultisampling(unsigned short samples)
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
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, mainFramebuffer);
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
			
			//Create the MSAA framebuffer (offscreen)
			glGenFramebuffersOES(1, &msaaFramebuffer);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFramebuffer);
			
			//Create the offscreen MSAA color buffer.
			glGenRenderbuffersOES(1, &msaaRenderbuffer);
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, msaaRenderbuffer);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, msaaRenderbuffer);
			
			//Attach depthbuffer
			glBindRenderbufferOES(GL_RENDERBUFFER, depthRenderbuffer);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
		}
	}else
	{
		//Detach depthbuffer
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
		
		if(msaaFramebuffer) 
		{
			glDeleteFramebuffersOES(1, &msaaFramebuffer);
			msaaFramebuffer = 0;
		}
		
		if(msaaRenderbuffer) 
		{
			glDeleteRenderbuffersOES(1, &msaaRenderbuffer);
			msaaRenderbuffer = 0;
		}
		
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, mainFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
	}
	
	msaasamples = samplesToUse;
	resizeBuffers();
}

bool sgRendererES1::resizeBuffers()
{
	// Allocate color buffer backing based on the current layer size
	/*	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	 [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
	 glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	 glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);*/
	
	if(msaasamples == 0)
	{
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, backingWidth, backingHeight);
	}else
	{
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, msaasamples, GL_DEPTH_COMPONENT24_OES, backingWidth, backingHeight);
		
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, msaaRenderbuffer);
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, msaasamples, GL_RGBA8_OES, backingWidth, backingHeight);
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

void sgRendererES1::setMaterial(sgMaterial *mat)
{
	if(lastmat == mat)
		return;
	else if(lastmat != NULL)
	{
		if(*lastmat == *mat)
			return;
	}
	
	//	glActiveTexture(GL_TEXTURE0);
	if(mat->textures.size() > 0)
		glBindTexture(GL_TEXTURE_2D, mat->textures[0]->texid);
	
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
		glEnable(GL_LIGHTING);
		
		if(mat->colors)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &mat->ambient.r);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &mat->diffuse.r);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mat->specular.r);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mat->shininess);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &mat->emissive.r);
		}
	}else
	{
		glDisable(GL_LIGHTING);
	}
	
	if(mat->alphatest)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, mat->alphatestvalue);
	}else
	{
		glDisable(GL_ALPHA_TEST);
	}
	
	if(mat->depthtest)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(mat->depthtestmode);
	}else
	{
		glDisable(GL_DEPTH_TEST);
	}
	
	lastmat = mat;
}

void sgRendererES1::renderObjects(sgCamera *cam, sgObject *first)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cam->matproj.mat);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cam->matview.mat);
	
	sgLight *light;
	unsigned int l = GL_LIGHT0;
	for(light = first_light->next; light != NULL; light = light->next)
	{
		glEnable(l);
		glLightfv(l, GL_AMBIENT,  &light->ambient.r);
		glLightfv(l, GL_DIFFUSE,  &light->diffuse.r);
		glLightfv(l, GL_SPECULAR, &light->specular.r);
		glLightfv(l, GL_POSITION, &light->position.x);
		glLightf(l, GL_QUADRATIC_ATTENUATION, light->attenuation);
		l++;
	}
	
	sgObject *curr;
	sgObjectBody *currbod;
	int i;
	for(curr = first->next; curr != NULL; curr = curr->next)
	{
		if((curr->tag == cam->tag && cam->tag != 0) || curr->culled)
			continue;
		
		curr->updateModel();
		currbod = curr->currbody;
		if(curr->sky)
		{
			sgMatrix4x4 viewmat = cam->rotation.getMatrix();
			viewmat.transpose();
			viewmat = matglobal3d*viewmat;
			glLoadMatrixf(viewmat.mat);
		}else
		{
			glLoadMatrixf(cam->matview.mat);
		}
		glMultMatrixf(curr->matmodel.mat);
		
		for(i = 0; i < currbod->meshs.size(); i++)
		{
			if(currbod->meshs[i]->culled)
				continue;
			
			setMaterial(currbod->materials[i]);
			
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(currbod->materials[i]->mattex.mat);
			glMatrixMode(GL_MODELVIEW);
			
			if(currbod->meshs[i]->vbo != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, currbod->meshs[i]->vbo);
				glVertexPointer(3, GL_FLOAT, sizeof(sgVertex), 0);
				glNormalPointer(GL_FLOAT, sizeof(sgVertex), (const void*)12);
				glTexCoordPointer(2, GL_FLOAT, sizeof(sgVertex), (const void*)24);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}else
			{
				glVertexPointer(3, GL_FLOAT, sizeof(sgVertex), &currbod->meshs[i]->vertices->position.x);
				glNormalPointer(GL_FLOAT, sizeof(sgVertex), &currbod->meshs[i]->vertices->normal.x);
				glTexCoordPointer(2, GL_FLOAT, sizeof(sgVertex), &currbod->meshs[i]->vertices->uv.x);
			}
			   
			if(currbod->meshs[i]->ivbo != -1)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currbod->meshs[i]->ivbo);
				glDrawElements(GL_TRIANGLES, currbod->meshs[i]->indexnum, GL_UNSIGNED_SHORT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}else
			{
				glDrawElements(GL_TRIANGLES, currbod->meshs[i]->indexnum, GL_UNSIGNED_SHORT, currbod->meshs[i]->indices);
			}
			
			if(currbod->meshs[i]->vertexformat == 1 || currbod->meshs[i]->vertexformat == 3)
			{
				glDisableClientState(GL_COLOR_ARRAY);
			}
		}
	}
}

void sgRendererES1::renderPanels(sgPanel *first)
{
	glViewport(0, 0, backingWidth, backingHeight);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glMatrixMode(GL_PROJECTION);
	sgMatrix4x4 matrix;
	matrix.makeProjectionOrtho(0, backingWidth, 0, backingHeight, -1, 1);
	glLoadMatrixf(matrix.mat);
	glMatrixMode(GL_MODELVIEW);
	
	glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
	glVertexPointer(3, GL_FLOAT, 20, 0);
	glTexCoordPointer(2, GL_FLOAT, 20, (const void*)12);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	sgImage *img;
	sgText *txt;
	for(sgPanel *curr = first->next; curr != NULL; curr = curr->next)
	{
		if(curr->rendertarget != NULL && currfbo != curr->rendertarget->fbo)
		{
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, curr->rendertarget->fbo);
			currfbo = curr->rendertarget->fbo;
		}else
		{
			if(msaasamples > 0)
			{
				if(currfbo != msaaFramebuffer)
				{
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFramebuffer);
					currfbo = msaaFramebuffer;
				}
			}else
			{
				if(currfbo != mainFramebuffer)
				{
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, mainFramebuffer);
					currfbo = mainFramebuffer;
				}
			}
		}
		
		glClearDepthf(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		for(int i = 0; i < curr->elements.size(); i++)
		{
			if(curr->elements[i]->type == 0)
			{
				if(curr->fixorientation)
					glLoadIdentity();
				else
					glLoadMatrixf(matglobal2d.mat);
				
				//draw image elements
				img = (sgImage*)curr->elements[i];
				glTranslatef(curr->pos.x+img->pos.x, curr->pos.y+img->pos.y, 0.0);
				glScalef(img->size.x, img->size.y, 1.0);
				glRotatef(360-img->ang, 0.0, 0.0, 1.0);
				
				setMaterial(img->mat);
			
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}else
			{
				//draw text elements
				txt = (sgText*)curr->elements[i];
				for(int n = 0; n < txt->str.str.size(); n++)
				{
					if(curr->fixorientation)
						glLoadIdentity();
					else
						glLoadMatrixf(matglobal2d.mat);
					
					glTranslatef(curr->pos.x+txt->pos.x+txt->size.x*n, curr->pos.y+txt->pos.y, 0.0);
					glScalef(txt->size.x, txt->size.y, 1.0);
					
					glMatrixMode(GL_TEXTURE);
					glLoadIdentity();
					glTranslatef((float)txt->str.translatedx[n]*(float)txt->charsize.x/(float)txt->mat->textures[0]->width, (float)txt->str.translatedy[n]*(float)txt->charsize.y/(float)txt->mat->textures[0]->height, 0);
					glScalef((float)txt->charsize.x/(float)txt->mat->textures[0]->width, (float)txt->charsize.y/(float)txt->mat->textures[0]->height, 1);
				
					setMaterial(txt->mat);
				
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					
					glLoadIdentity();
					glMatrixMode(GL_MODELVIEW);
				}
			}
		}
	}
	
	glEnableClientState(GL_NORMAL_ARRAY);
}

void sgRendererES1::render()
{
	//Clear color buffer
	if(msaasamples > 0)
	{
		if(currfbo != msaaFramebuffer)
		{
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFramebuffer);
			currfbo = msaaFramebuffer;
		}
	}else
	{
		if(currfbo != mainFramebuffer)
		{
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, mainFramebuffer);
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
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, cam->rendertarget->fbo);
			currfbo = cam->rendertarget->fbo;
		}else
		{
			if(msaasamples > 0)
			{
				if(currfbo != msaaFramebuffer)
				{
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFramebuffer);
					currfbo = msaaFramebuffer;
				}
			}else
			{
				if(currfbo != mainFramebuffer)
				{
					glBindFramebufferOES(GL_FRAMEBUFFER_OES, mainFramebuffer);
					currfbo = mainFramebuffer;
				}
			}
		}
		
		//Clear depth buffer
		glClearDepthf(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		//Update camera
		cam->updateView();
		cam->matview = matglobal3d*cam->matview;
		
		//Do view frustum culling
		culling(cam, first_solid);
		
		//Draw objects
		glViewport(cam->screenpos.x, cam->screenpos.y, cam->size.x, cam->size.y);
		renderObjects(cam, first_sky);
		renderObjects(cam, first_solid);
	}
	
	//Draw panels
	renderPanels(first_panel);
	
	if(msaasamples > 0)
	{
		//Resolve from msaaFramebuffer to resolveFramebuffer
		glDisable(GL_SCISSOR_TEST);     
		glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer);
		glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, mainFramebuffer);
		glResolveMultisampleFramebufferAPPLE();
		
		if(supportdiscard)
		{
			//discard framebuffer (a simple to use performence boost)
			GLenum attachments[] = { GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES};
			glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
		}
	}else
	{
		if(supportdiscard)
		{
			//discard framebuffer (a simple to use performence boost)
			GLenum attachments[] = {GL_DEPTH_ATTACHMENT_OES};
			glDiscardFramebufferEXT(GL_FRAMEBUFFER_OES, 1, attachments);
		}
	}
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
}
