#include <Windows.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "GameUtility/File/Include/Csv.hpp"
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommandLine, int nShowCommand)
#endif
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    /********************************************
    **         Initialize
    *********************************************/
    
    /********************************************
    **         MainLoop
    *********************************************/
    
    /********************************************
    **         Check MemoryLeaks
    *********************************************/
    _CrtDumpMemoryLeaks();
    /********************************************
    **         Termination Process
    *********************************************/
    exit(EXIT_SUCCESS);
   
}

