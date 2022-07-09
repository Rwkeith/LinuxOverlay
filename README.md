![Imgur Image](https://i.imgur.com/rM3tClN.png)

# LinuxOverlay
An external overlay which uses GLFW and ImGUI for drawing

## About

This overlay setup uses GLFW as window manager.  It configures the window to be top-most, transparent, and borderless.  When toggling the overlay, mouse passthrough and the GUI is toggled.  Keyboard input is captured using the device /dev/input/eventN where N is for the keyboard device. Therefore Sudo/Root privledges are required to capture input.

## Using as library

Example using LinuxOverlay with your project
```cpp
#include "Overlay.h"

int main(int, char**)
{
    Overlay overlay;
    if (overlay.isInitialized)
    {
        overlay.Run();
    }
    return 0;
}
```

## Interface Usage

Run as sudo.  The overlay will start in window set mode.  Use the arrowkeys and numpad to adjust.  Then spacebar to set.

![Imgur Image](https://i.imgur.com/zTeUZTR.png)

Press insert key to toggle the overlay.

## Building

Uses Meson with Ninja to build.

### Standalone build
```console
meson builddir
ninja -C builddir
```

Then Run
```console
sudo ./builddir/LinuxOverlay
```

### Use as library for your project

Your project needs a `meson_options.txt` and a `meson.build` file in it's root directory.

In `meson_options.txt`:
```meson
option('LinuxOverlayBuildType', type : 'combo', value : 'dependency', choices: ['standalone', 'dependency'])
```
In `meson.build`:
```meson
LinuxOverlayLib = subproject('LinuxOverlay').get_variable('LinuxOverlayDep')

yourIncs = include_directories('.', './include')

ImGUIinc = subproject('ImGUI').get_variable('imguiInc')  # will be pulled by LinuxOverlay
glfwInc = subproject('glfw').get_variable('glfwIncs')    # will be pulled by ImGUI

files = files(
    'Main.cpp',
)

executable('MyExe', files, include_directories : [yourIncs, ImGUIinc, glfwInc], dependencies : [LinuxOverlayLib] )
```
