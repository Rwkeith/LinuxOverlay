#include "input.h"
#include "overlay.h"
#include "utility.h"

bool showMenu = false;
bool shouldExit = false;

int main(int, char**)
{
    bool wait = true;
    int valheimPID = utility::GetProcessPID("valheim.x86_64", wait);
    long monolibBase = utility::GetModuleHandle("libmonobdwgc-2.0.so", valheimPID);
    long freeSpaceAddr = utility::GetFreeSpaceAddr(valheimPID);
    // Run() blocks
    input inputHandler;
    Overlay ol;
    ol.Run();
    return 0;
}