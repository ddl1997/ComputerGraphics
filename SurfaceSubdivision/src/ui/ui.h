#ifndef SUBDIVISION_UI_H
#define SUBDIVISION_UI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ui {

    class subdivisionUI {
    public:
        subdivisionUI();

        subdivisionUI(int width, int height);

        void initGlfw(int width, int height);

        void init(int width, int height);

        void render();

        void displayConfigWindow();

        void displayViewWindow();

    private:
        int windowWidth;
        int windowHeight;
        GLFWwindow* window;


    };
}
#endif // !SUBDIVISION_UI_H
