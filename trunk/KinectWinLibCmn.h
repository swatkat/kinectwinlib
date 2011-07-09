#ifndef __KINECTWINLIBCMN_H__
#define __KINECTWINLIBCMN_H__

#include <Windows.h>
#include <mmsystem.h>
#include <cstdio>

// Comment this line to disable debug logging
#define KINECTWIN_DBG_LOG

#ifdef KINECTWIN_DBG_LOG
    #undef KINECTWIN_FN_ENTRY
    #define KINECTWIN_FN_ENTRY { char szFuncNameTracer[MAX_PATH];                               \
                                 sprintf_s( szFuncNameTracer, MAX_PATH, "+ %s", __FUNCTION__ ); \
                                 OutputDebugString( szFuncNameTracer );                         \
                               }
#else
    #define KINECTWIN_FN_ENTRY
#endif // KINECTWIN_DBG_LOG

#ifdef KINECTWIN_DBG_LOG
    #undef KINECTWIN_FN_EXIT
    #define KINECTWIN_FN_EXIT  { char szFuncNameTracer[MAX_PATH];                               \
                                 sprintf_s( szFuncNameTracer, MAX_PATH, "- %s", __FUNCTION__ ); \
                                 OutputDebugString( szFuncNameTracer );                         \
                               }
#else
    #define KINECTWIN_FN_EXIT
#endif // KINECTWIN_DBG_LOG

// Utility function to convert HRESULT error code to string
void ReportError( char* pszMethod, HRESULT hr );

// Timeout used in waiting for events from NUI runtime
const int KINECTWINLIB_NUI_WAIT_TIMEOUT_IN_MS = 5;

// Timeout used in gesture guessing thread to initiate guessing
const int KINECTWINLIB_NUI_GESTURE_WAIT_TIMEOUT_IN_MS = 100;

// Minimum number of hand positions that should be saved for guessing gesture
const int KINECTWINLIB_NUI_HAND_LOCATION_HISTORY_THRESHOLD = 30;

// Low threshold value for hand position variation delta
const float KINECTWINLIB_NUI_GESTURE_LOW_THRESHOLD = -0.1f;

// High threshold value for hand position variation delta
const float KINECTWINLIB_NUI_GESTURE_HIGH_THRESHOLD = 0.1f;

#endif // __KINECTWINLIBCMN_H__