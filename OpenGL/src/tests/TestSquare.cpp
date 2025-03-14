#include "TestSquare.h"

test::TestSquare::TestSquare()
{
	m_renderer = new Renderer();
	m_va = new VertexArray();
	m_vb = new VertexBuffer(m_positions, 2 * 4 * sizeof(float));
	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(2);
	m_va->AddBuffer(*m_vb, *m_layout);
	m_ib = new IndexBuffer(m_indices, 6);
	m_shader = new Shader("res/shaders/BasicTriangleShader.shader");
	m_shader->Bind();
}

test::TestSquare::~TestSquare()
{
	delete m_renderer;
	delete m_va;
	delete m_vb;
	delete m_layout;
	delete m_ib;
	delete m_shader;
}

void test::TestSquare::OnUpdate(float deltaTime)
{

}

void test::TestSquare::OnRender()
{
	m_renderer->Draw(*m_va, *m_ib, *m_shader);
}

void test::TestSquare::OnImGuiRender()
{
	
}
