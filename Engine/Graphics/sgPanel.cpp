//
//	sgPanel.cpp
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

#include "sgPanel.h"

#include "sgMaterial.h"

void sgDisplayString::translate()
{
	translatedx.clear();
	translatedy.clear();
	for(int i = 0; i < str.size(); i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			translatedx.push_back(2+(str[i]-65)*2);
			translatedy.push_back(0);
		}
			
		if(str[i] >= 'a' && str[i] <= 'z')
		{
			translatedx.push_back(1+(str[i]-97)*2);
			translatedy.push_back(0);
		}
			
		if(str[i] >= '0' && str[i] <= '9')
		{
			translatedx.push_back(25+(str[i]-48));
			translatedy.push_back(1);
		}
		
		if(str[i] == ' ')
		{
			translatedx.push_back(0);
			translatedy.push_back(0);
		}
		
		if(str[i] == ' ')
		{
			translatedx.push_back(0);
			translatedy.push_back(0);
		}
		
		if(str[i] == '.')
		{
			translatedx.push_back(1);
			translatedy.push_back(1);
		}
		
		if(str[i] == ',')
		{
			translatedx.push_back(2);
			translatedy.push_back(1);
		}
		
		if(str[i] == ':')
		{
			translatedx.push_back(3);
			translatedy.push_back(1);
		}
		
		if(str[i] == ';')
		{
			translatedx.push_back(4);
			translatedy.push_back(1);
		}
		
		if(str[i] == '(')
		{
			translatedx.push_back(5);
			translatedy.push_back(1);
		}
		
		if(str[i] == ')')
		{
			translatedx.push_back(6);
			translatedy.push_back(1);
		}
		
		if(str[i] == '{')
		{
			translatedx.push_back(7);
			translatedy.push_back(1);
		}
		
		if(str[i] == '}')
		{
			translatedx.push_back(8);
			translatedy.push_back(1);
		}
		
		if(str[i] == '@')
		{
			translatedx.push_back(9);
			translatedy.push_back(1);
		}
		
		if(str[i] == '[')
		{
			translatedx.push_back(10);
			translatedy.push_back(1);
		}
		
		if(str[i] == ']')
		{
			translatedx.push_back(11);
			translatedy.push_back(1);
		}
		
		if(str[i] == '+')
		{
			translatedx.push_back(12);
			translatedy.push_back(1);
		}
		
		if(str[i] == '-')
		{
			translatedx.push_back(13);
			translatedy.push_back(1);
		}
		
		if(str[i] == '/')
		{
			translatedx.push_back(14);
			translatedy.push_back(1);
		}
		
		if(str[i] == '*')
		{
			translatedx.push_back(15);
			translatedy.push_back(1);
		}
		
		if(str[i] == '\\')
		{
			translatedx.push_back(16);
			translatedy.push_back(1);
		}
		
		if(str[i] == '!')
		{
			translatedx.push_back(17);
			translatedy.push_back(1);
		}
		
		if(str[i] == '"')
		{
			translatedx.push_back(18);
			translatedy.push_back(1);
		}
		
/*		if(str[i] == '§')
		{
			translatedx.push_back(19);
			translatedy.push_back(1);
		}*/
		
		if(str[i] == '$')
		{
			translatedx.push_back(20);
			translatedy.push_back(1);
		}
		
		if(str[i] == '%')
		{
			translatedx.push_back(21);
			translatedy.push_back(1);
		}
		
		if(str[i] == '&')
		{
			translatedx.push_back(22);
			translatedy.push_back(1);
		}
		
		if(str[i] == '=')
		{
			translatedx.push_back(23);
			translatedy.push_back(1);
		}
		
		if(str[i] == '?')
		{
			translatedx.push_back(24);
			translatedy.push_back(1);
		}
		
/*		if(str[i] == 'ä')
		{
			translatedx.push_back(1);
			translatedy.push_back(2);
		}
		
		if(str[i] == 'Ä')
		{
			translatedx.push_back(2);
			translatedy.push_back(2);
		}
		
		if(str[i] == 'ö')
		{
			translatedx.push_back(3);
			translatedy.push_back(2);
		}
		
		if(str[i] == 'Ö')
		{
			translatedx.push_back(4);
			translatedy.push_back(2);
		}
		
		if(str[i] == 'ü')
		{
			translatedx.push_back(5);
			translatedy.push_back(2);
		}
		
		if(str[i] == 'Ü')
		{
			translatedx.push_back(6);
			translatedy.push_back(2);
		}
		
		if(str[i] == 'ß')
		{
			translatedx.push_back(7);
			translatedy.push_back(2);
		}*/
	}
}

