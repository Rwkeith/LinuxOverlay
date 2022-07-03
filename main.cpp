#include "Input.h"
#include "Overlay.h"
#include "Utility.h"

bool showMenu = false;
bool shouldExit = false;

int main(int, char**)
{
    bool wait = true;
    int valheimPID = Utility::GetProcessPID("valheim.x86_64", wait);
    /*long monolibBase = */Utility::GetModuleHandle("libmonobdwgc-2.0.so", valheimPID);
    /*long freeSpaceAddr = */Utility::GetFreeSpaceAddr(valheimPID);
    // Run() blocks
    Input inputHandler;
    Overlay ol;
    ol.Run();
    return 0;
}