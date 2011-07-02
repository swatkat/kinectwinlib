#include "KinectWinLibCmn.h"

// Utility function to convert HRESULT error code to string
void ReportError( char* pszMethod, HRESULT hr )
{
    KINECTWIN_FN_ENTRY;
    DWORD dwError = HRESULT_CODE(hr);
    TCHAR szErrMsg[MAX_PATH];
    TCHAR szMessage[MAX_PATH];
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, szErrMsg, MAX_PATH, NULL );
    sprintf_s( szMessage, MAX_PATH, "%s error: %s", pszMethod, szErrMsg );
    OutputDebugString( szMessage );
    KINECTWIN_FN_EXIT;
}