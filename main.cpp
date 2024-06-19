
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string &source)
{

    unsigned int id = glCreateShader(type);

    // This is a pointer to the very beginning of our source
    const char *src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);

    int result;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *msg = (char *)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, msg);

        std::cout << msg << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
};

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Check the version of the OpenGL
    // std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6]{
        0.0f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
    };

    unsigned int myBuffer;
    glGenBuffers(1, &myBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, myBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        " gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        " color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw Triangle
        // glBegin(GL_TRIANGLES);
        // glVertex2f(0.0f, 0.5f);
        // glVertex2f(-0.5f, -0.5f);
        // glVertex2f(0.5f, -0.5f);
        // glEnd();

        // Draw the triangle using buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}