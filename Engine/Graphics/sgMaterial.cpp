//
//	sgMaterial.cpp
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

#include "sgMaterial.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "sgResourceManager.h"
#include "sgShader.h"
#include "sgRenderer.h"

sgMaterial::sgMaterial()
{
	shader = NULL;
	setDefault();
}

sgMaterial::sgMaterial(sgShader::BuiltInShaders shad)
{
	if(sgRenderer::oglversion > 1)
	{
		shader = sgShader::getShader(shad);
	}else
	{
		shader = NULL;
	}
	setDefault();
	
	if(shad == 0)
		lighting = false;
}

sgMaterial::sgMaterial(const char *vsname, const char *fsname)
{
	if(sgRenderer::oglversion > 1)
	{
		shader = sgShader::getShader(vsname, fsname);
	}else
	{
		shader = NULL;
	}
	setDefault();
}

sgMaterial::sgMaterial(const char *texfile, bool mipmaps, sgShader::BuiltInShaders shad)
{
	if(sgRenderer::oglversion > 1)
	{
		shader = sgShader::getShader(shad);
	}else
	{
		shader = NULL;
	}
	setDefault();
	
	if(shad == 0)
		lighting = false;

	textures.push_back(sgTexture::getTexture(texfile, mipmaps));
	setDefault();
}

sgMaterial::sgMaterial(sgTexture *tex, sgShader::BuiltInShaders shad)
{
	if(sgRenderer::oglversion > 1)
	{
		shader = sgShader::getShader(shad);
	}else
	{
		shader = NULL;
	}
	setDefault();
	
	if(shad == 0)
		lighting = false;
	
	textures.push_back(tex);
	setDefault();
}

sgMaterial::sgMaterial(const char *vsname, const char *fsname, const char *texfile, bool mipmaps)
{
	if(sgRenderer::oglversion > 1)
	{
		shader = sgShader::getShader(vsname, fsname);
	}else
	{
		shader = NULL;
	}

	textures.push_back(sgTexture::getTexture(texfile, mipmaps));
	setDefault();
}

sgMaterial::sgMaterial(const char *vsname, const char *fsname, sgTexture *tex)
{
	if(sgRenderer::oglversion > 1)
	{
		shader = sgShader::getShader(vsname, fsname);
	}else
	{
		shader = NULL;
	}
	
	textures.push_back(tex);
	setDefault();
}

sgMaterial::~sgMaterial()
{
	for(int i = 0; i < parameters.size(); i++)
	{
		if(parameters[i].datatype == 0)
		{
			delete[] (float*)parameters[i].parameter;
		}else
		{
			delete[] (int*)parameters[i].parameter;
		}
	}
}

bool sgMaterial::operator== (const sgMaterial &other)
{
	if(parameters.size() != other.parameters.size())
		return false;
	
	for(int i = 0; i < parameters.size(); i++)
	{
		if(parameters[i] != other.parameters[i])
			return false;
	}
	
	if(shader == other.shader && textures == other.textures && culling == other.culling && cullmode == other.cullmode &&
		blending == other.blending && blendsource == other.blendsource && blenddestination == other.blenddestination&& lighting == other.lighting && colors == other.colors &&
		ambient == other.ambient && diffuse == other.diffuse && specular == other.specular && shininess == other.shininess && emissive == other.emissive && alphatest == other.alphatest &&
		alphatestvalue == other.alphatestvalue && depthtest == other.depthtest && depthtestmode == other.depthtestmode && mattex == other.mattex)
	{
		return true;
	}
	
	return false;
}

void sgMaterial::setDefault()
{
	culling = true;
	cullmode = GL_CCW;
	
	blending = false;
	blendsource = GL_ONE;
	blenddestination = GL_ONE_MINUS_SRC_ALPHA;
	
	lighting = true;
	colors = true;
	ambient.r = 0.2;
	ambient.g = 0.2;
	ambient.b = 0.2;
	ambient.a = 1.0;
	diffuse.r = 0.8;
	diffuse.g = 0.8;
	diffuse.b = 0.8;
	diffuse.a = 1.0;
	specular.r = 0.0;
	specular.g = 0.0;
	specular.b = 0.0;
	specular.a = 1.0;
	shininess = 0;
	emissive.r = 0.0;
	emissive.g = 0.0;
	emissive.b = 0.0;
	emissive.a = 1.0;
	
	alphatest = false;
	alphatestvalue = 0.75;
	
	depthtest = true;
	depthtestmode = GL_LEQUAL;
	depthwrite = true;
	
	mattex.makeIdentity();
	
	if(sgRenderer::oglversion > 1)
	{
		if(shader == NULL)
			shader = sgShader::getShader(sgShader::BIS_TEXTURE);
		
		getUniforms();
	}else
	{
		shader = NULL;
	}

}

