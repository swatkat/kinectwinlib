#include "KinectWinMgr.h"
#include <MSR_NuiApi.h>

// Constructor
KinectWinMgr::KinectWinMgr():
m_hNuiSkeletonEvent( NULL ),
m_hNuiProcessThread( NULL ),
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

    KINECTWIN_FN_EXIT;
    return true;
}

// Terminate the listening thread and uninitialize NUI runtime
void KinectWinMgr::NuiUnInit()
{
    KINECTWIN_FN_ENTRY;
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
        CloseHandle( m_hEvNuiProcessThreadStopEvent );
    }

    // Disable skeleton tracking
    NuiSkeletonTrackingDisable();

    // Shutdown NUI
    NuiShutdown();

    // Close skeleton tracking alert event handle
    if( m_hNuiSkeletonEvent )
    {
        CloseHandle( m_hNuiSkeletonEvent );
    }
    m_hNuiProcessThread = NULL;
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
            break;
        }
        else if( 1 == ( WAIT_OBJECT_0 + nEventIdx ) )
        {
            // Delegate to mouse tracker
            pThis->m_kinectWinMouseTracker.NuiGotSkeletonAlert();
        }
    }
    KINECTWIN_FN_EXIT;
    return 0;
}