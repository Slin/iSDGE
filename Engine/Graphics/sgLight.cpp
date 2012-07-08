//
//	sgLight.cpp
//	iSDGE
//
//	Created by Nils Daumann on 19.04.10.
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

#include "sgLight.h"

sgLight::sgLight(sgLight *p, sgLight *n)
{
	prev = p;
	next = n;
	if(next)
	{
		next->prev = this;
	}
	setDefault();
}

sgLight *sgLight::createLight()
{
	next = new sgLight(this, next);
	return next;
}

void sgLight::setDefault()
{
	ambient.r = 1.0;
	ambient.g = 1.0;
	ambient.b = 1.0;
	ambient.a = 1.0;
	diffuse.r = 1.0;
	diffuse.g = 1.0;
	diffuse.b = 1.0;
	diffuse.a = 1.0;
	specular.r = 1.0;
	specular.g = 1.0;
	specular.b = 1.0;
	specular.a = 1.0;
	position.x = 1.0;
	position.y = -1.0;
	position.z = 1.0;
	position.w = 0.0;
	attenuation = 0.004;
	shadow = true;
}

void sgLight::destroy()
{
	if(prev)
		prev->next = next;
	else
		return;

	if(next)
		next->prev = prev;

	delete this;
}

void sgLight::destroyAll()
{
	sgLight *ntemp = next;
	sgLight *ptemp = prev;
	destroy();

	if(ntemp)
		ntemp->destroyAll();
	if(ptemp)
		ptemp->destroyAll();
}
