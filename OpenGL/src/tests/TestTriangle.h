#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace test {
	class TestTriangle: public Test
	{
	public:
		TestTriangle();
		~TestTriangle();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		float m_positions[6] = { // Positions of triangle vertices
			-0.5f, -0.5f,
			 0.0f,  0.5f,
			 0.5f, -0.5f
		};
		VertexArray m_va;
		VertexBuffer m_vb = {m_positions, 2*3*sizeof(float)};
		VertexBufferLayout m_layout;
		Shader m_shader = { "res/shaders/BasicTriangleShader.shader" };
	};

}