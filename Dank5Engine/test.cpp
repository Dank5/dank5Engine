#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// consts used

// settings

const unsigned int _WIDTH = 800;
const unsigned int _HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// function to process input, queries whether a key has been pressed and performs action accordingly
void processInput(GLFWwindow* w) {
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(w, true);
}
// function to change rendering window size on GLFW window resize
void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {

	/* WINDOW CREATION START */
	// Initilise glfw
	glfwInit();
	// Tell GLFW which version to use and which Opengl profile we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a new window
	GLFWwindow* w = glfwCreateWindow(_WIDTH, _HEIGHT, "Zombie Shooter", NULL, NULL);
	//check its not null (Remember this is good practice for all dynamically allocated memory
	//consider using nullptr instead
	if (w == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// make the window the context of the current thread
	glfwMakeContextCurrent(w);
	// initilise GLAD (handles function pointers for OpenGL)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initilise GLAD" << std::endl;
		return -1;
	}

	// tell opengl size of rendering window (lower left, lower left, width, height)
	glViewport(0, 0, 800, 600);

	/* Shader code */

	// create vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLIATION FAILED\n" << infoLog << std::endl;
	}

	// create fragment shader object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for shader error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLIATION FAILED\n" << infoLog << std::endl;
	}

	//link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check for linking error
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// setup vertex data and buffer objects
	float vertices[] = {
		0.5f, 0.5f, 0.0f,		// top right
		0.5f, -0.5f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		// bottom left
		-0.5f, 0.5f, 0.0f		// top left
	};
	unsigned int indices[] = {
		0,1,3, //first triangle
		1,2,3  //second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	//bind array object
	glBindVertexArray(VAO);
	//bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//set vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//configure vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//unbind vbo but not VAO or EBO - never unbind EBO before VAO (only unbind if necessary)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//unbind VAO
	glBindVertexArray(0);




	// tell GLFW to call framebuffer size callback on resize
	glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);

	// render loop
	while (!glfwWindowShouldClose(w)) {
		// manage input
		processInput(w);
		
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 

		// swap the buffers and poll for input using glfw
		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// terminate program
	glfwTerminate();
	return 0;

}

