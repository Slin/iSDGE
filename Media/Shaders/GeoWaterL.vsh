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
varying float light;
varying float camdist;

void main()
{
	gl_Position = matProjViewModel*vec4(vertPos, 1.0);
	projpos = gl_Position.xyw*vec3(-0.5, 0.5, 1.0);
	
	vec2 vertpos = (matModel*vec4(vertPos, 1.0)).xz;
	texcoord.xy = matTex[3].xy*0.3+vertpos;
	light = 0.95;
	
	camdist = distance(vertpos, vPosition.xz)-150.0;
}
