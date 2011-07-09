#include "KinectWinGesture.h"

#ifdef KINECTWIN_DBG_LOG
const char* KINECTWINGESTURE_TYPE_STRING[] = { "None",
                                               "Horizontal Left to Right",
                                               "Horizontal Right to Left",
                                               "Vertical Top to Bottom",
                                               "Vertical Bottom to Top" };
#endif // KINECTWIN_DBG_LOG

KinectWinGesture::KinectWinGesture()
{
    KINECTWIN_FN_ENTRY;
    InitializeCriticalSection( &m_csNuiHandLocationHistory );
    KINECTWIN_FN_EXIT;
}

KinectWinGesture::~KinectWinGesture()
{
    KINECTWIN_FN_ENTRY;
    DeleteCriticalSection( &m_csNuiHandLocationHistory );
    KINECTWIN_FN_EXIT;
}

void KinectWinGesture::SaveHandNuiPosition( float x, float y )
{
    NuiTrackedSkeletalObjectLocation nuiHandPos;
    nuiHandPos.x = x;
    nuiHandPos.y = y;
    if( TryEnterCriticalSection( &m_csNuiHandLocationHistory ) )
    {
        m_NuiHandLocationHistory.push_back( nuiHandPos );
        LeaveCriticalSection( &m_csNuiHandLocationHistory );
    }
}

eHandGestureType KinectWinGesture::GuessHandGuesture()
{
    eHandGestureType eHandGesture = eHandGestureNone;
    if( TryEnterCriticalSection( &m_csNuiHandLocationHistory ) )
    {
        if( m_NuiHandLocationHistory.size() > KINECTWINLIB_NUI_HAND_LOCATION_HISTORY_THRESHOLD )
        {
            float fMotionDelta = m_NuiHandLocationHistory.front().x - m_NuiHandLocationHistory.back().x;
            if( ( fMotionDelta > KINECTWINLIB_NUI_GESTURE_LOW_THRESHOLD ) && ( fMotionDelta < KINECTWINLIB_NUI_GESTURE_HIGH_THRESHOLD ) )
            {
                // Not enough variations in X-axis to treat as a gesture. Let's check Y-axis.
                fMotionDelta = m_NuiHandLocationHistory.front().y - m_NuiHandLocationHistory.back().y;
                if( ( fMotionDelta > KINECTWINLIB_NUI_GESTURE_LOW_THRESHOLD ) && ( fMotionDelta < KINECTWINLIB_NUI_GESTURE_HIGH_THRESHOLD ) )
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
        LeaveCriticalSection( &m_csNuiHandLocationHistory );
    }
#ifdef KINECTWIN_DBG_LOG
    OutputDebugString( KINECTWINGESTURE_TYPE_STRING[eHandGesture] );
#endif // KINECTWIN_DBG_LOG

    return eHandGesture;
}