void sgMaterial::setShader(const char *vsname, const char *fsname)
{
	if(sgRenderer::oglversion <= 1)
		return;
	
	shader = sgShader::getShader(vsname, fsname);
	getUniforms();
}

void sgMaterial::setShader(sgShader::BuiltInShaders shad)
{
	if(sgRenderer::oglversion <= 1)
		return;
	
	shader = sgShader::getShader(shad);
	getUniforms();
}

void sgMaterial::setTexture(unsigned int tex, const char *texfile, bool mipmaps)
{
	if(tex == -1 || tex >= textures.size())
	{
		textures.push_back(sgTexture::getTexture(texfile, mipmaps));
		getUniforms();
		return;
	}
	
	if(tex >= textures.size())
		return;
	
	textures[tex] = sgTexture::getTexture(texfile, mipmaps);
	getUniforms();
}

void sgMaterial::setTexture(unsigned int tex, sgTexture *texptr)
{
	if(tex == -1)
	{
		textures.push_back(texptr);
		getUniforms();
		return;
	}
	
	if(tex > textures.size())
		return;
	
	textures[tex] = texptr;
	getUniforms();
}

void sgMaterial::getUniforms()
{
	if(shader == NULL)
		return;
	
	char str[10];
	texlocation.clear();
	for(int i = 0; i < textures.size(); i++)
	{
		sprintf(str, "mTexture%i", i);
		texlocation.push_back(glGetUniformLocation(shader->program, str));
	}
	
	for(int i = 0; i < parameters.size(); i++)
	{
		parameters[i].location = glGetUniformLocation(shader->program, parameters[i].name.c_str());
	}
}

sgMaterialParameter *sgMaterial::addParameter(const char *name, void *ptr, unsigned int datatype, unsigned int size, unsigned int count)
{
	sgMaterialParameter param;
	param.parameter = ptr;
	param.name = std::string(name);
	
	if(sgRenderer::oglversion > 1)
		param.location = glGetUniformLocation(shader->program, name);
	
	param.datatype = datatype;
	param.size = size;
	param.count = count;
	parameters.push_back(param);
	
	return &parameters[parameters.size()-1];
}

sgMaterial *sgMaterial::getMaterial()
{
	sgMaterial *mat = new sgMaterial();
	sgResourceManager::addResource(mat);
	return mat;
}

sgMaterial *sgMaterial::getMaterial(sgShader::BuiltInShaders shad)
{
	sgMaterial *mat = new sgMaterial(shad);
	sgResourceManager::addResource(mat);
	return mat;
}

sgMaterial *sgMaterial::getMaterial(const char *vsname, const char *fsname)
{
	sgMaterial *mat = new sgMaterial(vsname, fsname);
	sgResourceManager::addResource(mat);
	return mat;
}

sgMaterial *sgMaterial::getMaterial(const char *texfile, bool mipmaps, sgShader::BuiltInShaders shad)
{
	sgMaterial *mat = new sgMaterial(texfile, mipmaps, shad);
	sgResourceManager::addResource(mat);
	return mat;
}

sgMaterial *sgMaterial::getMaterial(sgTexture *tex, sgShader::BuiltInShaders shad)
{
	sgMaterial *mat = new sgMaterial(tex, shad);
	sgResourceManager::addResource(mat);
	return mat;
}
	
sgMaterial *sgMaterial::getMaterial(const char *vsname, const char *fsname, const char *texfile, bool mipmaps)
{
	sgMaterial *mat = new sgMaterial(vsname, fsname, texfile, mipmaps);
	sgResourceManager::addResource(mat);
	return mat;
}

sgMaterial *sgMaterial::getMaterial(const char *vsname, const char *fsname, sgTexture *tex)
{
	sgMaterial *mat = new sgMaterial(vsname, fsname, tex);
	sgResourceManager::addResource(mat);
	return mat;
}

void sgMaterial::destroy()
{
	sgResourceManager::removeResource(this);
	delete this;
}
