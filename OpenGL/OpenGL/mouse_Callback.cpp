#include"VBO.h"
#include"shaderClass.h"
#include"bresenham.h"
#include "VAO.h"
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
// stores the points of the mouse (-1 is a placeholder)
 float g_mouseX1 = -1, g_mouseY1 = -1;
 float g_mouseX2 = -1, g_mouseY2 = -1;
 bool  g_waitingSecondClick = false;  // false = waiting first click, true = waiting second click
 bool  g_lineReady = false;           // true when we already have the two points

 // GLFW give us the position of the mouse in pixels, but we need to convert it to OpenGL coordinates
 
 void  mouse_Callback(GLFWwindow* window, int button, int action, int mods) {
     //the method only excecutes if we're cliking with left button and ignores all buttons left
	
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        // Obtain current cursor position in pixels
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Obtain current window size
        //who help us to convert the mouse position to openGL coordinates

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        if (!g_waitingSecondClick) {
			// first click, we store the first point and wait for the second click
            g_mouseX1 = (float)mouseX;
            //-GLFW origin is in the top left corner, but 
            // OpenGL origin is in the bottom left corner, so we need to invert the Y coordinate
            g_mouseY1 = (float)(height - mouseY); 
			//we wait for the second click
            g_waitingSecondClick = true;
			// we are not ready to draw the line yet, we need the second point
            g_lineReady = false;
        }
        else {
			// second click, we store the second point and we are ready to draw the line
            g_mouseX2 = (float)mouseX;
            g_mouseY2 = (float)(height - mouseY);
            g_waitingSecondClick = false;
            g_lineReady = true;
        }
    }
}