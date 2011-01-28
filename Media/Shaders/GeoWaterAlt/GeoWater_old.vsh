//
//	Shader.vsh
//	iSDGE
//
//	Created by Nils Daumann on 16.04.10.
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

attribute vec3 vertPos;
attribute vec2 vertTexcoord0;

uniform mat4 matProjViewModel;
uniform mat4 matModel;
uniform vec3 vPosition;
uniform mat4 matTex;

varying vec2 texcoord;
varying vec3 projpos;
varying vec3 normal_;
varying float light;

float getHeight(vec2 pos, float time)
{
	return sin(pos.x+time)*sin(pos.y*0.3+1.2*time)*cos(pos.y*0.1+0.5*time)*0.25;
}

vec3 getNormal(vec3 pos1, vec3 pos2, vec3 pos3)
{
	vec3 dir1 = pos2-pos1;
	vec3 dir2 = pos3-pos1;
	return normalize(cross(dir1, dir2));
}

void main()
{
	vec2 vertpos = (matModel*vec4(vertPos, 1.0)).xz;
	float distfac = min(distance(vertpos, vPosition.xz)*0.016, 1.0);
	distfac *= distfac*distfac;//*distfac*distfac;
	distfac = 1.0-distfac;
	float height = getHeight(vertpos, matTex[3].x)*distfac;
	float height2 = getHeight(vertpos+vec2(1.0, 0.0), matTex[3].x)*distfac;
	float height3 = getHeight(vertpos+vec2(0.0, 1.0), matTex[3].x)*distfac;
	
	gl_Position = matProjViewModel*vec4(vertPos.x, vertPos.y+height, vertPos.z, 1.0);
	projpos = gl_Position.xyw*vec3(-0.5, 0.5, 1.0);
	
	texcoord.xy = matTex[3].xy*0.15+vertpos*0.3;
	
	vec3 norm = getNormal(vec3(0.0, height, 0.0), vec3(1.0, height2, 0.0), vec3(0.0, height3, 1.0));
	light = 0.8+max(dot(norm, normalize(vec3(1.0, -1.0, 1.0))), 0.0)*0.25;
	normal_ = norm*0.3-0.75;
}
