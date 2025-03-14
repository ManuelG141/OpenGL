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
#include "TestSquare.h"
#include "TestUniform.h"
#include "TestTexture.h"
#include "TestMath.h"
#include "TestMultipleObjects.h"

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

	glfwSwapInterval(0); // To activate frame rate

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

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest); // TestMenu Instance heap allocated
		currentTest = testMenu;

		// Add test applications just like this
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTriangle>("Basic Triangle");
		testMenu->RegisterTest<test::TestSquare>("Basic Square");
		testMenu->RegisterTest<test::TestUniform>("Basic Uniform");
		testMenu->RegisterTest<test::TestTexture>("Basic Texture");
		testMenu->RegisterTest<test::TestMath>("Basic Math");
		testMenu->RegisterTest<test::TestMultipleObjects>("Multiple Objects");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest) // Not null
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("Back to menu")) // If the first two arguments, left to right are false, then the last argument won't be evaluated
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			// Rendering
			// (Your code clears your framebuffer, renders your other stuff etc.)
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	};

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}