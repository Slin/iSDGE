//
//	sgViewController.h
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

//#define ISDGE_CAMERA_SUPPORT
//#define ISDGE_COMPASS_SUPPORT

#import <UIKit/UIKit.h>

#if defined (ISDGE_COMPASS_SUPPORT)
#import <CoreLocation/CoreLocation.h>
#endif

#if defined (ISDGE_CAMERA_SUPPORT)
#import <AVFoundation/AVFoundation.h>
#endif

@interface sgViewController : UIViewController
<
	UIAccelerometerDelegate
#if defined (ISDGE_COMPASS_SUPPORT)
	,CLLocationManagerDelegate
#endif
#if defined (ISDGE_CAMERA_SUPPORT)
	,AVCaptureVideoDataOutputSampleBufferDelegate
#endif
>
{
#if defined (ISDGE_COMPASS_SUPPORT)
	CLLocationManager *locationManager;
#endif
#if defined (ISDGE_CAMERA_SUPPORT)
	AVCaptureSession *captureSession;
#endif
}

- (void)startCamera:(bool)front;
- (void)startAccelerometer;
- (void)startCompass;

@end
