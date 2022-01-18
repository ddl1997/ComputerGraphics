#ifndef SUBDIVISION_UI_H
#define SUBDIVISION_UI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../subdivision.h"

namespace ui {

    class subdivisionUI {
    public:
        subdivisionUI();

        subdivisionUI(int width, int height);

        void initGlfw(int width, int height); // 初始化glfw

        void init(int width, int height); // 初始化UI

        void render();

        void displayConfigWindow(); // 配置窗口

        void displayViewWindow();

    private:
        int windowWidth;
        int windowHeight;
        GLFWwindow* window;
        int subdivideType; // 细分算法类型
        int subdivideMesh; // 网格
        bool meshImported; // 网格模型是否已经导入
        bool meshSubdivided; // 网格模型是否已经细分
        subdivision division;

    };
}
#endif // !SUBDIVISION_UI_H
