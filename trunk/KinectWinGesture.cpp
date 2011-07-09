#include "KinectWinGesture.h"

#ifdef KINECTWIN_FN_LOG
const char* KINECTWINGESTURE_TYPE_STRING[] = { "None",
                                               "Horizontal Left to Right",
                                               "Horizontal Right to Left",
                                               "Vertical Top to Bottom",
                                               "Vertical Bottom to Top" };
#endif // KINECTWIN_FN_LOG

KinectWinGesture::KinectWinGesture()
{
    KINECTWIN_FN_ENTRY;
    KINECTWIN_FN_EXIT;
}

KinectWinGesture::~KinectWinGesture()
{
    KINECTWIN_FN_ENTRY;
    KINECTWIN_FN_EXIT;
}

void KinectWinGesture::SaveHandNuiPosition( float x, float y )
{
    NuiTrackedSkeletalObjectLocation nuiHandPos;
    nuiHandPos.x = x;
    nuiHandPos.y = y;
    m_NuiHandLocationHistory.push_back( nuiHandPos );
}

eHandGestureType KinectWinGesture::GuessHandGuesture()
{
    eHandGestureType eHandGesture = eHandGestureNone;
    if( m_NuiHandLocationHistory.size() > 30 )
    {
        float fMotionDelta = m_NuiHandLocationHistory.front().x - m_NuiHandLocationHistory.back().x;
        if( ( fMotionDelta > -0.1 ) && ( fMotionDelta < 0.1 ) )
        {
            // Not enough variations in X-axis to treat as a gesture. Let's check Y-axis.
            fMotionDelta = m_NuiHandLocationHistory.front().y - m_NuiHandLocationHistory.back().y;
            if( ( fMotionDelta > -0.1 ) && ( fMotionDelta < 0.1 ) )
            {
                // Not enough variations in Y-axis to treat as a gesture. Let's check Z-axis?
            }
            else
            {
                eHandGesture = ( fMotionDelta > 0 ) ? eeHandGestureVerticalBottomToTop : eeHandGestureVerticalTopToBottom;
            }
        }
        else
        {
            eHandGesture =  ( fMotionDelta > 0 ) ? eeHandGestureHorizontalRightToLeft : eeHandGestureHorizontalLeftToRight;
        }
        m_NuiHandLocationHistory.clear();
    }
#ifdef KINECTWIN_FN_LOG
    OutputDebugString( KINECTWINGESTURE_TYPE_STRING[eHandGesture] );
#endif // KINECTWIN_FN_LOG
    return eHandGesture;
}
