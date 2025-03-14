#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	class TestMultipleObjects : public Test
	{
	public:
		TestMultipleObjects();
		~TestMultipleObjects();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		const float m_positions[16] = { // Positions of triangle vertices
			-100.f, -100.f, 0.0f, 0.0f,
			 100.f, -100.f, 1.0f, 0.0f,
			 100.f,  100.f, 1.0f, 1.0f,
			-100.f,  100.f, 0.0f, 1.0f
		};
		const unsigned int m_indices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		// Model, View, Projection Matrix
		const glm::mat4 m_proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		const glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::vec3 m_modelA_transform = glm::vec3(280.0f, 270.0f, 0.0f);
		glm::vec3 m_modelB_transform = glm::vec3(680.0f, 270.0f, 0.0f);
		glm::mat4 m_MVP;
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