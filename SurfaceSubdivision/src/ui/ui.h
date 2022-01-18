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

        void initGlfw(int width, int height); // ��ʼ��glfw

        void init(int width, int height); // ��ʼ��UI

        void render();

        void displayConfigWindow(); // ���ô���

        void displayViewWindow();

    private:
        int windowWidth;
        int windowHeight;
        GLFWwindow* window;
        int subdivideType; // ϸ���㷨����
        int subdivideMesh; // ����
        bool meshImported; // ����ģ���Ƿ��Ѿ�����
        bool meshSubdivided; // ����ģ���Ƿ��Ѿ�ϸ��
        subdivision division;

    };
}
#endif // !SUBDIVISION_UI_H
