#pragma once
#include <vector>
#include <GLFW/glfw3.h>


extern float g_mouseX1, g_mouseY1;
extern float g_mouseX2, g_mouseY2;
extern bool  g_waitingSecondClick;
extern bool  g_lineReady;

void mouse_Callback(GLFWwindow* window, int button, int action, int mods);

