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
	precision mediump float;
#else
	#define lowp
	#define mediump
	#define highp
#endif

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;

varying vec2 texcoord;
varying vec3 projpos;
varying vec3 worldpos;
varying vec3 viewpos;

void main()
{
	float camdist = distance(viewpos, worldpos);
	vec4 dis = texture2D(mTexture0, texcoord.xy)-0.5;
	vec2 reflcoords = projpos.xy/projpos.z*0.5+0.5;
#ifdef GL_ES
    reflcoords.x = 1.0-reflcoords.x;
#else
	reflcoords.y = 1.0-reflcoords.y;
#endif
	vec4 color = texture2D(mTexture1, reflcoords+dis.rg);
	color.rgb = mix(color.rgb, vec3(0.7333, 0.7843, 0.8549), min(camdist, 1.0))*0.5;
	color.a = 0.5;
	gl_FragColor = color;
}
