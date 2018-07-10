#include "Window.h"

Window::Window(int width, int height, const char * title)
	:window(glfwCreateWindow(width,height,title,nullptr,nullptr))
	, scale(100.0f), location{0,0},keyStatus(GLFW_RELEASE)
{
	if (window == nullptr) {
		//couldn't create window
		std::cerr << "Can't creat GLFW window" << std::endl;
		exit(1);
	}
	
	//target current window for process
	glfwMakeContextCurrent(window);

	//GLEW init
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//failed in intializing glew
		std::cerr << "Can't intialize GLEW" << std::endl;
		exit(1);
	}
	//wait V sync
	glfwSwapInterval(1);
	

	//record this pointer
	glfwSetWindowUserPointer(window, this);

	//register callback-function when window resized
	glfwSetWindowSizeCallback(window, resize);

	//register process when mouse wheel is operated
	glfwSetScrollCallback(window, wheel);

	//register process when keyboard is operated
	glfwSetKeyCallback(window, keyboard);
	


	//initialize opened window
	resize(window, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

int Window::shouldClose() const
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}
//swap color-buffers and get events 
void Window::swapBuffers()
{
	glfwSwapBuffers(window);

	//glfwPollEvents();
	
	//if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
	//	location[0] -= 2.0f / size[0];
	//else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
	//	location[0] += 2.0f / size[0];
	//if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
	//	location[1] -= 2.0f / size[1];
	//else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
	//	location[1] += 2.0f / size[1];
	//double x, y;
	//glfwGetCursorPos(window, &x, &y);
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE) 
	//{
	//	location[0] = static_cast<GLfloat>(x)*2.0f / size[0] - 1.0f;
	//	location[1] = 1.0f - static_cast<GLfloat>(y)*2.0f / size[1];
	//}

}
void Window::pollEvents()
{
	glfwPollEvents();
}
bool Window::keyOn(int glfw_keyID)const
{
	return glfwGetKey(window, glfw_keyID) != GLFW_RELEASE;
}
bool Window::keyTriggered(int glfw_keyID)const
{
	return glfwGetKey(window, glfw_keyID) == GLFW_PRESS;
}
void Window::resize(GLFWwindow * const window, int width, int height)
{
	glViewport(0, 0, width, height);

	//get this pointer
	Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
	
	if (instance != nullptr)
	{
		instance->size[0] = static_cast<GLfloat>(width);
		instance->size[1] = static_cast<GLfloat>(height);
	}
}

void Window::wheel(GLFWwindow * window, double x, double y)
{
	//get this pointer
	Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

	if (instance != nullptr)
	{
		instance->scale += static_cast<GLfloat>(y);
	}
}

void Window::keyboard(GLFWwindow * const window, int key, int scancode, int action, int mods)
{
	Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
	if (instance != nullptr)
	{
		instance->keyStatus = action;
	}
}
