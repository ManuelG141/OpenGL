#include "TestMultipleObjects.h"

namespace test {

	TestMultipleObjects::TestMultipleObjects()
	{
		m_renderer = new Renderer();
		m_va = new VertexArray();
		m_vb = new VertexBuffer(m_positions, 4 * 4 * sizeof(float));
		m_layout = new VertexBufferLayout();
		m_layout->Push<float>(2); //Rectangle
		m_layout->Push<float>(2); //Texture Coordinates
		m_va->AddBuffer(*m_vb, *m_layout);
		m_ib = new IndexBuffer(m_indices, 6);
		m_shader = new Shader("res/shaders/MVP.shader"); // The same shader as before
		m_shader->Bind();
		m_texture = new Texture("res/textures/thanos_patrick.png");
		m_texture->Bind();
		m_shader->SetUniform1i("u_Texture", 0);
	}

	TestMultipleObjects::~TestMultipleObjects()
	{
		delete m_renderer;
		delete m_va;
		delete m_vb;
		delete m_layout;
		delete m_ib;
		delete m_shader;
		delete m_texture;
	}

	void TestMultipleObjects::OnUpdate(float deltaTime)
	{

	}

	void TestMultipleObjects::OnRender()
	{
		m_MVP = m_proj * m_view * glm::translate(glm::mat4(1.0f), m_modelA_transform);
		m_shader->SetUniformMat4f("u_MVP", m_MVP);
		m_renderer->Draw(*m_va, *m_ib, *m_shader);

		m_MVP = m_proj * m_view * glm::translate(glm::mat4(1.0f), m_modelB_transform);
		m_shader->SetUniformMat4f("u_MVP", m_MVP);
		m_renderer->Draw(*m_va, *m_ib, *m_shader);
	}

	void TestMultipleObjects::OnImGuiRender()
	{
		ImGui::SliderFloat3("Model A Transform", &m_modelA_transform.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Model B Transform", &m_modelB_transform.x, 0.0f, 960.0f);
		if (ImGui::Button("Reset Model Transforms"))
		{
			m_modelA_transform.x = 280.f;
			m_modelA_transform.y = 270.f;
			m_modelA_transform.z = 0.f;

			m_modelB_transform.x = 680.f;
			m_modelB_transform.y = 270.f;
			m_modelB_transform.z = 0.f;
		}
	}

}