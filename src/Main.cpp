#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

// Include Classes
#include "Shaders/Shader.h"
#include "OpenGL/VAO.h"
#include "OpenGL/VBO.h"
#include "OpenGL/EBO.h"
#include <stb/stb_image.h>
#include "Shaders/Texture.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void processInput(GLFWwindow* window);

bool fileExists(const std::string& filename) {
	std::ifstream file(filename);
	return file.good();
}

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};


int main() {


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Engine", nullptr, nullptr);

	if (window == NULL) {
		std::cout << "Failed to init window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	Shader shaderProgram("src/Shaders/Default.vert", "src/Shaders/Default.frag");

	VAO vao1;
	vao1.Bind();


	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));


	vao1.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vao1.Unbind();
	ebo.Unbind();
	vbo.Unbind();

	GLint uniId = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture grassBlock("src/wall.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	grassBlock.texUnit(shaderProgram, "tex0", 0);



	while (!glfwWindowShouldClose(window)) {


		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		grassBlock.Bind();


		glUniform1f(uniId, 0.5f);

		vao1.Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		processInput(window);

		glfwPollEvents();
	}



	vao1.Delete();
	vbo.Delete();
	ebo.Delete();
	grassBlock.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		std::cout << "Enter Pressed" << std::endl;
}