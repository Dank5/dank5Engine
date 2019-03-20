#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

class Window {

public:

	Window(int width, int height, const std::string &title);

	void Hint(int target, int hint);
	void setInputMode(int mode, int value);
	void setCursorPosCallback(CursorPositionFunction cpf);

private:

	GLFWwindow* _window;
	bool _destroy;



};
#endif