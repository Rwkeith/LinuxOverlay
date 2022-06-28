#include "input.h"
#include "overlay.h"

bool showMenu = true;
bool shouldExit = false;

int main(int, char**)
{
    input inputHandler;
    Overlay ol;
    ol.Run();
    return 0;
}