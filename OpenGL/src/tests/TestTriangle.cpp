#include "TestTriangle.h"

test::TestTriangle::TestTriangle()
{
	m_va = new VertexArray();
	m_vb = new VertexBuffer(m_positions, 2 * 3 * sizeof(float));
	m_layout = new VertexBufferLayout();
	m_layout->Push<float>(2);
	m_va->AddBuffer(*m_vb, *m_layout);
	m_shader = new Shader("res/shaders/BasicTriangleShader.shader");
	m_shader->Bind();
}

test::TestTriangle::~TestTriangle()
{
	delete m_va;
	delete m_vb;
	delete m_layout;
	delete m_shader;
}

void test::TestTriangle::OnUpdate(float deltaTime)
{

}

void test::TestTriangle::OnRender()
{
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void test::TestTriangle::OnImGuiRender()
{
}

