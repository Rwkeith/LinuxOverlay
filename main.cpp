#include "include/Input.h"
#include "include/Overlay.h"

int main(int, char**)
{
    Overlay overlay;
    if (overlay.isInitialized)
    {
        overlay.Run();
    }
    return 0;
}