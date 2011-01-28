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
varying vec4 vertpos;

float getHeight(vec2 pos, float time)
{
	return (1.0-sin(abs(mod(pos.x*0.2+time, 3.14159))))*(1.0+0.5*sin(pos.x*0.0833+time))*0.65;
}

vec3 getNormal(vec3 pos1, vec3 pos2, vec3 pos3)
{
	vec3 dir1 = pos2-pos1;
	vec3 dir2 = pos3-pos1;
	return normalize(cross(dir1, dir2));
}

void main()
{
	vertpos.xy = (matModel*vec4(vertPos, 1.0)).xz;
	vertpos.zw = vPosition.xz;
	float distfac = min(abs(vertpos.x-vPosition.x)*0.016, 1.0);
	distfac *= distfac*distfac;
	distfac = 1.0-distfac;
	float height = getHeight(vertpos.xy, matTex[3].x)*distfac;
	float height2 = getHeight(vertpos.xy+vec2(1.0, 0.0), matTex[3].x)*distfac;
	
	gl_Position = matProjViewModel*vec4(vertPos.x, vertPos.y+height, vertPos.z, 1.0);
	projpos = gl_Position.xyw*vec3(-0.5, 0.5, 1.0);
	
	texcoord.xy = matTex[3].xy*0.15+vertpos.xy*0.03;
	
	vec3 norm = normalize(vec3(-0.25*(height2-height), 0.0625, 0.0))*0.2;
	normal_ = vec3(norm.xz-0.25, height);
	vertpos *= 0.004;
}
