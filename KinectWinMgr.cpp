#include "KinectWinMgr.h"
#include "KinectWinDesktopInput.h"
#include <MSR_NuiApi.h>

// Constructor
KinectWinMgr::KinectWinMgr():
m_hNuiSkeletonEvent( NULL ),
m_hNuiProcessThread( NULL ),
m_hNuiGuessGestureThread( NULL ),
m_hEvNuiProcessThreadStopEvent( NULL )
{
    KINECTWIN_FN_ENTRY;
    KINECTWIN_FN_EXIT;
}

// Destructor
KinectWinMgr::~KinectWinMgr()
{
    KINECTWIN_FN_ENTRY;
    KINECTWIN_FN_EXIT;
}

// Initialize NUI runtime and start a thread to listen for events
bool KinectWinMgr::NuiInit()
{
    KINECTWIN_FN_ENTRY;
    if( m_hNuiProcessThread || m_hEvNuiProcessThreadStopEvent || m_hNuiSkeletonEvent )
    {
        OutputDebugString( "KinectWinMgr::Nui_Init(): Already initialized" );
        KINECTWIN_FN_EXIT;
        return true;
    }

    // Initialize NUI
    HRESULT hr = NuiInitialize( NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_SKELETON );
    if( FAILED( hr ) )
    {
        ReportError( "NuiInitialize", hr );
        KINECTWIN_FN_EXIT;
        return false;
    }

    // Enable skeleton tracking
    m_hNuiSkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    hr = NuiSkeletonTrackingEnable( m_hNuiSkeletonEvent, 0 );
    if( FAILED( hr ) )
    {
        ReportError( "NuiSkeletonTrackingEnable", hr );
        KINECTWIN_FN_EXIT;
        return false;
    }

    // Start the NUI processing thread
    m_hEvNuiProcessThreadStopEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
    m_hNuiProcessThread = CreateThread( NULL, 0, NuiProcessThread, this, 0, NULL );
    m_hNuiGuessGestureThread = CreateThread( NULL, 0, NuiGuessGestureThread, this, 0, NULL );

    KINECTWIN_FN_EXIT;
    return true;
}

// Terminate the listening thread and uninitialize NUI runtime
void KinectWinMgr::NuiUnInit()
{
    KINECTWIN_FN_ENTRY;
    
    // Disable skeleton tracking
    NuiSkeletonTrackingDisable();

    // Shutdown NUI
    NuiShutdown();

    // Stop the NUI processing thread
    if( m_hEvNuiProcessThreadStopEvent )
    {
        // Signal the thread and wait for it to terminate
        SetEvent( m_hEvNuiProcessThreadStopEvent );
        if( m_hNuiProcessThread )
        {
            WaitForSingleObject( m_hNuiProcessThread, INFINITE );
            CloseHandle( m_hNuiProcessThread );  
        }
        if( m_hNuiGuessGestureThread )
        {
            WaitForSingleObject( m_hNuiGuessGestureThread, INFINITE );
            CloseHandle( m_hNuiGuessGestureThread );
        }
        CloseHandle( m_hEvNuiProcessThreadStopEvent );
    }

    // Close skeleton tracking alert event handle
    if( m_hNuiSkeletonEvent )
    {
        CloseHandle( m_hNuiSkeletonEvent );
    }
    m_hNuiProcessThread = NULL;
    m_hNuiGuessGestureThread = NULL;
    m_hEvNuiProcessThreadStopEvent = NULL;
    m_hNuiSkeletonEvent = NULL;
    KINECTWIN_FN_EXIT;
}

// Thread that processes events from NUI runtime
DWORD WINAPI KinectWinMgr::NuiProcessThread( LPVOID pParam )
{
    KINECTWIN_FN_ENTRY;
    KinectWinMgr* pThis = (KinectWinMgr*)pParam;
    HANDLE hEvents[2];
    int nEventIdx = 0;

    // Configure events to be listened on
    hEvents[0] = pThis->m_hEvNuiProcessThreadStopEvent;
    hEvents[1] = pThis->m_hNuiSkeletonEvent;

    // Main thread loop
    while(1)
    {
        // Wait for an event to be signalled
        nEventIdx = WaitForMultipleObjects( sizeof(hEvents)/sizeof(hEvents[0]), hEvents, FALSE, KINECTWINLIB_NUI_WAIT_TIMEOUT_IN_MS );
        if( 0 == ( WAIT_OBJECT_0 + nEventIdx ) )
        {
            // Exit from loop
            SetEvent( pThis->m_hEvNuiProcessThreadStopEvent );
            break;
        }
        else if( 1 == ( WAIT_OBJECT_0 + nEventIdx ) )
        {
            // Process skeleton motion alert
            pThis->NuiGotSkeletonAlert();
        }
        else if( WAIT_FAILED == nEventIdx )
        {
#ifdef KINECTWIN_DBG_LOG
            OutputDebugString( "WaitForMultipleObjects error!" );
#endif // KINECTWIN_DBG_LOG
            break;
        }
    }
    KINECTWIN_FN_EXIT;
    return 0;
}

// Thread that guesses gesture at regular intervals.
DWORD WINAPI KinectWinMgr::NuiGuessGestureThread( LPVOID pParam )
{
    KINECTWIN_FN_ENTRY;
    int nWaitRetCode = 0;
    eHandGestureType detectedHandGuesture = eHandGestureNone;
    KinectWinMgr* pThis = (KinectWinMgr*)pParam;
    while( 1 )
    {
        nWaitRetCode = WaitForSingleObject( pThis->m_hEvNuiProcessThreadStopEvent, KINECTWINLIB_NUI_GESTURE_WAIT_TIMEOUT_IN_MS );
        if( WAIT_OBJECT_0 == nWaitRetCode )
        {
            // Exit from loop
            SetEvent( pThis->m_hEvNuiProcessThreadStopEvent );
            break;
        }
        else if( WAIT_TIMEOUT == nWaitRetCode )
        {
            // Check for any guesture every second?
            detectedHandGuesture = pThis->m_kinectWinGesture.GuessHandGuesture();
            SendKeyboardInputs( detectedHandGuesture );
        }
        else
        {
#ifdef KINECTWIN_DBG_LOG
            OutputDebugString( "WaitForSingleObject error!" );
#endif // KINECTWIN_DBG_LOG
            break;
        }
    }
    KINECTWIN_FN_EXIT;
    return 0;
}

void KinectWinMgr::NuiGotSkeletonAlert()
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
        float fLeftX = 0, fLeftY = 0;
        Vector4 v4LeftHand = nuiSkeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
        NuiTransformSkeletonToDepthImageF( v4LeftHand, &fLeftX, &fLeftY );

        // Move cursor
        SendMouseInputs( fLeftX, fLeftY );

        // Save coordinates
        m_kinectWinGesture.SaveHandNuiPosition( fLeftX, fLeftY );
    }
}