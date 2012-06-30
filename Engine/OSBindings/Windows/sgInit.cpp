//
//  sgInit.mm
//  iSDGE
//
//	Created by Nils Daumann on 17.06.12.
//	Copyright (c) 2012 Nils Daumann

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

#include "sgInit.h"
#include "sgMain.h"
#include "sgEvents.h"
#include <cstdio>


int sgInit(int argc, char *argv[], sgEvents *event, int res_x, int res_y, bool fullscreen)
{
	sgMain::eventhandler = event;
    bool running = true;

    glfwInit();

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
    if(!glfwOpenWindow( res_x, res_y, 0, 0, 0, 0, 24, 8, fullscreen?GLFW_FULLSCREEN:GLFW_WINDOW))
    {
        glfwTerminate();
        return 0;
    }

	printf("Multisampling samples: %i\n", glfwGetWindowParam(GLFW_FSAA_SAMPLES));
    glEnable(GL_MULTISAMPLE_ARB);

    sgRenderer::backingWidth = res_x;
    sgRenderer::backingHeight = res_y;

    glfwSetWindowTitle("iSDGE");

    glfwSetMousePos(res_x*0.5, res_y*0.5);

    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  printf("Error: %s\n", glewGetErrorString(err));
	  return -1;
	}
	printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//Initialize the renderer
	sgMain *sgmain = new sgMain(2);

	int mouse_x, mouse_y;
    while(running)
    {
    	glfwGetMousePos(&mouse_x, &mouse_y);
		glfwSetMousePos(res_x*0.5, res_y*0.5);
		sgMouse::currdir = sgVector2(mouse_x-res_x*0.5, mouse_y-res_y*0.5);
		sgMouse::currpos = sgVector2(mouse_x, mouse_y);

        sgmain->drawView();
        glfwSwapBuffers();

        // exit if ESC was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }

    glfwTerminate();
}
