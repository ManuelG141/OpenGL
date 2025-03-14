#include "TestUniform.h"

#include <chrono>

static void updateColors(float m_Color[3], std::atomic_bool& m_SetColorManually, const float m_increment, const int delayTime)
{
	int index = 0;
	while (!m_SetColorManually)
	{
		if (index == 0)
		{
			m_Color[0] += m_increment;
			m_Color[2] -= m_increment;
		}
		else if (index == 1)
		{
			m_Color[1] += m_increment;
			m_Color[0] -= m_increment;
		}
		else
		{
			m_Color[2] += m_increment;
			m_Color[1] -= m_increment;
		}
		if (m_Color[0] > 1.0f || m_Color[1] > 1.0f)
		{
			index++;
		}
		else if (m_Color[2] > 1.0f)
		{
			index = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
	}
}

namespace test {
	TestUniform::TestUniform()
	{
		m_renderer = new Renderer();
		m_va = new VertexArray();
		m_vb = new VertexBuffer(m_positions, 2 * 4 * sizeof(float));
		m_layout = new VertexBufferLayout();
		m_layout->Push<float>(2);
		m_va->AddBuffer(*m_vb, *m_layout);
		m_ib = new IndexBuffer(m_indices, 6);
		m_shader = new Shader("res/shaders/BasicUniform.shader");
		m_shader->Bind();
	}

	TestUniform::~TestUniform()
	{
		delete m_renderer;
		delete m_va;
		delete m_layout;
		delete m_ib;
		delete m_shader;

		if (m_UpdateColorThread.joinable()) // If the thread is still running then destroy it
		{
			m_SetColorManually = true; // Finish the while loop inside the thread
			m_UpdateColorThread.join(); // Wait until the execution of the thread finish
		}
	}

	void TestUniform::OnUpdate(float deltaTime)
	{

	}

	void TestUniform::OnRender()
	{
		m_shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
		m_renderer->Draw(*m_va, *m_ib, *m_shader);
	}

	void TestUniform::OnImGuiRender()
	{
		ImGui::ColorEdit4("Square Color", m_Color);
		if (ImGui::Button(m_SetColorManually ? "RGB" : "Manually"))
		{
			m_SetColorManually = !m_SetColorManually;
			if (!m_SetColorManually)
			{
				m_Color[0] = 0.0f;
				m_Color[1] = 0.0f;
				m_Color[2] = 1.0f;
				m_UpdateColorThread = std::thread{ updateColors, m_Color, std::ref(m_SetColorManually), m_increment, m_DelayTime };
			}
			else
				m_UpdateColorThread.join();
		}
	}
}