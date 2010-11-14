//
//	sgShader.h
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

#ifndef __SGSHADER_H__
#define __SGSHADER_H__

#include "sgBase.h"

/**
 * Shader class. This class loads, compiles and represents shaders.
 */
class sgShader : public sgBase
{
	public:
		/**
		 *	Constructor.
		 *	Sets up an instance of this class without a shader.
		 */
		sgShader();
		
		/**
		 *	Constructor.
		 *	Sets up an instance of this class and assigns a shader programm compiled from the given files.
		 * @param vsfilename name of the textfile containing the vertex shader of the type .vsh, without the type specifier
		 * @param fsfilename name of the textfile containing the fragment shader of the type .fsh, without the type specifier
		 */
		sgShader(const char *vsfilename, const char *fsfilename);
	
		/**
		 *	Deconstructor.
		 *	The deconstructor.
		 */
		~sgShader();
		
		/**
		 *	Get shader.
		 *	Checks, if there is a shader of the given filenames or create a new one otherwize.
		 * @param vsfilename name of the textfile containing the vertex shader of the type .vsh, without the type specifier
		 * @param fsfilename name of the textfile containing the fragment shader of the type .fsh, without the type specifier
		 * @return pointer to the shaders instance
		 */
		static sgShader *getShader(const char *vsfilename, const char *fsfilename);
		
		/**
		 *	Get shader.
		 *	Returns the pointer to an instance of a default shader identified by the given number.
		 * @param shad id of the default shader to return (0 for no shading, 1 for shading)
		 * @return pointer to the shaders instance
		 */
		static sgShader *getShader(unsigned int shad);
		
		/**
		 *	Destroy.
		 *	Destroys the instance.
		 */
		void destroy();
		
		/**
		 *	Validate shader program.
		 *	Checks if the shader is setup correctly.
		 * @return TRUE if everything is fine, FALSE otherwize
		 */
		bool validateProgram();
		
		/**
		 *	Number of dynamic lights.
		 *	The number of dynamic lights handled by this shader. Default 2.
		 */
		unsigned int maxlights;
		
		/**
		 *	Shader program.
		 *	The id of the OpenGL shader program.
		 */
		unsigned int program;
		
		
		unsigned int matproj;
		unsigned int matview;
		unsigned int matmodel;
		unsigned int matprojviewmodel;
		unsigned int matnormal;
		unsigned int mattex;
	
		unsigned int position;
		unsigned int normal;
		unsigned int texcoord0;
		unsigned int color;
	
		unsigned int mambientloc;
		unsigned int mdiffuseloc;
		unsigned int mspecularloc;
		unsigned int mshininessloc;
		unsigned int memissiveloc;
		unsigned int malphatestvalue;
	
		unsigned int lambientloc;
		unsigned int ldiffuseloc;
		unsigned int lspecularloc;
		unsigned int lpositionloc;
		unsigned int lattenuationloc;
	
	private:
		bool create(const char *vsfilename, const char *fsfilename);
		bool compileShader(unsigned int *shader, unsigned int type, const char *filepath);
		bool linkProgram(unsigned int prog);
		void getEngineUniforms();
};

#endif
