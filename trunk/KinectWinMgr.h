#ifndef __KINECTWINMGR_H__
#define __KINECTWINMGR_H__
#include "KinectWinLibCmn.h"
#include "KinectWinGesture.h"

// Kinect manager class that initializes NUI and listens for events from NUI runtime
class KinectWinMgr
{
public:
    KinectWinMgr();
    virtual ~KinectWinMgr();
    bool NuiInit();
    void NuiUnInit();

private:
    static DWORD WINAPI NuiGuessGestureThread( LPVOID pParam );
    static DWORD WINAPI NuiProcessThread( LPVOID pParam );
    void NuiGotSkeletonAlert();

    HANDLE m_hNuiProcessThread;
    HANDLE m_hNuiGuessGestureThread;
    HANDLE m_hEvNuiProcessThreadStopEvent;
    HANDLE m_hNuiSkeletonEvent;
    KinectWinGesture m_kinectWinGesture;
};

#endif // __KINECTWINMGR_H__