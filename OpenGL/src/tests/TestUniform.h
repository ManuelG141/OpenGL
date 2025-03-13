#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace test {

	class TestUniform : public Test
	{
	public:
		TestUniform();
		~TestUniform();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		const float m_positions[8] = { // Positions of triangle vertices
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};
		const unsigned int m_indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		float m_Color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		float m_increment = 0.01f;
		bool m_SetColorManually = true;
		// I had to heap allocate this classes, so it's possible for me to control their constructing and deleting
		Renderer* m_renderer;
		VertexArray* m_va;
		VertexBuffer* m_vb;
		VertexBufferLayout* m_layout;
		IndexBuffer* m_ib;
		Shader* m_shader;
	};
}