//
//	sgViewController.mm
//	iSDGE
//
//	Created by Nils Daumann on 11.02.11.
//	Copyright (c) 2011 Nils Daumann

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

#import "sgViewController.h"
#import "sgView.h"


@implementation sgViewController

/*
 // The designated initializer. Override to perform setup that is required before the view is loaded.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
 if (self) {
 // Custom initialization
 }
 return self;
 }
 */


 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView
 {
	 sgView *view = [[sgView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
	 self.view = view;
	 [view release];
 }


- (void)startAccelerometer
{
	//Configure and start accelerometer
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0/30.0f)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
}


 // Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 - (void)viewDidLoad
 {
	 [self startAccelerometer];
	 [super viewDidLoad];
 }
 


/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


//Handle player input
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(int i = 0; i < [touches count]; i++)
	{
		UITouch *curr = [touches anyObject];
		CGPoint pos = [curr locationInView:self.view];
		
		switch([sgView main]->orientation)
		{
			case 0:
				sgTouches::addTouch(sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::addTouch(sgVector2(pos.x, sgRenderer::backingHeight-pos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::addTouch(sgVector2(sgRenderer::backingHeight-pos.y, sgRenderer::backingWidth-pos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::addTouch(sgVector2(pos.y, pos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::addTouch(sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(pos.x, pos.y));
				break;
		}
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *curr;
	for(int i = 0; i < sgTouches::touches.size(); i++)
	{
		curr = [[[event allTouches] allObjects] objectAtIndex:i];
		CGPoint pos = [curr locationInView:self.view];
		CGPoint ppos = [curr previousLocationInView:self.view];
		
		switch([sgView main]->orientation)
		{
			case 0:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(sgRenderer::backingWidth-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::updateTouch(i, sgVector2(pos.x, sgRenderer::backingHeight-pos.y), sgVector2(ppos.x, sgRenderer::backingHeight-ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight-pos.y, sgRenderer::backingWidth-pos.x), sgVector2(sgRenderer::backingHeight-ppos.y, sgRenderer::backingWidth-ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::updateTouch(i, sgVector2(pos.y, pos.x), sgVector2(ppos.y, ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(sgRenderer::backingWidth-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
		}
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(int i = 0; i < [touches count]; i++)
		sgTouches::removeTouch(sgTouches::touches.size()-1);
	
	UITouch *curr;
	for(int i = 0; i < sgTouches::touches.size(); i++)
	{
		curr = [[[event allTouches] allObjects] objectAtIndex:i];
		CGPoint pos = [curr locationInView:self.view];
		CGPoint ppos = [curr previousLocationInView:self.view];
		
		switch([sgView main]->orientation)
		{
			case 0:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(sgRenderer::backingWidth-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::updateTouch(i, sgVector2(pos.x, sgRenderer::backingHeight-pos.y), sgVector2(ppos.x, sgRenderer::backingHeight-ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight-pos.y, sgRenderer::backingWidth-pos.x), sgVector2(sgRenderer::backingHeight-ppos.y, sgRenderer::backingWidth-ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::updateTouch(i, sgVector2(pos.y, pos.x), sgVector2(ppos.y, ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(sgRenderer::backingWidth-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
		}
		
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(int i = 0; i < [touches count]; i++)
		sgTouches::removeTouch(sgTouches::touches.size()-1);
	
	UITouch *curr;
	for(int i = 0; i < sgTouches::touches.size(); i++)
	{
		curr = [[[event allTouches] allObjects] objectAtIndex:i];
		CGPoint pos = [curr locationInView:self.view];
		CGPoint ppos = [curr previousLocationInView:self.view];
		
		switch([sgView main]->orientation)
		{
			case 0:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(sgRenderer::backingWidth-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::updateTouch(i, sgVector2(pos.x, sgRenderer::backingHeight-pos.y), sgVector2(ppos.x, sgRenderer::backingHeight-ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight-pos.y, sgRenderer::backingWidth-pos.x), sgVector2(sgRenderer::backingHeight-ppos.y, sgRenderer::backingWidth-ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::updateTouch(i, sgVector2(pos.y, pos.x), sgVector2(ppos.y, ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth-pos.x, pos.y), sgVector2(sgRenderer::backingWidth-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
		}
		
	}
}

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{	
	sgVector3 temp;
	temp.x = acceleration.x;
	temp.y = acceleration.y;
	temp.z = acceleration.z;
	
	sgAccelerometer::curraccabs = temp;
	
	switch([sgView main]->orientation)
	{
		case 0:
			sgAccelerometer::curracc.x = temp.x;
			sgAccelerometer::curracc.y = temp.y;
			sgAccelerometer::curracc.z = temp.z;
			break;
			
		case 1:
			sgAccelerometer::curracc.x = -temp.x;
			sgAccelerometer::curracc.y = -temp.y;
			sgAccelerometer::curracc.z = -temp.z;
			break;
			
		case 2:
			sgAccelerometer::curracc.x = temp.y;
			sgAccelerometer::curracc.y = -temp.x;
			sgAccelerometer::curracc.z = temp.z;
			break;
			
		case 3:
			sgAccelerometer::curracc.x = -temp.y;
			sgAccelerometer::curracc.y = temp.x;
			sgAccelerometer::curracc.z = temp.z;
			break;
			
		default:
			sgAccelerometer::curracc.x = temp.x;
			sgAccelerometer::curracc.y = temp.y;
			sgAccelerometer::curracc.z = temp.z;
			break;
	}
}

- (void)dealloc {
    [super dealloc];
}

@end
