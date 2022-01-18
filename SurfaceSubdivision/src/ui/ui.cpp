#include "ui.h"

using namespace ui;

subdivisionUI::subdivisionUI()
{
    init(10, 10);

    subdivideType = 0;
    subdivideMesh = 0;
    meshImported = false;
    meshSubdivided = false;
}

subdivisionUI::subdivisionUI(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    init(windowWidth, windowHeight);
}

// 处理输入
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void subdivisionUI::initGlfw(int width, int height)
{
    // 初始化和配置glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    window = glfwCreateWindow(width, height, "Surface Subvision", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void subdivisionUI::init(int width, int height)
{
    // 加载glfw
    initGlfw(width, height);

    // 加载glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return;
    }

    // --------------IM GUI------------
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImVec4 clear_color = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

    // --------------IM GUI------------
    while (!glfwWindowShouldClose(window)) {
        {
            glfwPollEvents();
            processInput(window);

            render();

            // Rendering IMGUI
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
            glfwSwapBuffers(window);
        }
    }
}

void subdivisionUI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    displayConfigWindow();
    displayViewWindow();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void subdivisionUI::displayConfigWindow()
{
    const char* items[] = { "Catmull-Clark","Doo-Sabin", "Loop" };
    static int lastType = subdivideType;
    ImGui::PushItemWidth(-1);
    ImGui::Text("Select Subdivide Algorithm:");
    ImGui::Separator();
    ImGui::ListBox("type", &subdivideType, items, IM_ARRAYSIZE(items), 3);
    if (subdivideType != lastType)
    {
        meshImported = false;
        meshSubdivided = false;
    }
    
    const char* catmullClarkMesh[] = { "cube", "stair" }; // 适用于Catmull-Clark算法的网格模型（四边形面）
    const char* dooSabinMesh[] = { "cube", "regular tetrahedron"}; // 适用于Doo-Sabin算法的网格模型
    const char* loopMesh[] = { "cube", "bunny"}; // 适用于Loop算法的网格模型（三角形面）
    ImGui::PushItemWidth(-1);
    ImGui::Text("Select Mesh:");
    ImGui::Separator();
    switch (subdivideType)
    {
    case 0:
        ImGui::ListBox("mesh", &subdivideMesh, catmullClarkMesh, IM_ARRAYSIZE(catmullClarkMesh), 3); break;
    case 1:
        ImGui::ListBox("mesh", &subdivideMesh, dooSabinMesh, IM_ARRAYSIZE(dooSabinMesh), 3); break;
    case 2:
        ImGui::ListBox("mesh", &subdivideMesh, loopMesh, IM_ARRAYSIZE(loopMesh), 3); break;
    }

    if (ImGui::Button("Import Mesh")) // 导入按钮
    {
        std::string filename;
        switch (subdivideType)
        {
        case 0:
            if (subdivideMesh == 0) filename = "model/cube.obj";
            else if (subdivideMesh == 1) filename = "model/stair.obj";
            break;
        case 1:
            if (subdivideMesh == 0) filename = "model/cube_tri.obj";
            else if (subdivideMesh == 1) filename = "model/regular_tetrahedron.obj";
            break;
        case 2:
            if (subdivideMesh == 0) filename = "model/cube_tri.obj";
            else if (subdivideMesh == 1) filename = "model/bunny.obj";
            break;
        }
        division.importMesh(filename, subdivideType);
        printf("Mesh imported!\n");
        meshImported = true;
        meshSubdivided = false;
    }

    if (meshImported)
    {
        static int iterCount = 0; // 细分迭代次数
        ImGui::InputInt("", &iterCount);

        if (ImGui::Button("Subdivide")) // 细分按钮
        {
            division.subdivide(iterCount, subdivideType);
            printf("Subdivision finished!\n");
            meshSubdivided = true;
        }

        if (ImGui::Button("Export")) // 导出按钮
        {
            if (meshSubdivided)
            {
                if (division.exportFile("model/output/output.obj", subdivideType))
                    printf("Export finished!\n");
                else
                    printf("Export failed!\n");
            }
            else
                printf("No subdivision surface!\n");
        }
    }
}

void subdivisionUI::displayViewWindow()
{

}