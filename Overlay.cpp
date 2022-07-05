#include "include/Overlay.h"

static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Overlay::Overlay()
{
    if (inputHandler.attached)
    {
        // Setup window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
            std::exit(1);

        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, true);

        window = glfwCreateWindow(2560, 1440, "LinuxOverlay v1.0",NULL, NULL);

        glfwGetWindowPos(window, &inputHandler.xpos, &inputHandler.ypos);
        glfwGetWindowSize(window, &inputHandler.width, &inputHandler.height);

        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
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
        isInitialized = true;
        printf("Successfully initialized overlay...\n");
    }
    else
    {
        printf("Overlay failed to initialize due to input handler not attaching...\n");
        isInitialized = false;
    }
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
    int dispW, dispH;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (inputHandler.windowSetMode)
        {
            sleep(0.5);
            glfwSetWindowPos(window, inputHandler.xpos, inputHandler.ypos);
            glfwSetWindowSize(window, inputHandler.width, inputHandler.height);
            WindowSetUpdate();
        }
        

        if (inputHandler.showMenu)
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

        glfwGetFramebufferSize(window, &dispW, &dispH);
        glViewport(0, 0, dispW, dispH);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    isRunning = false;
}

void Overlay::Update()
{
    static int counter = 0;

    if (inputHandler.showMenu)
    {
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        if (ImGui::Button("Exit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            inputHandler.shouldExit = true;
            std::exit(0);
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void Overlay::WindowSetUpdate()
{
        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        canvasPos.x = 0;
        canvasPos.y = 0;
        canvasSize.x = 2560;
        canvasSize.y = 1440;
        ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
        ImGui::GetBackgroundDrawList()->AddRect(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(255, 255, 255, 255));
}