#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"

// consts used

// settings

const unsigned int _WIDTH = 800;
const unsigned int _HEIGHT = 600;

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
	GLFWwindow* w = glfwCreateWindow(_WIDTH, _HEIGHT, "dank5 Engine", NULL, NULL);
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

	// create shader object
	Shader shader("testVert.vs", "testFrag.fs");

	// setup vertex data and buffer objects
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// color data starts after each position so offset is every 3 * float (xyz)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind vbo but not VAO or EBO - never unbind EBO before VAO (only unbind if necessary)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//unbind VAO
	glBindVertexArray(0);

	// generate opengl texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load a texture using the stb_image program
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// if found the texture data
	if (data)
	{
		// generate texture using the loaded texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// free the image memory
	stbi_image_free(data);


	shader.use();
	shader.setInt("texture1", 0);

	// tell GLFW to call framebuffer size callback on resize
	glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);

	// render loop
	while (!glfwWindowShouldClose(w)) {
		// manage input
		processInput(w);
		
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*        
		// bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
		
		*/

		// bind texture on texture units
		glBindTexture(GL_TEXTURE_2D, texture);

		//2d rectangle made of two triangles
		shader.use();

		// perspective projection
		// glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); //orthographic

		// create a model matrix 
		glm::mat4 model = glm::mat4(1.0f);
		// rotate on x-axis so it appears to be on the floor
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// move backwards
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		// create perspective
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)_WIDTH / (float)_HEIGHT, 0.1f, 100.0f); // perspective


		// use shader functions to set uniforms
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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