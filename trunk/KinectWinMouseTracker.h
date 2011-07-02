#ifndef __KINECTWINMOUSETRACKER_H__
#define __KINECTWINMOUSETRACKER_H__
#include "KinectWinLibCmn.h"

class KinectWinMouseTracker
{
public:
    KinectWinMouseTracker();
    virtual ~KinectWinMouseTracker();
    void NuiGotSkeletonAlert( );

private:
};

#endif // __KINECTWINMOUSETRACKER_H__