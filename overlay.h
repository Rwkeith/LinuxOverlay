#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

extern bool showMenu;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

class Overlay
{
private:
    GLFWwindow* window;
public:
    Overlay();
    ~Overlay();
    void Run();
    void Update();
};

Overlay::Overlay()
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        std::exit(1);

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, true);

    int count, windowWidth, windowHeight, monitorX, monitorY;

    // I am assuming that main monitor is in the 0 position
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
    // width: 75% of the screen
    windowWidth = static_cast<int>(videoMode->width / 1.5);
    // aspect ratio 16 to 9
    windowHeight = static_cast<int>(videoMode->height / 16 * 9);

    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    // Create window with graphics context
    printf("Monitorx: %i\n", monitorX);
    printf("Monitory: %i\n", monitorY);

    window = glfwCreateWindow(2560, 1440, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    
    //glfwSetWindowMonitor(window, monitors[0], 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
    glfwSetWindowPos(window, monitorX + (videoMode->width - windowWidth) / 2, monitorY + (videoMode->height - windowHeight) / 2);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    //glfwSetWindowAttrib(window, GLFW_MAXIMIZED, GLFW_TRUE);
    //glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_TRUE);
    glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);

    if (window == NULL)
        std::exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

Overlay::~Overlay()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Overlay::Run()
{
    while (!glfwWindowShouldClose(window))
    {       
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if (showMenu)
        {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
            Update();
        }
        else
        {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void Overlay::Update()
{
    static int counter = 0;

    if (showMenu)
    {
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        if (ImGui::Button("Exit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            std::exit(0);
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}
