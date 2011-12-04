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
#include <cmath>
#include "sgCamera.h"
#include "sgTexture.h"
#include "sgDebug.h"

sgCamera::sgCamera(sgCamera *p, sgCamera *n)
{
	rendertarget = NULL;
	
	size = sgVector2(100, 100);
	aspect = 1;
	arc = 70;
	clipnear = 0.1;
	clipfar = 500.0;
	updateProj();
	
	tag = 0;
	lodshift = 0.0;
	
	fogcolor = 1.0;
	fogstartend = 0.75; //deactivate fog
	
	prev = p;
	next = n;
	if(next)
	{
		next->prev = this;
	}
	
	updateCamera();
}

sgCamera *sgCamera::createCamera()
{
	next = new sgCamera(this, next);
	return next;
}

sgVector3 sgCamera::camToWorld(sgVector3 dir)
{
	sgVector4 vec(dir.x, dir.y, dir.z, 1.0);
	vec = matinvproj*vec;
	vec /= vec.w;
	
	sgVector3 temp = vec;
	temp = matinvview*temp;
	return temp;
}

sgVector3 sgCamera::worldToCam(sgVector3 dir)
{
	sgMatrix4x4 tempmat = matproj*matview;
	sgVector4 vec(dir.x, dir.y, dir.z, 1.0);
	vec = tempmat*vec;
	
	if(vec.w != 0)
		vec /= vec.w;
	
	sgVector3 temp = vec;
	return temp;
}

void sgCamera::updateProj()
{
	matproj.makeProjectionPersp(arc, aspect, clipnear, clipfar);
	matinvproj.makeInvProjectionPersp(arc, aspect, clipnear, clipfar);
	
//	sgMatrix4x4 test = matproj*matinvview;
//	sgLog("%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f", test.mat[0], test.mat[1], test.mat[2], test.mat[3], test.mat[4], test.mat[5], test.mat[6], test.mat[7], test.mat[8], test.mat[9], test.mat[10], test.mat[11], test.mat[12], test.mat[13], test.mat[14], test.mat[15]);
}

void sgCamera::updateCamera()
{
	matview = rotation.getMatrix();
	matview.transpose();
	matview.translate(position*(-1));
	
	matinvview.makeTranslate(position);
	matinvview.rotate(rotation);
}

void sgCamera::updateFrustum()
{
	sgVector3 pos2 = camToWorld(sgVector3(-1.0, 1.0, 1.0));
	sgVector3 pos3 = camToWorld(sgVector3(-1.0, -1.0, 1.0));
	sgVector3 pos5 = camToWorld(sgVector3(1.0, 1.0, 1.0));
	sgVector3 pos6 = camToWorld(sgVector3(1.0, -1.0, 1.0));
	
	sgVector3 vmax;
	sgVector3 vmin;
	vmax.x = fmax(position.x, fmax(pos2.x, fmax(pos3.x, fmax(pos5.x, pos6.x))));
	vmax.y = fmax(position.y, fmax(pos2.y, fmax(pos3.y, fmax(pos5.y, pos6.y))));
	vmax.z = fmax(position.z, fmax(pos2.z, fmax(pos3.z, fmax(pos5.z, pos6.z))));
	vmin.x = fmin(position.x, fmin(pos2.x, fmin(pos3.x, fmin(pos5.x, pos6.x))));
	vmin.y = fmin(position.y, fmin(pos2.y, fmin(pos3.y, fmin(pos5.y, pos6.y))));
	vmin.z = fmin(position.z, fmin(pos2.z, fmin(pos3.z, fmin(pos5.z, pos6.z))));
	
	frustumcenter = vmax+vmin;
	frustumcenter *= 0.5;
	frustumradius = frustumcenter.dist(vmax);
	
	plleft.setPlane(position, pos2, pos3);
	plright.setPlane(position, pos5, pos6);
	pltop.setPlane(position, camToWorld(sgVector3(-1.0, 1.0, 1.0)), camToWorld(sgVector3(1.0, 1.0, 1.0)));
	pldown.setPlane(position, camToWorld(sgVector3(-1.0, -1.0, 1.0)), camToWorld(sgVector3(1.0, -1.0, 1.0)));
	
//		sgVector3 dir = camToWorld(sgVector3(-1.0, 1.0, 1.0));
//		sgLog("X: %f, Y: %f, Z: %f", dir.x, dir.y, dir.z);
}

bool sgCamera::inFrustum(sgVector4 &sphere)
{
	if(frustumcenter.dist(sphere) > frustumradius+sphere.w)
		return false;
	
	if(plleft.dist(sphere) > sphere.w)
		return false;
	
	if(plright.dist(sphere) < -sphere.w)
		return false;
	
	if(pltop.dist(sphere) < -sphere.w)
		return false;
	
	if(pldown.dist(sphere) > sphere.w)
		return false;
	
	return true;
}

void sgCamera::destroy()
{
	if(prev)
		prev->next = next;
	else
		return;
	
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
	else
		next = 0;
}
