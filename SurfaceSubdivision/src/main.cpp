#include "DooSabin/doosabin.h"
#include "Loop/loop.h"
#include "CatmullClark/catmullclark.h"
#include "shader.h"
#include "ui/ui.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    /*DooSabin::mesh m("model/cube.obj");
    for (int i = 0; i < 2; i++)
    {
        printf("正在进行第%d次细分……\n", i + 1);
        m.subdivide();
    }
    m.toFile("subdivided.obj");*/

    /*Loop::mesh m("model/spot_triangulated_good.obj");
    for (int i = 0; i < 1; i++)
    {
        printf("正在进行第%d次细分……\n", i + 1);
        m.subdivide();
    }
    printf("细分完成!\n");
    m.toFile("subdivided.obj");*/

    /*CatmullClark::mesh m("model/cube.obj");
    for (int i = 0; i < 4; i++)
    {
        printf("正在进行第%d次细分……\n", i + 1);
        m.subdivide();
    }
    printf("细分完成!\n");
    m.toFile("subdivided.obj");*/

    ui::subdivisionUI ui;

    //shader s;

    //auto vertices = m.getVertices();
    //auto indices = m.getFaceList();
    //unsigned int VBO, VAO, EBO;

    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &vertices[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //// render loop
    //// -----------
    //while (!glfwWindowShouldClose(window))
    //{
    //    // input
    //    // -----
    //    processInput(window);

    //    // render
    //    // ------
    //    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT);

    //    s.use();
    //    glBindVertexArray(VAO);
    //    auto fIter = indices.begin();
    //    for (; fIter != indices.end(); fIter++)
    //    {
    //        auto faceIndices = *fIter;
    //        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndices.size() * sizeof(int), &faceIndices[0], GL_STATIC_DRAW);
    //        glDrawElements(GL_POLYGON, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    //        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //    }

    //    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    //    // -------------------------------------------------------------------------------
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    //// glfw: terminate, clearing all previously allocated GLFW resources.
    //// ------------------------------------------------------------------
    //glfwTerminate();
    return 0;
}
