//
//	Shader.fsh
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

#ifdef GL_ES
	precision highp float;
#else
	#define lowp
	#define mediump
	#define highp
#endif

uniform lowp sampler2D mTexture0;
uniform lowp sampler2D mTexture1;

varying vec2 texcoord;
varying vec3 lightdir;
varying vec3 viewdir;

void main()
{
	vec4 Color = texture2D(mTexture0, texcoord.xy);
	vec4 Normal = texture2D(mTexture1, texcoord.xy)*2.0-1.0;
	Normal.a = 1.0;
	Color.rgb *= vec3(max(dot(lightdir, Normal.xyz), 0.0)+0.3);
	vec3 halfvec = normalize(viewdir+lightdir);
	Color.rgb += pow(max(dot(halfvec, Normal.xyz), 0.0), 50.0)*0.8;
    gl_FragColor = Color;
}
