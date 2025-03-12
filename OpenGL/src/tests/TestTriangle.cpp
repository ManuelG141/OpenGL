#include "TestTriangle.h"

test::TestTriangle::TestTriangle()
{
	m_layout.Push<float>(2);
	m_va.AddBuffer(m_vb, m_layout);
	m_shader.Bind();
}

test::TestTriangle::~TestTriangle()
{

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

