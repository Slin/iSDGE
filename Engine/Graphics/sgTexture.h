//
//	sgTexture.h
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

#ifndef __SGTEXTURE_H__
#define __SGTEXTURE_H__

#include "sgBase.h"

#if defined __IOS__
    #include <OpenGLES/ES1/gl.h>
    #include <OpenGLES/ES1/glext.h>
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
    #include <CoreGraphics/CoreGraphics.h>
#elif defined __ANDROID__
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
#endif

class sgColorA;

/**
 * Texture class. Stores and handles a texture.
 */
class sgTexture : public sgBase
{
	public:
		/**
		 *	Constructor.
		 *	Initializes without a texture.
		 */
		sgTexture();

		/**
		 *	Deconstructor.
		 *	Frees all memory allocated by this class.
		 */
		~sgTexture();

		/**
		 *	Create Texture.
		 *	If there isn´t already a texture, this function loads it from the given file.
		 * @param filename the name of the file to load
		 * @param mipmaps has to be set to FALSE if you don´t want mipmaps to be created for this texture
		 * @param lock keeps the texture locked after creation if true.
		 */
		void createTexture(const char *filename, bool mipmaps = true, bool lock = false);

		/**
		 *	Create Texture.
		 *	If there isn´t already a texture, this function creates a black one with the given dimensions.
		 * @param width the width of the texture to create
		 * @param height the height of the texture to create
		 * @param cubemap will create this as a cubemap with 6 sides
		 */
		void createTexture(float width_, float height_, bool cubemap = false);

		/**
		 *	Create Texture from PVR file.
		 *	If there isn´t already a texture, this function loads it from a PVRTS compressed file.
		 * @param filename the name of the file to load, without the type
		 * @return pointer to the texture classes instance
		 */
		void createPVRTexture(const char *filename);

		/**
		 *	Make rendertarget.
		 *	Prepares the texture to render into it.
		 */
		void makeRendertarget();

		/**
		 *	Generate mipmaps.
		 *	Generate mipmaps if the type and format allows mipmaps.
		 */
		void generateMipmaps();

		/**
		 *	Get Texture.
		 *	Creates and returns a pointer to a texture loaded from the given file.
		 * @param filename the name of the file to load
		 * @param mipmaps has to be set to FALSE if you don´t want mipmaps to be created for this texture
		 * @param lock keeps the texture locked after creation if true.
		 * @param cubemap you are expected to add the 5 missing sides of the cube if this is true
		 * @return pointer to the texture classes instance
		 */
		static sgTexture *getTexture(const char *filename, bool mipmaps = true, bool lock = false);

		/**
		 * Get Texture.
		 * Creates and returns a pointer to a texture loaded from the given file.
		 * @param width the width of the texture to create
		 * @param height the height of the texture to create
		 * @param cubemap will create this as a cubemap with 6 sides
		 * @return pointer to the texture classes instance
		 */
		static sgTexture *getTexture(float width_, float height_, bool cubemap = false);

		/**
		 * Lock pixels.
		 * Creates an array of the texture pixels which is manipulatable using setPixels() and getPixels().
		 */
		void lockPixels();

		/**
		 * Update pixels.
		 * Updates the texture from locked pixels without unlocking. Only use this if you constantly change the texture during the game. Use unlockPixels() otherwize.
		 * @param swapped has to be true if the colors are meant to be updated as BGRA
		 */
		void updatePixels(bool swapped = false);

		/**
		 * Unlock pixels.
		 * Updates the texture from the array created in lockPixels() and delete that array.
		 */
		void unlockPixels();

		/**
		 * Set color.
		 * Changes the color of all pixels between a lock and unlock call.
		 * @param color new pixel color.
		 */
		void setColor(sgColorA color);

		/**
		 * Set pixel.
		 * Changes a texture pixel between a lock and unlock call.
		 * @param x horizontal pixel position.
		 * @param y vertical pixel position.
		 * @param color new pixel color.
		 */
		void setPixel(int x, int y, sgColorA color);

		/**
		 * Set pixels.
		 * Copies pixel data from an external source into the texture between a lock and unlock call.
		 * @param data pointer to the new data.
		 * @param offset offset in bytes on where to put the data into the texture.
		 * @param size number of bytes of the new data.
		 */
		void setPixels(unsigned char *data, unsigned int offset, unsigned int size);

		/**
		 * Get pixel.
		 * Returns a textures pixel color between a lock and unlock call.
		 * @param x horizontal pixel position.
		 * @param y vertical pixel position.
		 * @return color of the pixel.
		 */
		sgColorA getPixel(int x, int y);

		/**
		 * Get pixel.
		 * Returns a textures pixel color between a lock and unlock call.
		 * @param x horizontal pixel position.
		 * @param y vertical pixel position.
		 * @return interpolated color of the pixel.
		 */
		sgColorA getPixel(float x, float y);

		/**
		 * Set texture parameter int.
		 * Sets the specified OpenGL parameter for the texture.
		 * @param pname the parameter to set a new value for
		 * @param param the new value
		 */
		void setParameteri(unsigned int pname, unsigned int param);

		/**
		 *	Destroy.
		 *	Destroys the instance.
		 */
		void destroy();

		/**
		 *	Texture type.
		 *	The OpenGL type of the texture.
		 */
		unsigned int textype;

		/**
		 *	Texture ID.
		 *	The OpenGL ID of the texture.
		 */
		unsigned int texid;

		/**
		 *	Framebuffer object.
		 *	The framebuffer object, the texture is attached to. -1 if there is none.
		 */
		unsigned int fbo;

		/**
		 *	Width.
		 *	The textures width.
		 */
		unsigned int width;

		/**
		 *	Height.
		 *	The textures height.
		 */
		unsigned int height;

		/**
		 *	Format.
		 *	The textures format (f.e. GL_RGBA).
		 */
		unsigned int format;

		/**
		 *	Loaded.
		 *	TRUE, if this texture is ready to be used.
		 */
		bool loaded;

	private:
		unsigned int fbo_depth;
		unsigned char *texdata;
};

#endif
