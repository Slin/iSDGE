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

precision highp float;

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;

varying vec2 texcoord;
varying vec3 projpos;
varying float light;
varying float camdist;

void main()
{
	vec2 dis = texture2D(mTexture0, texcoord.xy).rg*1.5-0.75;
	vec2 reflcoords = projpos.xy/projpos.z+0.5;
	vec4 color = texture2D(mTexture1, reflcoords+dis.rg)*light*vec4(0.5, 0.5, 0.55, 1.0);
    gl_FragColor = mix(color, vec4(0.7333, 0.7843, 0.8549, 1.0), min(max(camdist*0.004, 0.0), 1.0));
}
