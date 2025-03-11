#include <GL/glew.h> // You have to include this before any other OpenGL include or it will trow a error
#include <GLFW/glfw3.h>
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (x) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// GLFW Documentation: https://www.glfw.org/documentation.html
// GLEW Documentation: https://glew.sourceforge.net/basic.html
// OpenGL Documentation: https://docs.gl/

// The cherno video tutorial: https://www.youtube.com/watch?v=71BLZwRGUJE

static void GLClearError()
{
    while (glGetError()); // When there's no more errors, glGetError returns a GL_NO_ERROR flag that's equal to 0
}

static bool GLLogCall(const char* function, const char* file, const int& line)
{
    bool anyErrors = false;
    while (GLenum e = glGetError()) // This will loop until e = 0 or false
    {
        std::cout << "[OpenGL Error] (" << e << ") " << function <<
            " " << file << ":" << line << std::endl;
        anyErrors = true;
    }
    return anyErrors;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
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

//Functions to create and compile our shaders
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // This converts from std::string type to a raw string
    glShaderSource(id, 1, &src, nullptr); // id of the shader, count of programs, pointer to the string (doble pointer), lenght of the string (If length is NULL, each string is assumed to be null terminated.)
    glCompileShader(id);

    // TODO: Syntax checking, Error Handling
    int shaderCompiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE)
    {
        int length; //GLsizei
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Get the lenght of our log string

        char* message = (char*)alloca(length * sizeof(char)); // Creating a char array that allocates with variable size on the heap
        glGetShaderInfoLog(id, length, &length, message); // Saving the message into the "message" string variable
        std::cout << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER DIDN'T COMPILE!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id); // Delete the shader if it doesn't compile
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) // 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); // Get's called for each vertex in our screen
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // Tells each pixel what color it should be

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);


    glDetachShader(program, vs); // Detach the shader from program object
    glDetachShader(program, fs);

    glDeleteShader(vs); // Free the memory that contains the shader
    glDeleteShader(fs);

    return program;
}

static void updateColors(float& r, float& g, float& b, const float& increment)
{
    static int count = 0;
    if (count == 0)
    {
        r += increment;
        b -= increment;
    }
    else if (count == 1)
    {
        g += increment;
        r -= increment;
    }
    else if (count == 2)
    {
        b += increment;
        g -= increment;
    }

    if (r > 1.0 || g > 1.0)
        count++;
    else if (b > 1.0)
        count = 0;

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Simple Square", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // To activate frame rate

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << "GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl; // Currently from Intel Integrated GPU

    float positions[] = { // Since these are vertices
        -0.8f, -0.8f, // first element, index 0
          0.8f, -0.8f, // index 1
          0.8f,  0.8f, // index 2
         -0.8f,  0.8f, // index 3
    };

    // Index buffer
    unsigned int indices[] = {
        0, 1, 2, // Use this indices to draw the first triangle
        2, 3, 0  // Use this indices to draw the second triangle
    };

    // Creating a Vertex Array Object
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // Creating the Buffer that Stores the data
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    // Specify the layer of the buffer
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); 

    // Creating index buffer
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // Writing the shaders
    ShaderProgramSource source = ParseShader("../../Basic.shader"); // The struct contains the source code of the fragment and vertex shaders
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    // Retreive the location of the uniform variable
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));

    ASSERT(location == -1); //Uniform variable wasn't found

    float r = 0.0f;
    float b = 1.0f;
    float g = 0.0f;
    float increment = 0.05f;

    // Unbind everithing to set up like we have multiple objects later
    GLCall(glBindVertexArray(0)); // Vertex Array
    GLCall(glUseProgram(0)); // Program
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Vertex Buffer
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // Index Buffer

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // This is all we need to draw a object into the screen
        GLCall(glUseProgram(shader)); // Bind the program and set the uniform
        GLCall(glUniform4f(location, r, g, b, 1.0f));

        GLCall(glBindVertexArray(vao)); // Just bind Vertex Array, that contains the Vertex Buffer, it's layout, and the index buffer object
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        updateColors(r, g, b, increment);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}