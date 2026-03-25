#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include "../OpenGL/shaderClass.h"
#include "../OpenGL/VAO.h"
#include "../OpenGL/VBO.h"

#include "../OpenGL/bresenham.h"


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create window
	GLFWwindow* window = glfwCreateWindow(800, 800, "Bresenham_Curve", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// initialize GLAD after making context current
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//intialize glad so openGL configurates it
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, 800, 800);
	//1.generate the shader object
	Shader shaderProgram("default.vert", "default.frag");
	
	//parameters of out bresenham line
	auto pts = bresenhamLine(100.0, 100.0, 900.0, 700.0, 1000, 1000);

	//4.store the data on the VAO and VBO
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();   

	VBO VBO1((GLfloat*)pts.data(), (GLsizeiptr)(pts.size() * sizeof(GLfloat)));
	VAO1.LinkVBO(VBO1, 0);

	VAO1.Unbind();
	VBO1.Unbind();


	glEnable(GL_PROGRAM_POINT_SIZE);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		VAO1.Bind();

		glDrawArrays(GL_POINTS, 0, pts.size() / 2); //draws the points

		glfwSwapBuffers(window);
	}

	// 5. clean
	VAO1.Delete();
	VBO1.Delete();
	shaderProgram.Delete();


	glfwTerminate();
	return 0;



}