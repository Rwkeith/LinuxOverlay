#include "include/Overlay.h"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/Helpers.h"

static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool Overlay::LoadTextureFromFile(const char* filename, GLuint& outTexture, int& outWidth, int& outHeight)
{
    unsigned char* imageData = stbi_load(filename, &outWidth, &outHeight, NULL, 4);
    if (imageData == NULL)
        return false;

    // Create a OpenGL texture identifier
    glGenTextures(1, &outTexture);
    glBindTexture(GL_TEXTURE_2D, outTexture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    stbi_image_free(imageData);

    return true;
}

void Overlay::KeysUpdate()
{
    bool pOpen = true;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBackground;
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 900, main_viewport->WorkPos.y + 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("ArrowKeys", &pOpen, windowFlags);
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text("Position:  x: %d y: %d", inputHandler.xpos, inputHandler.ypos);
    ImGui::Image((void*)(intptr_t)arrowKeysTexture, ImVec2(imageWidth, imageHeight));
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 1200, main_viewport->WorkPos.y + 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Numpad Keys", &pOpen, windowFlags);
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text("(Numpad) Step size: %d", inputHandler.stepSize);
    ImGui::Image((void*)(intptr_t)numKeysTexture, ImVec2(imageWidth, imageHeight));
    ImGui::End();
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
        
        std::string exePath = GetExecutableDir();
        std::string arrowKeysPath = exePath + "resources/arrowKeys.png";
        std::string numKeysPath = exePath + "resources/numKeys.png";

        bool ret = LoadTextureFromFile(arrowKeysPath.c_str(), arrowKeysTexture, imageWidth, imageHeight);
        IM_ASSERT(ret);
        ret = LoadTextureFromFile(numKeysPath.c_str(), numKeysTexture, imageWidth, imageHeight);
        IM_ASSERT(ret);
        printf("Successfully initialized LinuxOverlay v1.01...\n");
    }
    else
    {
        printf("Overlay failed to initialize due to input handler not attaching...\n");
        isInitialized = false;
    }
}

Overlay::~Overlay()
{
    if (window)
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }
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
            KeysUpdate();
        }
        

        if (inputHandler.showMenu)
        {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
            Update();
            ImGui::ShowDemoWindow();
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
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        if (ImGui::Button("Exit"))
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