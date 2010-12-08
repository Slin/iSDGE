//
//	sgCamera.cpp
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

#include <cstring>
#include "sgCamera.h"
#include "sgTexture.h"

sgCamera::sgCamera(sgCamera *p, sgCamera *n)
{
	rendertarget = NULL;
	
	size = sgVector2(100, 100);
	aspect = 1;
	arc = 70;
	clipnear = 0.01;
	clipfar = 500.0;
	updateProj();
	
	tag = 0;
	lodshift = 0.0;
	
	prev = p;
	next = n;
}

sgCamera *sgCamera::createCamera()
{
	next = new sgCamera(this, next);
	return next;
}

sgVector3 sgCamera::camToView(sgVector3 dir)
{
	return matproj.transform(dir);
}

sgVector3 sgCamera::camToWorld(sgVector3 dir)
{
	sgVector3 temp = camToView(dir);
	temp = matview.transform(dir);
	return temp;
}

void sgCamera::updateProj()
{
	matproj.makeProjectionPersp(arc, aspect, clipnear, clipfar);
}

void sgCamera::updateView()
{
	matview = rotation.getMatrix();
	matview.transpose();
	matview.translate(position*(-1));
}

void sgCamera::destroy()
{
	if(prev)
		prev->next = next;
	if(next)
		next->prev = prev;
	
	delete this;
}

void sgCamera::destroyAll()
{
	destroy();
	
	if(next)
		next->destroyAll();
	if(prev)
		prev->destroyAll();
}
