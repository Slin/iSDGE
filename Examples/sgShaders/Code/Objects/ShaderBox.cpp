//
//	ShaderBox.cpp
//	Engine
//
//	Created by Nils Daumann on 02.06.10.
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

#include "ShaderBox.h"
#include <cmath>

void ShaderBox::onInit(sgEntity *e)
{
	ent = e;
	counter = 0;
	
	//Set the shader of the first material (which is the only one in this case)
	ent->obj->materials[0]->setShader("sgsDefaultlight", "ShaderCol");
	
	//Add a new parameter of thw type float4 to the material
	param = ent->obj->materials[0]->addParameter("color", (void*)(new float[4]));
}

void ShaderBox::onDraw(float timestep)
{
	counter += timestep*5.0f;
	
	//Update the material parameters values
	((float*)param->parameter)[0] = sin(counter);
	((float*)param->parameter)[1] = cos(counter);
	((float*)param->parameter)[2] = 1.0f;
	((float*)param->parameter)[3] = 1.0f;
}
