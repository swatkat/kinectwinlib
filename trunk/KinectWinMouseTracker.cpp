#include "KinectWinMouseTracker.h"
#include <MSR_NuiApi.h>

KinectWinMouseTracker::KinectWinMouseTracker()
{
    KINECTWIN_FN_ENTRY;
    KINECTWIN_FN_EXIT;
}

KinectWinMouseTracker::~KinectWinMouseTracker()
{
    KINECTWIN_FN_ENTRY;
    KINECTWIN_FN_EXIT;
}

void KinectWinMouseTracker::NuiGotSkeletonAlert()
{
    // Get skeleton frame
    NUI_SKELETON_FRAME nuiSkeletonFrame = {0};
    HRESULT hr = NuiSkeletonGetNextFrame( 0, &nuiSkeletonFrame );
    if( FAILED( hr ) )
    {
        ReportError( "NuiSkeletonGetNextFrame", hr );
        return;
    }

    // Check if we have any skeleton
    int i = 0;
    bool bFoundSkeleton = false;
    for( i = 0; i < NUI_SKELETON_COUNT; i++ )
    {
        // Found atleast one tracked skeleton
        if( nuiSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED )
        {
            bFoundSkeleton = true;
            break;
        }
    }

    if( bFoundSkeleton )
    {
        // Smooth the skeleton positions
        NUI_TRANSFORM_SMOOTH_PARAMETERS nuiSmoothParams;
        nuiSmoothParams.fCorrection = 0.3f;
        nuiSmoothParams.fJitterRadius = 1.0f;
        nuiSmoothParams.fMaxDeviationRadius = 0.5f;
        nuiSmoothParams.fPrediction = 0.4f;
        nuiSmoothParams.fSmoothing = 0.7f;
        NuiTransformSmooth( &nuiSkeletonFrame, &nuiSmoothParams );

        // Convert left hand vector4 to x,y coordinates
        Vector4 v4LeftHand = nuiSkeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
        float fLeftX = 0;
        float fLeftY = 0;
        NuiTransformSkeletonToDepthImageF( v4LeftHand, &fLeftX, &fLeftY );

        // Get desktop boundaries
        RECT rectDesktop;
        HWND hDesktopWnd = GetDesktopWindow();
        GetClientRect( hDesktopWnd, &rectDesktop );

        // Set mouse cursor boundaries
        ClipCursor( &rectDesktop );

        // Convert floating coordinates to integer pixel coordinates
        POINT ptMouse;
        ptMouse.x = (int) ( fLeftX * rectDesktop.right + 0.5f ); // rectDesktop.right --> width
        ptMouse.y = (int) ( fLeftY * rectDesktop.bottom + 0.5f ); // rectDesktop.bottom --> height

        // Move cursor
        SetCursorPos( ptMouse.x, ptMouse.y );
    }
}