sgPanel::sgPanel(sgPanel* p, sgPanel *n)
{
	fixorientation = false;
	rendertarget = NULL;
	prev = p;
	next = n;
	if(next)
	{
		next->prev = this;
	}
}

sgPanel::~sgPanel()
{
	for(int i = 0; i < elements.size(); i++)
	{
		delete elements[i];
	}
}

sgPanel *sgPanel::createPanel()
{
	next = new sgPanel(this, next);
	return next;
}

sgPanelElement *sgPanel::addImage(const char *imgtexfile, const sgVector2 &pos, const sgVector2 &size, const char *vs, const char *fs)
{
	sgImage *img = new sgImage;
	img->type = 0;
	
	if(vs == NULL || fs == NULL)
		img->mat = sgMaterial::getMaterial(imgtexfile, false, sgShader::BIS_TEXTURE);
	else
		img->mat = sgMaterial::getMaterial(vs, fs, imgtexfile, false);
	
	img->mat->lighting = false;
	img->mat->blending = true;

	img->pos = pos;
	img->size = size;
	img->ang = 0;
	
	elements.push_back(img);
	
	return (sgPanelElement*)img;
}

sgPanelElement *sgPanel::addImage(sgTexture *tex, const sgVector2 &pos, const sgVector2 &size, const char *vs, const char *fs)
{
	sgImage *img = new sgImage;
	img->type = 0;
	
	if(vs == NULL || fs == NULL)
		img->mat = sgMaterial::getMaterial(tex, sgShader::BIS_TEXTURE);
	else
		img->mat = sgMaterial::getMaterial(vs, fs, tex);
	
	img->mat->lighting = false;
	img->mat->blending = true;
	
	img->pos = pos;
	img->size = size;
	img->ang = 0;
	
	elements.push_back(img);
	
	return (sgPanelElement*)img;
}

sgPanelElement *sgPanel::addImage(sgMaterial *mat, const sgVector2 &pos, const sgVector2 &size)
{
	if(mat == NULL)
		return NULL;
	
	sgImage *img = new sgImage;
	img->type = 0;
	img->mat = mat;
	
	img->mat->lighting = false;
	img->mat->blending = true;
	
	img->pos = pos;
	img->size = size;
	img->ang = 0;
	
	elements.push_back(img);
	
	return (sgPanelElement*)img;
}

sgPanelElement *sgPanel::addText(const char *str, const sgVector2 &charsize, const sgVector2 &pos, const sgVector2 &size, const char *fonttexfile, const char *vs, const char *fs)
{
	sgText *txt = new sgText;
	txt->type = 1;
	
	if(vs == NULL || fs == NULL)
		txt->mat = sgMaterial::getMaterial(fonttexfile, false, sgShader::BIS_TEXTURE);
	else
		txt->mat = sgMaterial::getMaterial(vs, fs, fonttexfile, false);
	
	txt->mat->lighting = false;
	txt->mat->blending = true;
	
	txt->charsize = charsize;
	txt->pos = pos;
	txt->size = size;
	txt->str.str = std::string(str);
	txt->str.translate();
	
	elements.push_back(txt);
	
	return (sgPanelElement*)txt;
}

void sgPanel::destroy()
{
	if(prev)
		prev->next = next;
	else
		return;

	if(next)
		next->prev = prev;
	
	delete this;
}

void sgPanel::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
	else
		next = 0;
}