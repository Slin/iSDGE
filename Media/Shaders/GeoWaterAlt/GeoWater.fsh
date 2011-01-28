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

precision mediump float;

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;

varying vec2 texcoord;
varying vec3 projpos;
varying vec3 normal_;
varying vec4 vertpos;

void main()
{
	float camfac = min(max(distance(vertpos.xy, vertpos.zw)-0.6, 0.0), 1.0);
	vec3 dis = texture2D(mTexture0, texcoord.xy).rgb;
	dis.rg = dis.rg*0.5+normal_.xy;
	dis.b *= normal_.z*(1.0-min(abs((vertpos.y-vertpos.w)*3.0), 1.0));
	vec2 reflcoords = projpos.xy/projpos.z+0.5;
	vec4 color = texture2D(mTexture1, reflcoords+dis.rg)*0.5;
	color = color*(1.0-dis.b)+vec4(dis.b);
    gl_FragColor = mix(color, vec4(0.7333, 0.7843, 0.8549, 1.0), camfac);
}
