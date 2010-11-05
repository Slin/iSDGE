//
//	Interface.cpp
//	Engine
//
//	Created by Nils Daumann on 13.08.10.
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

#include "Interface.h"
#include "sgTouches.h"
#include "sgRenderer.h"
#include "sgDebug.h"

void Interface::onInit(sgEntity *e)
{
	ent = e;
	
	//Create a static hello world text.
	ent->pan->addText("Hello World0", sgVector2(12, 18), sgVector2(0, 302), sgVector2(12, 18), "font1.png");
	
	//Create another text. The same as above, but its string will be updated.
	angdisp = (sgText*)ent->pan->addText("Angle: 00", sgVector2(12, 18), sgVector2(0, 200), sgVector2(12, 18), "font1.png");
	
	//Add an image element.
	logo = (sgImage*)ent->pan->addImage("logo_64.png", sgVector2(350, 200), sgVector2(64, 64));
	
	//Acess it as the last panel element and set its angle.
	((sgImage*)ent->pan->elements.back())->ang = 45;
}

void Interface::onDraw(float timestep)
{
	//If their is a touch
	if(sgTouches::touches.size() > 0)
	{
		//Place the image element at the touches position and rotate it.
		logo->pos = sgTouches::touches[0]->position;
		logo->ang += timestep*100;
		
		//Display the new angle on our second text element.
		char *angstr = new char[5];
		sprintf(angstr, "%.0f0", logo->ang);
		angdisp->str.str = std::string("Angle: ")+std::string(angstr);	//Assign the new string
		angdisp->str.translate();	//Make it displayable
		delete[] angstr;
	}
}
