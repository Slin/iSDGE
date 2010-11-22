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
uniform mat4 matTex;

varying vec4 texcoord;
varying float fog;

void main()
{
	texcoord.xy = (matTex*vec4(vertTexcoord0, 1.0, 1.0)).xy;
	texcoord.zw = texcoord.xy*200.0;
	gl_Position = matProjViewModel*vec4(vertPos, 1.0);
	
	fog = min(gl_Position.w/800.0, 0.4);
}