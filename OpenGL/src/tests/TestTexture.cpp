#include "TestTexture.h"

namespace test {

	TestTexture::TestTexture()
	{
		m_renderer = new Renderer();
		m_va = new VertexArray();
		m_vb = new VertexBuffer(m_positions, 4 * 4 * sizeof(float));
		m_layout = new VertexBufferLayout();
		m_layout->Push<float>(2); //Rectangle
		m_layout->Push<float>(2); //Texture Coordinates
		m_va->AddBuffer(*m_vb, *m_layout);
		m_ib = new IndexBuffer(m_indices, 6);
		m_shader = new Shader("res/shaders/BasicTexture.shader");
		m_shader->Bind();
		m_texture = new Texture("res/textures/thanos_patrick.png");
		m_texture->Bind();
		m_shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture::~TestTexture()
	{
		delete m_renderer;
		delete m_va;
		delete m_layout;
		delete m_ib;
		delete m_shader;
		delete m_texture;
	}

	void TestTexture::OnUpdate(float deltaTime)
	{

	}

	void TestTexture::OnRender()
	{
		m_renderer->Draw(*m_va, *m_ib, *m_shader);
	}

	void TestTexture::OnImGuiRender()
	{

	}

}