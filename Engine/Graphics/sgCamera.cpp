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
#include "sgDebug.h"

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
	sgMatrix4x4 mat;
	mat.makeTranslate(position*(-1));
	mat.rotate(rotation);
	
	sgVector3 temp = camToView(dir);
	temp = mat*temp;
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
	
//	matinvview = 
}

void sgCamera::updateFrustum()
{
	plleft.setPlane(position, camToWorld(sgVector3(-1.0, 1.0, 0.0)), camToWorld(sgVector3(-1.0, -1.0, 0.0)));
	plright.setPlane(position, camToWorld(sgVector3(1.0, 1.0, 0.0)), camToWorld(sgVector3(1.0, -1.0, 0.0)));
	pltop.setPlane(position, camToWorld(sgVector3(-1.0, 1.0, 0.0)), camToWorld(sgVector3(1.0, 1.0, 0.0)));
	pldown.setPlane(position, camToWorld(sgVector3(-1.0, -1.0, 0.0)), camToWorld(sgVector3(1.0, -1.0, 0.0)));
	
/*	sgVector3 dir = camToWorld(sgVector3(0.0, 0.0, 0.0));
	sgLog("X: %f, Y: %f, Z: %f", dir.x, dir.y, dir.z);*/
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
