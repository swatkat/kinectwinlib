#include "KinectWinLibTester.h"
#include <conio.h>

int main( int argc, char* argv[] )
{
    KinectWinMgr kinectMgr;
    if( kinectMgr.NuiInit() )
    {
        printf( "\n\n************** PRESS ANY KEY TO EXIT *****************\n\n" );
        _getch();
    }
    kinectMgr.NuiUnInit();
    return 0;
}