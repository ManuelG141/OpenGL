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

// Imgui page https://github.com/ocornut/imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(960, 540, "Simple Texture", NULL, NULL);
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
        float positions[] = { 
            // rectangle, Texture Coordinates
			  -50.f, -50.f, 0.0f, 0.0f,
			   50.f, -50.f, 1.0f, 0.0f,
			   50.f,  50.f, 1.0f, 1.0f,
			  -50.f,  50.f, 0.0f, 1.0f
        };

        // Index buffer
        unsigned int indices[] = {
            0, 1, 2, // Use this indices to draw the first triangle
            2, 3, 0  // Use this indices to draw the second triangle
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Creating VertexArray
        VertexArray va;
        // Creating VertexBuffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        // Creating Layout for our VertexBuffer and pushing it into va object
        VertexBufferLayout layout;
        layout.Push<float>(2); // Rectangle
        layout.Push<float>(2); // Texture Coordinates
        va.AddBuffer(vb, layout);
        // Creating IndexBuffer
        IndexBuffer ib(indices, 6);

        // Matrices (MVP or PVM) = Model * View * Projection
        // Model refers to the model transform (Similar to ROS) of the actual model that we want to draw
        // View refers to the position and orientation of the camera (to simulate a camera)
        // Projection transforms from custom coordinate space to normalized space coordinates

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); // Ortho and perspective
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // View matrix, moved to the right -100 units
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)); // View matrix, moved to the left 200 units, and up 200 units

        // Multiplication on matrices isn't commutative, the order here matters
        //glm::mat4 mvp = proj * view * model; // Multiplication in OpenGL it's from right to left

        // Creating Shaders
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/textures/thanos_patrick.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0); // Here we enter the slot of the texture

        Renderer renderer;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();

        // Unbind everything to set up like we have multiple objects later
        va.Unbind(); // Vertex Array
        vb.Unbind(); // Vertex Buffer
        ib.Unbind(); // Index Buffer
        shader.Unbind(); // Shaders

        glm::vec3 model_translationA(280.0f, 270.0f, 0.f);
        glm::vec3 model_translationB(680.0f, 270.0f, 0.f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//ImGui::ShowDemoWindow(); // Show demo window! :)

			{ // Draw Model A
				glm::mat4 model = glm::translate(glm::mat4(1.0f), model_translationA);
				glm::mat4 mvp = proj * view * model;
                shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);
			}

			{ // Draw Model B
				glm::mat4 model = glm::translate(glm::mat4(1.0f), model_translationB);
				glm::mat4 mvp = proj * view * model;
                shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);
			}

			{
				static float f = 0.0f;

				ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("Set the modelA translation.");               // Display some text (you can use a format strings too)
				ImGui::SliderFloat3("translation A", &model_translationA.x, 0, 960.0f);

                ImGui::Text("Set the modelB translation.");
                ImGui::SliderFloat3("translation B", &model_translationB.x, 0, 960.0f);


                if (ImGui::Button("Reset Translation"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                {
                    model_translationA.x = 280.f;
                    model_translationA.y = 270.f;
                    model_translationA.z = 0;

					model_translationB.x = 680.f;
					model_translationB.y = 270.f;
					model_translationB.z = 0;
                }

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
    };

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}