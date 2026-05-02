#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include "../OpenGL/shaderClass.h"
#include "../OpenGL/VAO.h"
#include "../OpenGL/VBO.h"
#include "../OpenGL/mouse_Callback.h"

#include "../OpenGL/bresenham.h"
#include "../OpenGL/CircleMidPoint.h"
using namespace std;

int main()
{

	//1. initialize GLFW
	glfwInit();
	//2. tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//3. tell GLFW we are using the CORE profile, which means we only have the modern functions of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int w = 800;
	int h = 800;
	// create window
	GLFWwindow* window = glfwCreateWindow(w, h, "Bresenham_Curve", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make the context of our window the main context
	glfwMakeContextCurrent(window);
	// initialize GLAD after making context current
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD";
		return -1;
	}
	//intialize glad so openGL configurates it
	gladLoadGL();


	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, 800, 800);
	//1.generate the shader object
	Shader shaderProgram("default.vert", "default.frag");

	//every time that the user clicks the mouse, the function mouse_Callback will be called
	glfwSetMouseButtonCallback(window, mouse_Callback);

	//it controls the size of the points in the shader
	glEnable(GL_PROGRAM_POINT_SIZE);

	//auto pts = bresenhamLine(  400,  400, 20, 600, w, h);


	// VAO/VBO
	GLuint VAO1, VBO1;
	//we generate the buffers

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	//we link the VAO and VBO, and we configure the vertex attributes
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	// attribute 0: vec2 (x, y)
	//we have a pair of coordinates (x, y) for each point, so the size of each vertex is 2 * sizeof(float)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	//we activate the attribute
	glEnableVertexAttribArray(0);
	//unbind
	glBindVertexArray(0);
	//counter of points to draw
	GLsizei g_linePointCount = 0;

	// we acumulate all the point lines in a single vector, so we can upload it to the GPU in one call
	std::vector<float> allPts;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// new pair of points to draw a line
		if (g_lineReady) {
			//bresenham algorithm call
			// calculate the points of the line using Bresenham's algorithm
			std::vector<float> newLine = bresenhamLine(g_mouseX1, g_mouseY1,g_mouseX2, g_mouseY2,w, h);

			// we add the new line points to the accumulator of all points
			allPts.insert(allPts.end(), newLine.begin(), newLine.end());

			// activate the buffer for the VAO, who is going to tell how to interpret the data
			glBindVertexArray(VAO1);
			//this is the buffer that we are going to fill with the data of the points
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			// we upload the data of the CPU to the GPU,
			//size in bites
			//the pointer to the data
			// we use GL_DYNAMIC_DRAW because the data is going to change frequently, every time the user clicks
			glBufferData(GL_ARRAY_BUFFER,allPts.size() * sizeof(float),allPts.data(),GL_DYNAMIC_DRAW);
			//unbind for safety:) bc we already save the configurations 
			glBindVertexArray(0);
			//we divide in two because we're using a pair of coordinates
			g_linePointCount = (GLsizei)(allPts.size() / 2);
			// we are not ready to draw the line anymore, we need a new pair of points
			g_lineReady = false;
		}

		// we clear the points if the user presses the D key 
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			allPts.clear();
			g_linePointCount = 0;
		}

		//color of the window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		if (g_linePointCount > 0) {
			//activates the configuration of the vertex data
			glBindVertexArray(VAO1);
			/*GL_POINTS- draws the points
			0 from first point
				g_linePointCount how many points is going to draw
			*/
			//uses the VBO data to draw the points
			glDrawArrays(GL_POINTS, 0, g_linePointCount);
		}
		
		glfwSwapBuffers(window);
	}

	// 5. clean
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	shaderProgram.Delete();
	glfwTerminate();
	return 0;



}