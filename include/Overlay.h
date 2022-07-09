#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "Input.h"

class Overlay
{
private:
    GLFWwindow* window;
    Input inputHandler;
    std::thread overlayThread;
    int imageWidth;
    int imageHeight;
    GLuint imageTexture;
public:
    bool isInitialized;
    bool isRunning;
    Overlay();
    ~Overlay();
    void Run();
    void Update();
    void WindowSetUpdate();
    void KeysUpdate();
    bool LoadTextureFromFile(const char* filename);
};
