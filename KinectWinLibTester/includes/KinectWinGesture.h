#ifndef __KINECTWINGESTURE_H__
#define __KINECTWINGESTURE_H__
#include "KinectWinLibCmn.h"
#include <vector>

typedef struct _NuiTrackedSkeletalObjectLocation
{
    float x;
    float y;
} NuiTrackedSkeletalObjectLocation;

typedef enum _eHandGestureType
{
    eHandGestureNone = 0,
    eeHandGestureHorizontalLeftToRight,
    eeHandGestureHorizontalRightToLeft,
    eeHandGestureVerticalTopToBottom,
    eeHandGestureVerticalBottomToTop
} eHandGestureType;

class KinectWinGesture
{
public:
    KinectWinGesture();
    ~KinectWinGesture();
    static DWORD WINAPI NuiGestureProcessThread( LPVOID pParam );
    eHandGestureType GuessHandGuesture();
    void SaveHandNuiPosition( float x, float y );
private:
    std::vector<NuiTrackedSkeletalObjectLocation> m_NuiHandLocationHistory;
};

#endif // __KINECTWINGESTURE_H__