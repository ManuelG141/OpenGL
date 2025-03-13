#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
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
		const float m_positions[6] = { // Positions of triangle vertices
			-0.5f, -0.5f,
			 0.0f,  0.5f,
			 0.5f, -0.5f
		};
		VertexArray* m_va;
		VertexBuffer* m_vb;
		VertexBufferLayout* m_layout;
		Shader* m_shader;
	};
}