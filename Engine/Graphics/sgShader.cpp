//
//	sgShader.cpp
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

#include "sgShader.h"

#include <string>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "sgResourceManager.h"
#include "sgDebug.h"

sgShader::sgShader()
{
	program = 0;
}

sgShader::sgShader(const char *vsfilename, const char *fsfilename)
{
	program = 0;
	create(vsfilename, fsfilename);
	maxlights = 2;
}

sgShader::~sgShader()
{
	
}

sgShader *sgShader::getShader(const char *vsfilename, const char *fsfilename)
{
	std::string str = std::string("shader")+std::string(vsfilename)+std::string(fsfilename);
	
	sgShader *shader = (sgShader*)sgResourceManager::getResource(str.c_str());
	if(shader != NULL)
		return shader;
	
	shader = new sgShader(vsfilename, fsfilename);
	sgResourceManager::addResource(str.c_str(), shader);
	
	return shader;
}

sgShader *sgShader::getShader(BuiltInShaders shad)
{
	sgShader *shader = NULL;
	switch(shad)
	{
		//Basic shaders
		case BIS_TEXTURE:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_texture");
			if(shader == NULL)
			{
				shader = new sgShader("sgsTexture.vsh", "sgsTexture.fsh");
				sgResourceManager::addResource("shader_texture", shader);
			}
			break;
		}
		case BIS_TEXTURE_DISCARD:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_texture_discard");
			if(shader == NULL)
			{
				shader = new sgShader("sgsTexture.vsh", "sgsTextureDiscard.fsh");
				sgResourceManager::addResource("shader_texture_discard", shader);
			}
			break;
		}
		case BIS_SUN:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_sun");
			if(shader == NULL)
			{
				shader = new sgShader("sgsSun.vsh", "sgsSun.fsh");
				sgResourceManager::addResource("shader_sun", shader);
			}
			break;
		}
		case BIS_LIGHT:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_light");
			if(shader == NULL)
			{
				shader = new sgShader("sgsLight.vsh", "sgsLight.fsh");
				sgResourceManager::addResource("shader_light", shader);
			}
			break;
		}
		case BIS_LIGHTMAP:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_lightmap");
			if(shader == NULL)
			{
				shader = new sgShader("sgsLightmap.vsh", "sgsLightmap.fsh");
				sgResourceManager::addResource("shader_lightmap", shader);
			}
			break;
		}
			
		//Special shaders
		case BIS_SHADOWVOLUME:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_shadowvolume");
			if(shader == NULL)
			{
				shader = new sgShader("sgsShadowvolume.vsh", "sgsShadowvolume.fsh");
				sgResourceManager::addResource("shader_shadowvolume", shader);
			}
			break;
		}
		case BIS_SHADOWQUAD:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_shadowquad");
			if(shader == NULL)
			{
				shader = new sgShader("sgsShadowquad.vsh", "sgsShadowquad.fsh");
				sgResourceManager::addResource("shader_shadowquad", shader);
			}
			break;
		}
		case BIS_PARTICLE:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_particle");
			if(shader == NULL)
			{
				shader = new sgShader("sgsParticle.vsh", "sgsParticle.fsh");
				sgResourceManager::addResource("shader_particle", shader);
			}
			break;
		}
			
		default:
		{
			shader = (sgShader*)sgResourceManager::getResource("shader_texture");
			if(shader == NULL)
			{
				shader = new sgShader("sgsTexture.vsh", "sgsTexture.fsh");
				sgResourceManager::addResource("shader_texture", shader);
			}
			break;
		}
	}
	return shader;
}

bool sgShader::compileShader(GLuint *shader, GLenum type, const char *filepath)
{
	GLint status;
	const GLchar *source;
	
	source = (GLchar *)sgResourceManager::getFileAsString(filepath);
	if(!source)
	{
		sgLog("Failed to load shader");
		return false;
	}
	
	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &source, NULL);
	glCompileShader(*shader);
	
	delete[] source;
	
	GLint logLength;
	glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength > 0)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetShaderInfoLog(*shader, logLength, &logLength, log);
		sgLog("%s: Shader compile log:\n %s", filepath, log);
		free(log);
	}
	
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		glDeleteShader(*shader);
		return false;
	}
	
	return true;
}

