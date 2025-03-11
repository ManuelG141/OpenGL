#include <GL/glew.h> // You have to include this before any other OpenGL include or it will trow a error
#include <GLFW/glfw3.h>
#include <iostream>

// GLFW Documentation: https://www.glfw.org/documentation.html
// GLEW Documentation: https://glew.sourceforge.net/basic.html

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glewInit(); // Glew cannot be initialized here, it needs an appropiate context before calling it

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Simple Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl; // Currently from Intel Integrated GPU

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Legacy OpenGL code
        glBegin(GL_TRIANGLES); // Just to draw a triangle
        glVertex2f(0.0f, 0.8f); // Default projection matrix from -1 to 1
        glVertex2f(0.8f, -0.8f);
        glVertex2f(-0.8f, -0.8f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}