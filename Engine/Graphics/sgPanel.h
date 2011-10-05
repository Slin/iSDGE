//
//	sgPanel.h
//	Engine
//
//	Created by Nils Daumann on 25.04.10.
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

#ifndef __SGPANEL_H__
#define __SGPANEL_H__

#include <vector>
#include <string>

#include "sgVector2.h"

class sgMaterial;
class sgTexture;

/**
 * Display string class. Translates it string to a format it can be displayed with.
 */
class sgDisplayString
{
	public:
		/**
		 *	Translate.
		 *	Translates the string str to the characters offsets on the font texture.
		 */
		void translate();
	
		/**
		 *	String.
		 *	The string to translate.
		 */
		std::string str;
	
		std::vector<unsigned int> translatedx;
		std::vector<unsigned int> translatedy;
};

/**
 * Panel element class. This is the base class of each element that can be displayed in a panel.
 */
class sgPanelElement
{
	public:
		/**
		 *	Type.
		 *	The type of the panel element.
		 */
		unsigned int type;
	
		/**
		 *	Position.
		 *	The elements position relative to the panels bottom left corner in pixels.
		 */
		sgVector2 pos;
	
		/**
		 *	Size.
		 *	The elements size in pixels.
		 */
		sgVector2 size;
	
		/**
		 *	Material.
		 *	The material used by this element.
		 */
		sgMaterial *mat;
};

/**
 * Image class. This class represents image panel elements.
 */
class sgImage : public sgPanelElement
{
	public:
		/**
		 *	Angle.
		 *	The images rotation angle around its center in degrees.
		 */
		float ang;
};

/**
 * Text class. This class represents text panel elements.
 */
class sgText : public sgPanelElement
{
	public:
		/**
		 *	String.
		 *	The translated string to display.
		 */
		sgDisplayString str;
	
		/**
		 *	Character size.
		 *	The size in pixels of one character in the texture.
		 */
		sgVector2 charsize;
};

/**
 * Panel class. This class represents a panel which can contain any number of elements.
 */
class sgPanel
{
	public:
		/**
		 *	Constructor.
		 *	Initializes a panel instance.
		 * @param p pointer to the previous panel in the list or NULL
		 * @param n pointer to the next panel in the list or NULL
		 */
		sgPanel(sgPanel* p, sgPanel *n);
	
		/**
		 *	Deconstructor.
		 *	The panels deconstructor.
		 */
		~sgPanel();
		
		/**
		 *	Create panel.
		 *	Creates a new panel and puts it into the list of this one, directly behind this one.
		 * @return pointer to the new panel instance
		 */
		sgPanel *createPanel();
		
		/**
		 *	Add image element.
		 *	Adds a new image element to the panel.
		 * @param imgtexfile file name of the image to display
		 * @param pos position of the images bottom left corner in pixels, relative to the panel ones
		 * @param size the size in pixels to display the image with
		 * @param vs file name of the vertex shader of the type .vsh without the type specifier or NULL to use the default shader
		 * @param fs file name of the fragment shader of the type .fsh without the type specifier or NULL to use the default shader
		 * @return pointer to the added element.
		 */
		sgPanelElement *addImage(const char *imgtexfile, const sgVector2 &pos, const sgVector2 &size, const char *vs = NULL, const char *fs = NULL);
	
		/**
		 *	Add image element.
		 *	Adds a new image element to the panel.
		 * @param mat pointer to a materials instance
		 * @param pos position of the images bottom left corner in pixels, relative to the panel ones
		 * @param size the size in pixels to display the image with
		 * @return pointer to the added element.
		 */
		sgPanelElement *addImage(sgMaterial *mat, const sgVector2 &pos, const sgVector2 &size);
	
		/**
		 *	Add image element.
		 *	Adds a new image element to the panel.
		 * @param tex pointer to a textures instance
		 * @param pos position of the images bottom left corner in pixels, relative to the panel ones
		 * @param size the size in pixels to display the image with
		 * @param vs file name of the vertex shader of the type .vsh without the type specifier or NULL to use the default shader
		 * @param fs file name of the fragment shader of the type .fsh without the type specifier or NULL to use the default shader
		 * @return pointer to the added element.
		 */
		sgPanelElement *addImage(sgTexture *tex, const sgVector2 &pos, const sgVector2 &size, const char *vs = NULL, const char *fs = NULL);
	
		/**
		 *	Add text element.
		 *	Adds a new text element to the panel.
		 * @param str the string to display
		 * @param charsize size in pixels of one character on the font texture
		 * @param pos position of the texts bottom left corner in pixels, relative to the panel ones
		 * @param size the size in pixels to display each character with
		 * @param fonttexfile file name of the font texture to use
		 * @param vs file name of the vertex shader of the type .vsh without the type specifier or NULL to use the default shader
		 * @param fs file name of the fragment shader of the type .fsh without the type specifier or NULL to use the default shader
		 * @return pointer to the added element.
		 */
		sgPanelElement *addText(const char *str, const sgVector2 &charsize, const sgVector2 &pos, const sgVector2 &size, const char *fonttexfile, const char *vs = NULL, const char *fs = NULL);
	
		/**
		 *	Remove panel element.
		 *	Removes the panel element from the panel and destroys it.
		 * @param element the element to remove
		 */
		void removeElement(sgPanelElement *element);
	
		/**
		 *	Remove all panel elements.
		 *	Removes and destroys all elements of this panel.
		 */
		void removeAllElements();
		
		/**
		 *	Destroy.
		 *	Destroys the panel and removes it from the list.
		 */
		void destroy();
	
		/**
		 *	Destroy all.
		 *	Destroys all panels in the list.
		 */
		void destroyAll();
		
		/**
		 *	Position.
		 *	Position of the panel in pixels relative to the screens bottom left corner.
		 */
		sgVector2 pos;
		
		/**
		 *	Elements.
		 *	An array storing the pointers to all panel elements.
		 */
		std::vector<sgPanelElement*> elements;
	
		/**
		 *	Fix orientation.
		 *	The panel isn´t rotated with the devices orientation if this is true.
		 */
		bool fixorientation;
	
		/**
		 *	Rendertarget.
		 *	The target texture, this view has to render into. Assign a prepared texture here or NULL to deactivate rendering into a texture.
		 */
		sgTexture *rendertarget;
		
		/**
		 *	Previous.
		 *	Pointer to the previous panel in the list or NULL if this is the first one.
		 */
		sgPanel *prev;
	
		/**
		 *	Next.
		 *	Pointer to the next panel in the list or NULL if this is the last one.
		 */
		sgPanel *next;
};

#endif