bool sgShader::linkProgram(GLuint prog)
{
	GLint status;
	
	glLinkProgram(prog);
	
	GLint logLength;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength > 0)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(prog, logLength, &logLength, log);
		sgLog("Program link log:\n &s", log);
		free(log);
	}
	
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if(status == 0)
		return false;
	
	return true;
}

bool sgShader::validateProgram()
{
	GLint logLength, status;
	
	glValidateProgram(program);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength > 0)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(program, logLength, &logLength, log);
		sgLog("Program validate log:\n%s", log);
		free(log);
	}
	
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if(status == 0)
		return false;
	
	return true;
}

void sgShader::getEngineUniforms()
{
	matproj = glGetUniformLocation(program, "matProj");
	matview = glGetUniformLocation(program, "matView");
	matmodel = glGetUniformLocation(program, "matModel");
	matprojviewmodel = glGetUniformLocation(program, "matProjViewModel");
	matnormal = glGetUniformLocation(program, "matNormal");
	mattex = glGetUniformLocation(program, "matTex");
	
	position = glGetAttribLocation(program, "vertPos");
	normal = glGetAttribLocation(program, "vertNormal");
	texcoord0 = glGetAttribLocation(program, "vertTexcoord0");
	texcoord1 = glGetAttribLocation(program, "vertTexcoord1");
	color = glGetAttribLocation(program, "vertColor");
	tangent = glGetAttribLocation(program, "vertTangent");
	
	mambientloc = glGetUniformLocation(program, "mAmbient");
	mdiffuseloc = glGetUniformLocation(program, "mDiffuse");
	mspecularloc = glGetUniformLocation(program, "mSpecular");
	mshininessloc = glGetUniformLocation(program, "mShininess");
	memissiveloc = glGetUniformLocation(program, "mEmissive");
	malphatestvalue = glGetUniformLocation(program, "mAlphaTest");
	
	fcolor = glGetUniformLocation(program, "fColor");
	fstartend = glGetUniformLocation(program, "fStartEnd");
	
	lambientloc = glGetUniformLocation(program, "lAmbient");
	ldiffuseloc = glGetUniformLocation(program, "lDiffuse");
	lspecularloc = glGetUniformLocation(program, "lSpecular");
	lpositionloc = glGetUniformLocation(program, "lPosition");
	lattenuationloc = glGetUniformLocation(program, "lAttenuation");
	
	time = glGetUniformLocation(program, "Time");
	vposition = glGetUniformLocation(program, "vPosition");
}

bool sgShader::create(const char *vsfilename, const char *fsfilename)
{
	if(program != 0)
		return false;
	
	GLuint vertShader, fragShader;
	const char *vertShaderPathname, *fragShaderPathname;
	
	// Create shader program
	program = glCreateProgram();
	
	// Create and compile vertex shader
	vertShaderPathname = sgResourceManager::getPath(vsfilename);
	if(!compileShader(&vertShader, GL_VERTEX_SHADER, vertShaderPathname))
	{
		sgLog("%s: Failed to compile vertex shader", vertShaderPathname);
		delete[] vertShaderPathname;
		return false;
	}
	delete[] vertShaderPathname;
	
	// Create and compile fragment shader
	fragShaderPathname = sgResourceManager::getPath(fsfilename);
	if(!compileShader(&fragShader, GL_FRAGMENT_SHADER, fragShaderPathname))
	{
		sgLog("%s: Failed to compile fragment shader", fragShaderPathname);
		delete[] fragShaderPathname;
		return false;
	}
	delete[] fragShaderPathname;
	
	// Attach vertex shader to program
	glAttachShader(program, vertShader);
	
	// Attach fragment shader to program
	glAttachShader(program, fragShader);
	
	// Link program
	if(!linkProgram(program))
	{
		sgLog("Failed to link program: %i", program);
		
		if(vertShader)
		{
			glDeleteShader(vertShader);
			vertShader = 0;
		}
		if(fragShader)
		{
			glDeleteShader(fragShader);
			fragShader = 0;
		}
		if(program)
		{
			glDeleteProgram(program);
			program = 0;
		}
		
		return false;
	}
	
	if(vertShader)
		glDeleteShader(vertShader);
	
	if(fragShader)
		glDeleteShader(fragShader);
	
	getEngineUniforms();
				
	return true;
}

void sgShader::destroy()
{
	if(program)
	{
		glDeleteProgram(program);
		program = 0;
	}
	
	sgResourceManager::removeResource(this);
	delete this;
}
