#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

namespace test {
	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		const float m_positions[16] = { // Positions of triangle vertices
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};
		const unsigned int m_indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// I had to heap allocate this classes, so it's possible for me to control their constructing and deleting
		Renderer* m_renderer;
		VertexArray* m_va;
		VertexBuffer* m_vb;
		VertexBufferLayout* m_layout;
		IndexBuffer* m_ib;
		Shader* m_shader;
		Texture* m_texture;
	};
}