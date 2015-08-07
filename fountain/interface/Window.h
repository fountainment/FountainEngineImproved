#if !defined(_FEI_WINDOW_H_)
#define _FEI_WINDOW_H_

#include "../base/basedef.h"
#include "../base/ModuleBase.h"
#include <GLFW/glfw3.h>

namespace fei {

class Window
{
public:
	Window();
	~Window();

	void setSize(int w, int h);
	void setTitle(std::string tt);
	void setResizable(bool resizable);
	void setFullscreen(bool fullscreen);

	GLFWwindow* getWindow(); 
	void delWindow();

	void setCurrent();

	void hide();
	void show();

	bool shouldClose();
	void swapBuffers();
	void pollEvents();

	bool isFullscreen();

	int getKey(int key);
private:
	GLFWwindow *window;
	int width, height;
	std::string title;
	bool _isFullscreen;
};

}

#endif
