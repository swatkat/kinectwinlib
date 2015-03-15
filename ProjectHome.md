## Introduction: ##
**KinectWinLib** is a C++ Win32 library based on Kinect Windows SDK that aims to implement gesture controls for Windows 7. This is still a work-in-progress.

## Source: ##
  * Check-out code using any SVN client from `http://kinectwinlib.googlecode.com/svn/trunk/`. It contains source code of library **KinectWinLib** and a sample application **KinectWinLibTester**.

## Currently implemented: ##
  * Control mouse cursor through left-hand gesture.
  * Recognize left-hand gesture:
    * Horizontal wave, left to right
    * Horizontal wave, right to left
    * Vertical wave, top to bottom
    * Vertical wave, bottom to top
  * ALT-ESC (similar to ALT-TAB) when left-hand horizontal, left to right gesture is detected. ALT-ESC cycles windows through Z-order.

## To do: ##
  * Faster response. Need to tinker with skeletal frame transitions and smoothing.
  * Capture clicks. Easy way to indicate left, right and double clicks.
  * Interact with applications. ALT-TAB, minimize, maximize?

## Prerequisites for development: ##
  * Kinect sensor
  * Windows 7
  * Windows SDK v7.1
  * Kinect SDK and Runtime
  * Visual C++ 2010 Express
  * DirectX SDK (for video streaming)
  * DebugView to see debug prints

## Prerequisites for running application: ##
  * Kinect sensor
  * Windows 7
  * Kinect SDK and Runtime
  * Check `Downloads` section for a sample application

## Links: ##
  * Kinect sensor: http://www.xbox.com/en-IN/Xbox360/Accessories/Kinect/Home
  * Windows SDK: http://www.microsoft.com/download/en/details.aspx?id=8279
  * Kinect SDK and Runtime: http://research.microsoft.com/en-us/um/redmond/projects/kinectsdk/
  * Visual C++ 2010 Express: http://www.microsoft.com/visualstudio/en-us/products/2010-editions/visual-cpp-express
  * DirectX SDK: http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=6812
  * DebugView: http://technet.microsoft.com/en-us/sysinternals/bb896647