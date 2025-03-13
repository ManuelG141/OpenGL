#include "TestUniform.h"

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
	}

	void TestUniform::OnUpdate(float deltaTime)
	{
		if (!m_SetColorManually)
		{
			static int index = 0;
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
			
		}
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
			if (m_SetColorManually)
			{
				m_Color[0] = 0.0f;
				m_Color[1] = 0.0f;
				m_Color[2] = 1.0f;
			}
			m_SetColorManually = !m_SetColorManually;
		}
	}
}