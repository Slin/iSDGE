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

#if defined (ISDGE_CAMERA_SUPPORT)
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreImage/CoreImage.h>
#import <QuartzCore/QuartzCore.h>
#endif

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
	 [super loadView];
	 sgView *view = [[sgView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
	 self.view = view;
	 [view release];
	 
	 [sgView setViewcontroller:self];
 }


- (void)startCamera:(bool)front
{
#if defined (ISDGE_CAMERA_SUPPORT)
	if(sgCameraStream::currimage == 0)
	{
		sgCameraStream::currimage = sgTexture::getTexture(480, 360);
		sgCameraStream::currimage->lockPixels();
	}
	
	captureSession = [[AVCaptureSession alloc] init];
	captureSession.sessionPreset = AVCaptureSessionPresetMedium;
	
	AVCaptureDevice *camera = nil;
	
	NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	for(AVCaptureDevice *device in devices) 
	{
		if(front && [device position] == AVCaptureDevicePositionFront)
		{
			camera = device;
			break;
		}
		if([device position] == AVCaptureDevicePositionBack && !front) 
		{
			camera = device;
			break;
		}
	}
	
	if(camera == nil)
	{
		camera = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
	}
	
	NSError *error = nil;
	AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:camera error:&error];
	[captureSession addInput:input];
	
	AVCaptureVideoDataOutput *output = [[[AVCaptureVideoDataOutput alloc] init] autorelease];
	[captureSession addOutput:output];
	output.videoSettings = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey];
//	output.minFrameDuration = CMTimeMake(1, 15);
	
//	dispatch_queue_t queue = dispatch_queue_create("com.slindev.cameraqueue", NULL);
	[output setSampleBufferDelegate:self queue:dispatch_get_current_queue()];
//	dispatch_release(queue);
	
	[captureSession startRunning];
#endif
}


- (void)startAccelerometer
{
	//Configure and start accelerometer
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0/30.0f)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
}


- (void)startCompass
{
#if defined (ISDGE_COMPASS_SUPPORT)
	locationManager = [[CLLocationManager alloc] init];
	locationManager.delegate = self;
	if([locationManager locationServicesEnabled] &&
	   [locationManager headingAvailable])
	{
//		[locationManager startUpdatingLocation];
		[locationManager startUpdatingHeading];
	}
#endif
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
	[super viewDidLoad];
}
 

 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


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
				sgTouches::addTouch(sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::addTouch(sgVector2(pos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::addTouch(sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::addTouch(sgVector2(pos.y, pos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::addTouch(sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(pos.x, pos.y));
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
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::updateTouch(i, sgVector2(pos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y), sgVector2(ppos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x), sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-ppos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::updateTouch(i, sgVector2(pos.y, pos.x), sgVector2(ppos.y, ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
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
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::updateTouch(i, sgVector2(pos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y), sgVector2(ppos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x), sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-ppos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::updateTouch(i, sgVector2(pos.y, pos.x), sgVector2(ppos.y, ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
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
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 1:
				sgTouches::updateTouch(i, sgVector2(pos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y), sgVector2(ppos.x, sgRenderer::backingHeight/sgRenderer::scaleFactor-ppos.y), sgVector2(pos.x, pos.y));
				break;
				
			case 2:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-pos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x), sgVector2(sgRenderer::backingHeight/sgRenderer::scaleFactor-ppos.y, sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			case 3:
				sgTouches::updateTouch(i, sgVector2(pos.y, pos.x), sgVector2(ppos.y, ppos.x), sgVector2(pos.x, pos.y));
				break;
				
			default:
				sgTouches::updateTouch(i, sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-pos.x, pos.y), sgVector2(sgRenderer::backingWidth/sgRenderer::scaleFactor-ppos.x, ppos.y), sgVector2(pos.x, pos.y));
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
	sgAccelerometer::currsmoothaccabs = temp*0.1f+sgAccelerometer::currsmoothaccabs*0.9f;
	
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
	
	sgAccelerometer::currsmoothacc = sgAccelerometer::curracc*0.1f+sgAccelerometer::currsmoothacc*0.9f;
}

#if defined (ISDGE_COMPASS_SUPPORT)
- (void)locationManager:(CLLocationManager*)manager didUpdateHeading:(CLHeading*)newHeading
{
	// If the accuracy is valid, process the event.
	if (newHeading.headingAccuracy > 0)
	{
		sgAccelerometer::currheading = newHeading.magneticHeading;
	}
}
#endif

#if defined (ISDGE_CAMERA_SUPPORT)
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
	CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
	
	sgCameraStream::devicedata = pixelBuffer;
	
	CVPixelBufferLockBaseAddress(pixelBuffer, 0);
	
	int bufferHeight = CVPixelBufferGetHeight(pixelBuffer);
	int bufferWidth = CVPixelBufferGetWidth(pixelBuffer);
	
	unsigned char *rowBase = (unsigned char *)CVPixelBufferGetBaseAddress(pixelBuffer);
	
	sgCameraStream::currimage->setPixels(rowBase, 0, bufferWidth*bufferHeight*4);
	
//	dispatch_async(dispatch_get_main_queue(), ^{
//		[(sgView*)self.view setContext];
		sgCameraStream::currimage->updatePixels(true);
//	});
	
	CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
}
#endif

- (void)dealloc
{
    [super dealloc];
}

@end
