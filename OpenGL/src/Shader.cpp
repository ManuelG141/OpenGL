#include "Shader.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath); // The struct contains the source code of the fragment and vertex shaders
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath); //Opens the file in that filepath

	std::string line;
	std::stringstream ss[2]; // One to storage the vertex source code, and the other for the fragment shader
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) // While there's more lines to read in the file
	{
		if (line.find("#shader") != std::string::npos) // npos refers to invalid position, line.find() just returns the position of the string that we specify
		{
			if (line.find("vertex") != std::string::npos)
				// Set to vertex mode
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				// Set to fragment mode
				type = ShaderType::FRAGMENT;
		}
		else // If the line doesn't have shader just push the data into the stream with the current mode
		{
			ss[(int)type] << line << "\n"; // Push the line of code and add a new line char
		}
	}
	stream.close();
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str(); // This converts from std::string type to a raw string
	GLCall(glShaderSource(id, 1, &src, nullptr)); // id of the shader, count of programs, pointer to the string (doble pointer), lenght of the string (If length is NULL, each string is assumed to be null terminated.)
	GLCall(glCompileShader(id));

	// TODO: Syntax checking, Error Handling
	int shaderCompiled;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled));
	if (shaderCompiled != GL_TRUE)
	{
		int length; //GLsizei
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // Get the lenght of our log string

		char* message = (char*)alloca(length * sizeof(char)); // Creating a char array that allocates with variable size on the heap
		GLCall(glGetShaderInfoLog(id, length, &length, message)); // Saving the message into the "message" string variable
		std::cout << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER DIDN'T COMPILE!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id)); // Delete the shader if it doesn't compile
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) // 
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));


	GLCall(glDetachShader(program, vs)); // Detach the shader from program object
	GLCall(glDetachShader(program, fs));

	GLCall(glDeleteShader(vs)); // Free the memory that contains the shader
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, const int& value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, const float& value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, const float& v0, const float& v1, const float& v2, const float& v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) // If the location is inside the map, we just return it
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) //Uniform variable wasn't found
		std::cout << "Warning: uniform '" << name << "' doesn't exits!" << std::endl;

	m_UniformLocationCache[name] = location; // If the location wasn't found we save it, so we doesn't have to search for it the next time
	return location;
}

