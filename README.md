# LinuxOverlay
An external overlay which uses GLFW and ImGUI for drawing

## About

This overlay setup uses GLFW as window manager.  It configures the window to be top-most, transparent, and borderless.  When toggling the overlay, mouse passthrough and the GUI is toggled.  Keyboard input is captured using the device /dev/input/eventN where N is for the keyboard device. Therefore Sudo/Root privledges are required to capture input.

## Usage

Run as sudo.  Press insert key to toggle the overlay.

## Building

Uses Meson with Ninja to build.
