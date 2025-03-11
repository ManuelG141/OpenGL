#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	VertexBufferElement(const unsigned int& t, const unsigned int& c, const bool& n)
		:type(t), count(c), normalized(n) {}

	static unsigned int GetSizeOfTyte(const unsigned int& type)
	{
		switch (type)
		{
			case GL_FLOAT:			return sizeof(GLfloat); // 4 Bytes
			case GL_UNSIGNED_INT:	return sizeof(GLuint);	// 4 Bytes
			case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);	// 1 Byte
		}
		ASSERT(true); // If the type doesn't match the ones that we support, then trigger an error
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) { };

	template<typename T>
	void Push(const unsigned int& count) // if you try to instantiate with a unsupported type
	{
		static_assert(false); //This will throw an exception
	}

	template<> // Template Specialization, the same template but with a different behavior
	void Push<float> (const unsigned int& count)
	{
		m_Elements.emplace_back(VertexBufferElement{ GL_FLOAT,count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfTyte(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(const unsigned int& count)
	{
		m_Elements.emplace_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfTyte(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(const unsigned int& count) // Unsigned char = byte
	{
		m_Elements.emplace_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfTyte(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; } // To avoid copy, we return const& here
	inline unsigned int GetStride() const& { return m_Stride; }
};