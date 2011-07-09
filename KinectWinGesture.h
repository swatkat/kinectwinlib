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
    eHandGestureType GuessHandGuesture();
    void SaveHandNuiPosition( float x, float y );

private:
    CRITICAL_SECTION m_csNuiHandLocationHistory;
    std::vector<NuiTrackedSkeletalObjectLocation> m_NuiHandLocationHistory;
};

#endif // __KINECTWINGESTURE_H__