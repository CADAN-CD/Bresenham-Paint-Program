#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include "../OpenGL/shaderClass.h"
#include "../OpenGL/VAO.h"
#include "../OpenGL/VBO.h"
#include "../OpenGL/mouse_Callback.h"

#include "../OpenGL/bresenham.h"
#include "../OpenGL/CircleMidPoint.h"

#include "../OpenGL/ImGui/imgui.h"
#include "../OpenGL/ImGui/backends/imgui_impl_glfw.h"
#include "../OpenGL/ImGui/backends/imgui_impl_opengl3.h"
using namespace std;

int main()
{


	bool open = true;

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


	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, 800, 800);
	//1.generate the shader object
	Shader shaderProgram("default.vert", "default.frag");

	//every time that the user clicks the mouse, the function mouse_Callback will be called
	glfwSetMouseButtonCallback(window, mouse_Callback);

	//it controls the size of the points in the shader
	glEnable(GL_PROGRAM_POINT_SIZE);


	// VAO/VBO
	GLuint VAO1, VBO1;
	//we generate the buffers

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	//we link the VAO and VBO, and we configure the vertex attributes
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	// attribute 0: the position (x, y)
	//the stride changes to 5 * sizeof(float) because we have 5 floats for each vertex (x, y, r, g, b)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//we activate the attribute
	glEnableVertexAttribArray(0);
	
	//atribute 1 : the color (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//atribute 2 : the size of the point
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind
	glBindVertexArray(0);
	//counter of points to draw
	GLsizei linePointCount = 0;

	// we acumulate all the point lines in a single vector, so we can upload it to the GPU in one call
	std::vector<float> allPts;

	
	// we bind the VAO 
	glBindVertexArray(VAO1);
	//we bind the buffer for the VAO, who is going to tell how to interpret the data
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	// we upload the data of the CPU to the GPU
	glBufferData(GL_ARRAY_BUFFER, allPts.size() * sizeof(float), allPts.data(), GL_DYNAMIC_DRAW);
	//unbind for safety:) bc we already save the configurations
	glBindVertexArray(0);
	//we divide in two because we're using a pair of coordinates
	linePointCount = (GLsizei)(allPts.size() / 5);

	//ImGui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//ImGui variables to control the state of the application
	static bool  drawLineMode = false;
	static bool drawCircleMode = false;

		//variables to control the state of the line and circle drawing modes and global variables related to the mouse position and the viewport position
	static bool  waitingSecondClick = false;
	static bool  waitingCircleRadius = false;
		//variables to store the center of the circle and the two points of the line
	static float circleCx, circleCy;
	static float mouseX1, mouseY1;
	static float mouseX2, mouseY2;
	ImVec2       viewportPos;
		//ImGui color propierties for the fragment shader
	//initial color white 
	static float drawColor[3] = { 1.0f, 1.0f, 1.0f };
	//initial size of the points
	static float pointSize = 2.0f;
	// we enable the GL_PROGRAM_POINT_SIZE to be able to change
	// the point size in the shader with the glPointSize function
	glEnable(GL_PROGRAM_POINT_SIZE);
	// takes all the generated points and adds the actual color to the vector 
	auto insertWithColor = [&](std::vector<float>& shape) {
	
		for (int i = 0; i < shape.size(); i += 2) {
			allPts.push_back(shape[i]);       // x
			allPts.push_back(shape[i + 1]);   // y
			allPts.push_back(drawColor[0]);   // r
			allPts.push_back(drawColor[1]);   // g
			allPts.push_back(drawColor[2]);   // b
		}
		};
	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();

		//color of the window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//ImGui 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Bresenham Paint", &open,ImGuiWindowFlags_NoResize);
					//ImGui color editor 
		ImGui::ColorEdit3("Color", drawColor);
		shaderProgram.Activate();
					//ImGui point size slider
		ImGui::SliderFloat("Point Size", &pointSize, 1.0f, 10.0f);


		// searches the location of the variable uColor in the fragment shader and sets its value to 
		// the current color selected in the ImGui color editor
		//variable to store the location of the uniform variable uColor in the shader
		int colorLoc = glGetUniformLocation(shaderProgram.ID, "uColor");
		//the color is sent to the GPU as 3 floats (r, g, b) like { 1.0f, 1.0f, 1.0f };
		glUniform3f(colorLoc, drawColor[0], drawColor[1], drawColor[2]);

				// clear the canvas call
		if(ImGui::Button("Clear Canvas"))
		{
			allPts.clear();
			linePointCount = 0;
		}
			     // circle middle point call
		if(ImGui::Button("Draw Circle"))
		{
			drawCircleMode = true;
			drawLineMode = false;
			
		}
				//bresenham algorithm call
		if (ImGui::Button("Draw Line")) {
			drawLineMode = true;
			drawCircleMode = false;
		}
				

		//im Gui uses pixel coordinates begining in the top left corner and y increases downwards
		//while openGl uses normalized coordinates begining in the center of the screen and y increases upwards
		viewportPos = ImGui::GetCursorScreenPos();

		auto screenToPixel = [&](float px, float py, float& outX, float& outY) {
			//we subtract the viewport position to get the coordinates relative to the viewport
			outX = px - viewportPos.x;
			//we invert the y coordinate to match the openGl coordinate system
			outY = (viewportPos.y + h) - py; 
			};

		//line drawing mode function 
		//if the user is in the drawing line mode and clicks the canvas without touching the Imgui Interface 
		
		if (drawLineMode && !ImGui::GetIO().WantCaptureMouse && ImGui::IsMouseClicked(0)) {
			//then we prepare the first point coordinates of the line
			ImVec2 mousePos = ImGui::GetMousePos();
			//we check if the mouse click is outside the canvas, if it is we ignore it
			bool offCanvas =
				//right of the canvas
				mousePos.x < viewportPos.x ||
				//left of the canvas
				mousePos.x > viewportPos.x + w ||
				//below canvas
				mousePos.y < viewportPos.y ||
				//above canvas
				mousePos.y > viewportPos.y + h;
			//if the click is inside the canvas we process it
			if (!offCanvas) {
				
				if (!waitingSecondClick) {
					//we convert the screen coordinates to pixel coordinates relative to the viewport
					// and store them as the first point of the line
					screenToPixel(mousePos.x, mousePos.y, mouseX1, mouseY1);
					waitingSecondClick	 = true;
				}
				else {
					//we convert the screen coordinates to pixel coordinates relative to the viewport
					 // and store them as the second point of the line
					screenToPixel(mousePos.x, mousePos.y, mouseX2, mouseY2);

					//bresenham line callback
					std::vector<float> newLine = bresenhamLine(mouseX1, mouseY1, mouseX2, mouseY2, w, h);
					
					insertWithColor(newLine);
					

					glBindVertexArray(VAO1);
					glBindBuffer(GL_ARRAY_BUFFER, VBO1);
					glBufferData(GL_ARRAY_BUFFER, allPts.size() * sizeof(float), allPts.data(), GL_DYNAMIC_DRAW);
					glBindVertexArray(0);
					//we update the points to the VBO
					linePointCount = (GLsizei)(allPts.size() / 5);
					// we set waitingSecondClick to false to prepare for the next line while the 
					//button is still active
					waitingSecondClick = false;
					
				}
			}
		}
		//circle drawing mode function
		if (drawCircleMode && !ImGui::GetIO().WantCaptureMouse && ImGui::IsMouseClicked(0)) {

			ImVec2 mousePos = ImGui::GetMousePos();

			bool offCanvas = mousePos.x < viewportPos.x ||
				mousePos.x > viewportPos.x + w ||
				mousePos.y < viewportPos.y ||
				mousePos.y > viewportPos.y + h;
			if (!offCanvas) {
				//everytime we click we rewrite the variables px and py
				float px, py;
				//we convert the screen coordinates to pixel coordinates relative to the viewport
				screenToPixel(mousePos.x, mousePos.y, px, py);	
				if (!waitingCircleRadius) {
					//when they're stored,
					// we prepare the center of the circle and wait for the second click to determine the radius
					//the radius is goint to rewrite the px and py coordinates
					circleCx = px;
					circleCy = py;
					waitingCircleRadius = true;
				}
				else {
					//function to calculate the distance between 2 points, which
					// is the radius of the circle
					float radius = std::sqrt((px - circleCx) * (px - circleCx) + (py - circleCy) * (py - circleCy));

					//circle middle point callback
					std::vector<float> newCircle = CircleMidPoint(circleCx, circleCy, radius, w, h);
					// we insert the new circle points with the current color to the allPts vector
					insertWithColor(newCircle);
					



					glBindVertexArray(VAO1);
					glBindBuffer(GL_ARRAY_BUFFER, VBO1);
					glBufferData(GL_ARRAY_BUFFER, allPts.size() * sizeof(float), allPts.data(), GL_DYNAMIC_DRAW);
					glBindVertexArray(0);

					linePointCount = (GLsizei)(allPts.size() / 5);
					waitingCircleRadius = false;
				}
			}
		}
		
		ImGui::End();
		shaderProgram.Activate();
		//we call the function to set the point size in the shader, so we can change it dinamically with the ImGui slider
		glPointSize(pointSize);
		if (linePointCount > 0) {
			//activates the configuration of the vertex data
			glBindVertexArray(VAO1);
			/*GL_POINTS- draws the points
			0 from first point
				linePointCount how many points is going to draw
			*/
			//uses the VBO data to draw the points
			glDrawArrays(GL_POINTS, 0, linePointCount);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		
	}

	// 5. clean
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	shaderProgram.Delete();
	glfwTerminate();
	return 0;



}