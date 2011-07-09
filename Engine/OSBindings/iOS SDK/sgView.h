//
//	sgView.h
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

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "sgMain.h"
#import "sgViewController.h"


/**
 * View class. The main class, which handles everything.
 */
@interface sgView : UIView
{
	@private
		BOOL animating;
		NSInteger animationFrameInterval;
		id displayLink;
		EAGLContext *context;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

/**
 *	Get sgMain instance.
 *	Returns the pointer to the instance of sgMain.
 */
+ (sgMain*)main;

/**
 *	Get sgView instance.
 *	Returns the pointer to the instance of sgView.
 */
+ (sgView*)view;

/**
 *	Get sgViewController instance.
 *	Returns the pointer to the instance of sgViewController.
 */
+ (sgViewController*)viewcontroller;

/**
 *	Set sgViewController instance.
 *	Sets the pointer to the instance of sgViewController.
 */
+ (void)setViewcontroller:(sgViewController*)controller;

/**
 *	Start animation.
 *	Starts the rendering loop.
 */
- (void)startAnimation;

/**
 *	Stop animation.
 *	Stops the rendering loop.
 */
- (void)stopAnimation;

/**
 *	Draw view.
 *	Called automatically every frame and draws everything. Just don´t touch it.
 */
- (void)drawView:(id)sender;

/**
 * Resize from layer.
 * Resizes all render buffers to the new size of the layer.
 * @param layer pointer to the iPhones rendering layer
 */
- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer;

@end