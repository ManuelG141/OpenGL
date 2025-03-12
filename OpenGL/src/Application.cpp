#include <GL/glew.h> // You have to include this before any other OpenGL include or it will trow a error
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// glm page https://github.com/g-truc/glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// ImGui page https://github.com/ocornut/imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "TestClearColor.h"
#include "TestTriangle.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Simple OpenGL practice", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // To activate frame rate

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl; // Currently from Intel Integrated GPU

	{

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // IF using Docking Branch

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();

		test::TestTriangle test;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			test.OnUpdate(0.0f);
			test.OnRender();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			test.OnImGuiRender();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			// Rendering
			// (Your code clears your framebuffer, renders your other stuff etc.)
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	};

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}