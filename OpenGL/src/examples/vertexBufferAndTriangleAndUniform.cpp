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

static bool GLLogCall(const char* function, const char* file,const int& line)
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

    //glewInit(); // Glew cannot be initialized here, it needs an appropiate context before calling it

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

    float positions[] ={ // Since these are vertices
        - 0.8f, -0.8f, // first element, index 0
          0.8f, -0.8f, // index 1
          0.8f,  0.8f, // index 2
         -0.8f,  0.8f, // index 3
    };

    // Index buffer
    unsigned int indices[] = {
        0, 1, 2, // Use this indices to draw the first triangle
        2, 3, 0  // Use this indices to draw the second triangle
    };

    // Creating the Buffer that Stores the data
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer)); // This will write the information in the memory address of the "buffer" variable
    // in this example "buffer" it's the ID of the buffer that we have, when we want to use our buffer we can just call the "buffer" variable, just like this
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // This will select the buffer that we want to use, GL_ARRAY_BUFFER just tells that this is an array
    // This is the data that will be called if need it later

    // The size here is in bytes
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));


    // Specify the layer of the buffer
    GLCall(glEnableVertexAttribArray(0)); // Index of the vertex that we want to enable 
    // glVertexAttribPointer( index first, count elements that has our attrib, type of data, if we want to normalize it "true or false", bytes of vertex "it has two floats inside so it's 8 bytes", offset of the first attribute in the array "bytes");
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)); // We should call this function for all the attributes that our vertex has inside

    // Creating index buffer
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // Writing the shaders
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); // The struct contains the source code of the fragment and vertex shaders
    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(program));

    // Retreive the location of the uniform variable
    GLCall(int location = glGetUniformLocation(program, "u_Color"));

    ASSERT(location == -1); //Uniform variable wasn't found

    // This is how we update a uniform variable of our shader
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float b = 1.0f;
    float g = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Without index buffer
        //glDrawArrays(GL_TRIANGLES, 0, 6); // Specify the element that OpenGL will draw, the first index and the number of elements to be rendered, three in total

        // Updating the reactangle with uniforms
        GLCall(glUniform4f(location, r, g, b, 1.0f)); // Uniforms are call per draw
        //With index buffer
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // The uniform will apply to all elements in this draw

        updateColors(r, g, b, increment);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(program));

    glfwTerminate();
    return 0;
}