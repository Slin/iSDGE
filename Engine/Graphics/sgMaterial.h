//
//	sgMaterial.h
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

#ifndef __SGMATERIAL_H__
#define __SGMATERIAL_H__

#include <vector>
#include <string>
#include "sgBase.h"
#include "sgColor.h"
#include "sgMatrix4x4.h"
#include "sgShader.h"
#include "sgTexture.h"

class sgMaterialParameter
{
	public:
		bool operator== (const sgMaterialParameter &other)
		{
			if(location == other.location && parameter == other.parameter)
				return true;
			
			return false;
		}
		
		bool operator!= (const sgMaterialParameter &other)
		{
			if(location != other.location || parameter != other.parameter)
				return true;
			
			return false;
		}
		
		std::string name;
		unsigned int location;
		
		void *parameter;
		unsigned int datatype;
		unsigned int size;
		unsigned int count;
};

/**
 * Material class. Stores and handles a material.
 */
class sgMaterial : public sgBase
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a default material.
		 */
		sgMaterial();
		
		/**
		 *	Constructor.
		 *	Initializes a default material with a default shader.
		 * @param shad the default shader to use
		 */
		sgMaterial(sgShader::BuiltInShaders shad);
	
		/**
		 *	Constructor.
		 *	Initializes a default material with a shader loaded from the given files.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 */
		sgMaterial(const char *vsname, const char *fsname);
	
		/**
		 *	Constructor.
		 *	Initializes a default material with a texture loaded from the given file.
		 * @param texfile the name of the texture file to load
		 * @param mipmaps has to be set to false if you don´t want mipmaps to be created for the texture
		 * @param shad the default shader to use
		 */
		sgMaterial(const char *texfile, bool mipmaps = true, sgShader::BuiltInShaders shad = sgShader::BIS_TEXTURE);
	
		/**
		 *	Constructor.
		 *	Initializes a default material with a texture loaded from the given file.
		 * @param tex pointer to the textures instance
		 * @param shad the default shader to use
		 */
		sgMaterial(sgTexture *tex, sgShader::BuiltInShaders shad = sgShader::BIS_TEXTURE);
	
		/**
		 *	Constructor.
		 *	Initializes a default material with a texture and a shader loaded from the given files.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 * @param texfile the name of the texture file to load
		 * @param mipmaps has to be set to false if you don´t want mipmaps to be created for the texture
		 */
		sgMaterial(const char *vsname, const char *fsname, const char *texfile, bool mipmaps = true);
	
		/**
		 *	Constructor.
		 *	Initializes a default material with a texture and a shader loaded from the given files.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 * @param tex pointer to the textures instance
		 */
		sgMaterial(const char *vsname, const char *fsname, sgTexture *tex);
	
		/**
		 *	Destructor.
		 *	Frees the parameters.
		 */
		~sgMaterial();
	
		/**
		 *	== operator.
		 *	Overloaded == operator to compare two instances of this class.
		 */
		bool operator== (const sgMaterial &other);
	
		/**
		 *	Set to default.
		 *	Sets all material parameters to the default ones (textures and shader aren´t effected).
		 */
		void setDefault();
	
		/**
		 *	Set shader.
		 *	Assigns a new shader to this material.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 */
		void setShader(const char *vsname, const char *fsname);
	
		/**
		 *	Set shader.
		 *	Assigns a default shader to this material.
		 * @param shad the default shaders id
		 */
		void setShader(sgShader::BuiltInShaders shad);
	
		/**
		 *	Set 2D texture.
		 *	Adds or replaces a texture.
		 * @param tex index of the texture to replace, or -1 to add a new one
		 * @param texfile the name of the texture file to set
		 * @param mipmaps activates or deactivates mipmaps for the texture
		 */
		void setTexture2D(unsigned int tex, const char *texfile, bool mipmaps = true);
	
		/**
		 *	Set 2D texture.
		 *	Adds or replaces a texture.
		 * @param tex index of the texture to replace, or -1 to add a new one
		 * @param texptr pointer to the textures instance
		 * @param mipmaps activates or deactivates mipmaps for the texture
		 */
		void setTexture2D(unsigned int tex, sgTexture *texptr);
	
		/**
		 *	Get uniforms.
		 *	Updates the uniform locations handled by this shader. Has for example to be called after adding a new texture.
		 */
		void getUniforms();
	
		/**
		 *	Add parameter.
		 *	Adds a custom parameter, which will be sent to the shader. The pointer is freed automatically!
		 * @param name the uniform name within the shader
		 * @param ptr pointer to the parameter
		 * @param datatype 0 for float or 1 for int
		 * @param size 0 to 3 for 1 to 4 elements, 4 to 6 for mat2, 3 or 4
		 * @param count number of parameters with size, usually 1, but higher if you want it as an array in your shader
		 * @return pointer to the parameter object
		 */
		sgMaterialParameter *addParameter(const char *name, void *ptr, unsigned int datatype = 0, unsigned int size = 3, unsigned int count = 1);
	
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material.
		 */
		static sgMaterial *getMaterial();
	
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material with a default shader.
		 * @param shad the default shader to use
		 */
		static sgMaterial *getMaterial(sgShader::BuiltInShaders shad);
		
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material with a shader loaded from the given files.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 */
		static sgMaterial *getMaterial(const char *vsname, const char *fsname);
		
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material with a texture loaded from the given file.
		 * @param texfile the name of the texture file to load
		 * @param mipmaps has to be set to false if you don´t want mipmaps to be created for the texture
		 * @param shad the default shader to use
		 */
		static sgMaterial *getMaterial(const char *texfile, bool mipmaps = true, sgShader::BuiltInShaders shad = sgShader::BIS_TEXTURE);
	
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material with a texture loaded from the given file.
		 * @param tex pointer to the textures instance
		 * @param shad the default shader to use
		 */
		static sgMaterial *getMaterial(sgTexture *tex, sgShader::BuiltInShaders shad = sgShader::BIS_TEXTURE);
		
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material with a texture and a shader loaded from the given files.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 * @param texfile the name of the texture file to load
		 * @param mipmaps has to be set to false if you don´t want mipmaps to be created for the texture
		 */
		static sgMaterial *getMaterial(const char *vsname, const char *fsname, const char *texfile, bool mipmaps = true);
	
		/**
		 *	Get material.
		 *	Creates, initializes and returns a default material with a texture and a shader loaded from the given files.
		 * @param vsname the name of the vertex shader file to load
		 * @param fsname the name of the fragment shader file to load
		 * @param tex pointer to the textures instance
		 */
		static sgMaterial *getMaterial(const char *vsname, const char *fsname, sgTexture *tex);
	
		/**
		 *	Destroy.
		 *	Destroys the instance.
		 */
		void destroy();
	
		/**
		 *	Shader.
		 *	The shader used for this material.
		 */
		sgShader *shader;
	
		/**
		 *	Textures.
		 *	The array of the textures used for this material.
		 */
		std::vector<sgTexture*> textures;
	
		/**
		 *	Texture location.
		 *	An array of the location of each texture within the shader.
		 */
		std::vector<unsigned int> texlocation;
	
		/**
		 *	Material parameters.
		 *	An array of custom material parameters which will be sent to the shader.
		 */
		std::vector<sgMaterialParameter> parameters;
	
		/**
		 *	Culling.
		 *	TRUE to activate culling (default), FALSE otherwise.
		 */
		bool culling;
	
		/**
		 *	Cullmode.
		 *	GL_CW for clockwise culling, GL_CCW for counter clockwise culling (default).
		 */
		unsigned int cullmode;
	
		/**
		 *	Blending.
		 *	TRUE to activate blending, FALSE otherwise (default).
		 */
		bool blending;
	
		/**
		 *	Blend source.
		 *	Blendmode for the source.
		 */
		unsigned int blendsource;
	
		/**
		 *	Blend destination.
		 *	Blendmode for the destination.
		 */
		unsigned int blenddestination;
	
		/**
		 *	Lighting.
		 *	TRUE to activate lighting (default), FALSE otherwise.
		 */
		bool lighting;
	
		/**
		 *	Colors.
		 *	TRUE to use the material parameters for lighting (default), FALSE otherwise. Ignored if lighting is deactivated and only an opengl version of 1 is supported.
		 */
		bool colors;
	
		/**
		 *	Ambient.
		 *	Color used for the ambient lighting.
		 */
		sgFColorA ambient;
	
		/**
		 *	Diffuse.
		 *	Color used for the diffuse lighting.
		 */
		sgFColorA diffuse;
	
		/**
		 *	Specular.
		 *	Color used for the specular lighting.
		 */
		sgFColorA specular;
	
		/**
		 *	Shininess.
		 *	Specular shininess.
		 */
		float shininess;
	
		/**
		 *	Emissive.
		 *	Color used for the emissive lighting.
		 */
		sgFColorA emissive;
	
		/**
		 *	Alphatest.
		 *	TRUE to activate alpha testing, FALSE otherwise (default).
		 */
		bool alphatest;
	
		/**
		 *	Alphatestvalue.
		 *	Threshold used for the alphatest (all pixels with a lower alpha value are clipped away by default).
		 */
		float alphatestvalue;
	
		/**
		 *	Depthtest.
		 *	True to activate depth testing (default), false otherwise.
		 */
		bool depthtest;
	
		/**
		 *	Depthwrite.
		 *	True to activate depth writing (default), false otherwise.
		 */
		bool depthwrite;
	
		/**
		 *	Depthtest mode.
		 *	Default is GL_LEQUAL, to only draw pixels, not covered by others.
		 */
		unsigned int depthtestmode;
	
		/**
		 *	Texture matrix.
		 *	A matrix transforming the texture coordinates of the mesh the material is assigned to.
		 */
		sgMatrix4x4 mattex;
};

#endif
