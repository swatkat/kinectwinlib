#ifndef __KINECTWINLIBCMN_H__
#define __KINECTWINLIBCMN_H__

#include <Windows.h>
#include <mmsystem.h>
#include <cstdio>

// Comment this line to disable debug logging
#define KINECTWIN_FN_LOG

#ifdef KINECTWIN_FN_LOG
    #undef KINECTWIN_FN_ENTRY
    #define KINECTWIN_FN_ENTRY { char szFuncNameTracer[MAX_PATH];                               \
                                 sprintf_s( szFuncNameTracer, MAX_PATH, "+ %s", __FUNCTION__ ); \
                                 OutputDebugString( szFuncNameTracer );                         \
                               }
#else
    #define KINECTWIN_FN_ENTRY
#endif // KINECTWIN_FN_LOG

#ifdef KINECTWIN_FN_LOG
    #undef KINECTWIN_FN_EXIT
    #define KINECTWIN_FN_EXIT  { char szFuncNameTracer[MAX_PATH];                               \
                                 sprintf_s( szFuncNameTracer, MAX_PATH, "- %s", __FUNCTION__ ); \
                                 OutputDebugString( szFuncNameTracer );                         \
                               }
#else
    #define KINECTWIN_FN_EXIT
#endif // KINECTWIN_FN_LOG

// Utility function to convert HRESULT error code to string
void ReportError( char* pszMethod, HRESULT hr );

// Timeout used in waiting for events from NUI runtime
const int KINECTWINLIB_NUI_WAIT_TIMEOUT_IN_MS = 5;

#endif // __KINECTWINLIBCMN_H__