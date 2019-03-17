#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	// tell GLFW to call framebuffer size callback on resize
	glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);

	// render loop
	while (!glfwWindowShouldClose(w)) {
		// manage input
		processInput(w);
		
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// swap the buffers and poll for input using glfw
		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	// terminate program
	glfwTerminate();
	return 0;

}

