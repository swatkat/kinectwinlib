#ifndef __KINECTWINMGR_H__
#define __KINECTWINMGR_H__
#include "KinectWinLibCmn.h"
#include "KinectWinMouseTracker.h"

// Kinect manager class that initializes NUI and listens for events from NUI runtime
class KinectWinMgr
{
public:
    KinectWinMgr();
    virtual ~KinectWinMgr();
    bool NuiInit();
    void NuiUnInit();

private:
    static DWORD WINAPI NuiProcessThread( LPVOID pParam );
    HANDLE m_hNuiProcessThread;
    HANDLE m_hEvNuiProcessThreadStopEvent;
    HANDLE m_hNuiSkeletonEvent;
    KinectWinMouseTracker m_kinectWinMouseTracker;
};

#endif // __KINECTWINMGR_H__