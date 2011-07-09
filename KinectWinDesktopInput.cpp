#include "KinectWinDesktopInput.h"

void SendMouseInputs( float x, float y )
{
    // Get desktop size and set mouse boundary
    RECT rectDesktop = {0};
    rectDesktop.right = GetSystemMetrics( SM_CXSCREEN );
    rectDesktop.bottom = GetSystemMetrics( SM_CYSCREEN );
    ClipCursor( &rectDesktop );

    // Move cursor
    INPUT mouseInput = {0};
    mouseInput.type = INPUT_MOUSE;
    mouseInput.mi.dx = (int)( x * 65535 );
    mouseInput.mi.dy = (int)( y * 65535 );
    mouseInput.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    SendInput( 1, &mouseInput, sizeof( INPUT ) );
}

void SendAlt( bool bUp )
{
    INPUT kbInput = {0};
    kbInput.type = INPUT_KEYBOARD;
    kbInput.ki.wVk = VK_MENU;
    kbInput.ki.wScan = 56;
    kbInput.ki.dwFlags = ( bUp ) ? ( KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP ) : KEYEVENTF_SCANCODE;
    SendInput( 1, &kbInput, sizeof( INPUT ) );
}

void SendEsc( bool bUp )
{
    INPUT kbInput = {0};
    kbInput.type = INPUT_KEYBOARD;
    kbInput.ki.wVk = VK_ESCAPE;
    kbInput.ki.wScan = 1;
    kbInput.ki.dwFlags = ( bUp ) ? ( KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP ) : KEYEVENTF_SCANCODE;
    SendInput( 1, &kbInput, sizeof( INPUT ) );
}

void SendTab( bool bUp )
{
    INPUT kbInput = {0};
    kbInput.type = INPUT_KEYBOARD;
    kbInput.ki.wVk = VK_TAB;
    kbInput.ki.wScan = 15;
    kbInput.ki.dwFlags = ( bUp ) ? ( KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP ) : KEYEVENTF_SCANCODE;
    SendInput( 1, &kbInput, sizeof( INPUT ) );
}

void SendKeyboardInputs( eHandGestureType eHandGestureType )
{
    if( eeHandGestureHorizontalLeftToRight == eHandGestureType ) 
    {
        SendAlt( false );
        SendEsc( false );
        SendEsc( true );
        SendAlt( true );
    }
}