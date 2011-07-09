#ifndef __KINECTWINDESKTOPINPUT_H__
#define __KINECTWINDESKTOPINPUT_H__
#include "KinectWinLibCmn.h"
#include "KinectWinGesture.h"

void SendMouseInputs( float x, float y );
void SendAlt( bool bUp );
void SendEsc( bool bUp );
void SendTab( bool bUp );
void SendKeyboardInputs( eHandGestureType eHandGestureType );

#endif // __KINECTWINDESKTOPINPUT_H__