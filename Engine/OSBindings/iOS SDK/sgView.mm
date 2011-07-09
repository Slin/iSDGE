//
//	sgView.mm
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

#import "sgView.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

//#define DISABLE_OPENGL_ES_2_0

@implementation sgView

@synthesize animating;
@dynamic animationFrameInterval;

sgMain *sgmain;
sgView *sgview;
sgViewController *sgviewcontroller;

+ (sgMain*)main
{
	return sgmain;
}

+ (sgView*)view
{
	return sgview;
}

+ (sgViewController*)viewcontroller
{
	return sgviewcontroller;
}

+ (void)setViewcontroller:(sgViewController*)controller
{
	sgviewcontroller = controller;
}

// You must implement this method
+ (Class)layerClass
{
	return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)aRect
{
	[super initWithFrame:aRect];

	//Initialise EAGL.
	CAEAGLLayer* eaglLayer = (CAEAGLLayer*)[self layer];	
	[eaglLayer setDrawableProperties: [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil]];
	eaglLayer.opaque = TRUE;
	
	//Find supported OpenGL ES version
	unsigned int oglvers;
	context = nil;
	
#ifndef DISABLE_OPENGL_ES_2_0
	if(context == nil)
	{
		oglvers = 2;
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		if(!context || ![EAGLContext setCurrentContext:context])
		{
			context = nil;
		}
	}
#endif
	
	if(context == nil)
	{
		oglvers = 1;
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		if(!context || ![EAGLContext setCurrentContext:context])
		{
			oglvers = 0;
			context = nil;
		}
	}
	
	//Initialize the renderer
	sgmain = new sgMain(oglvers);
	[self resizeFromLayer:(CAEAGLLayer*)self.layer];

	//Initialize drawing
	animating = FALSE;
	animationFrameInterval = 1;
	displayLink = nil;
	
	sgview = self;
	
	[self startAnimation];
	
	return self;
}

- (void)drawView:(id)sender
{
	[EAGLContext setCurrentContext:context];
	sgmain->drawView();
//	[EAGLContext setCurrentContext:context];
//	glBindRenderbuffer(GL_RENDERBUFFER, sgmain->renderer->colorRenderbuffer);
	if(sgmain->renderer->oglversion >= 2)
	{
		[context presentRenderbuffer:GL_RENDERBUFFER];
	}else
	{
		[context presentRenderbuffer:GL_RENDERBUFFER_OES];
	}
}

- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer
{
	if(sgmain->renderer->oglversion == 1)
	{
		// Allocate color buffer backing based on the current layer size
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, sgmain->renderer->colorRenderbuffer);
		[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &sgRenderer::backingWidth);
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &sgRenderer::backingHeight);
	}
	
	if(sgmain->renderer->oglversion >= 2)
	{
		// Allocate color buffer backing based on the current layer size
		glBindRenderbuffer(GL_RENDERBUFFER, sgmain->renderer->colorRenderbuffer);
		[context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &sgRenderer::backingWidth);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &sgRenderer::backingHeight);
	}
	
	return sgmain->renderer->resizeBuffers();
}

- (void)layoutSubviews
{
    [self resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
	if(frameInterval >= 1)
	{
		animationFrameInterval = frameInterval;
	
		if(animating)
		{
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

- (void)startAnimation
{
    if(!animating)
    {
		displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
		[displayLink setFrameInterval:animationFrameInterval];
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if(animating)
    {
        [displayLink invalidate];
        displayLink = nil;
		
        animating = FALSE;
    }
}

- (void)dealloc
{
	delete sgmain;
	
	// Tear down context
	if([EAGLContext currentContext] == context)
		[EAGLContext setCurrentContext:nil];
	
	if(context != nil)
	{
		[context release];
		context = nil;
	}
	
	[super dealloc];
}


//App delegate implementation
- (void)applicationWillResignActive:(UIApplication *)application
{
		[self stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
		[self startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
		[self stopAnimation];
}

@end
