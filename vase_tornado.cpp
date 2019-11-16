#include <iostream>
#include <stdlib.h>
#include <glew.h>
#include <glfw3.h>
#include "Shader.h"
#include "Tornado.h"
#include "glm/ext.hpp"
using namespace std;

GLFWwindow* window;
GLuint VertexArrayID;
Hierarchy* tornado, * tornado2;
int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 1000;

void mouseMoveEvent(GLFWwindow* window, double x, double y)
{
	double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
	double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
	printf("X : %f, Y : %f\n", mod_x, mod_y);
}
void mouseClickEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
		double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
		if (action == GLFW_PRESS)
		{
			printf("LEFT CLICK ON : (%lf, %lf)\n", mod_x, mod_y);
		}
		else if (action == GLFW_RELEASE)
		{
			printf("LEFT RELEASE ON : (%lf, %lf)\n", mod_x, mod_y);
		}
	}
}
void screenResizeEvent(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void initializeGLFW() {
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
}
void initializeWindow() {
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Computer Graphics", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseMoveEvent);
	glfwSetMouseButtonCallback(window, mouseClickEvent);
	glfwSetFramebufferSizeCallback(window, screenResizeEvent);

	glEnable(GL_DEPTH_TEST);
}
void initializeGLEW() {
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		return;
	}
}

void initializeShapes() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Vertex control_pts[8] =
	{
		Vertex(-0.02, 0.02),
		Vertex(-0.05, 0.15),
		Vertex(-0.15, 0.35),
		Vertex(-0.25, 0.55),
		Vertex(-0.35, 0.75),
		Vertex(-0.375, 0.79),
		Vertex(-0.4, 0.8),
		Vertex(-0.45, 0.9)
	};

	//coba = new Hierarchy(new Box1(-0.5, 0.5, 0));
	tornado = new Hierarchy(new Vase(control_pts, 8, -0.5, 0, 0, 25, 1));
	tornado2 = new Hierarchy(new Vase(control_pts, 8, 1, 0, 0, 25, 1));

	char vertexShader[][100] = { "shaders/Vertex.shader" };
	char fragmentShader[][100] = { "shaders/triangle/red.shader", "shaders/triangle/blue.shader", "shaders/triangle/yellow.shader", "shaders/triangle/brown.shader" };
	char fragmentOutlineShader[][100] = { "shaders/triangle/fragment_outline_1.shader" };

	//tornado->addChild(new Hierarchy(new Box1(tornado->getParent()->getPosition().x, tornado->getParent()->getPosition().y + 0.75, tornado->getParent()->getPosition().z)));
	tornado->addChild(new Hierarchy(new Box(tornado->getParent()->getPosition().x, tornado->getParent()->getPosition().y + 0.75, tornado->getParent()->getPosition().z)));
	tornado->getChild(0)->addChild(new Hierarchy(new Box(tornado->getParent()->getPosition().x, tornado->getParent()->getPosition().y + 0.8, tornado->getParent()->getPosition().z)));
	tornado->getChild(0)->getChild(0)->rotate(tornado->getChild(0)->getChild(0)->getParent()->getPosition(), tornado->getChild(0)->getChild(0)->getParent()->getEuler(2), 45);

	//tornado2->addChild(new Hierarchy(new Box1(tornado2->getParent()->getPosition().x, tornado2->getParent()->getPosition().y + 0.75, tornado2->getParent()->getPosition().z)));
	tornado2->addChild(new Hierarchy(new Box(tornado2->getParent()->getPosition().x, tornado->getParent()->getPosition().y + 0.75, tornado->getParent()->getPosition().z)));
	tornado2->getChild(0)->addChild(new Hierarchy(new Box(tornado2->getParent()->getPosition().x, tornado->getParent()->getPosition().y + 0.8, tornado->getParent()->getPosition().z)));
	tornado2->getChild(0)->getChild(0)->rotate(tornado2->getChild(0)->getChild(0)->getParent()->getPosition(), tornado2->getChild(0)->getChild(0)->getParent()->getEuler(2), 45);

	//coba->initiateBuffer();
	//coba->initiateShader(vertexShader[0], fragmentShader[0]);
	//coba->initiateOutlineShader(vertexShader[0], fragmentOutlineShader[0]);
	//coba->resetEuler();

	tornado->initiateBuffer();
	tornado->initiateShader(vertexShader[0], fragmentShader[0]);
	tornado->getChild(0)->getParent()->initiateShader(vertexShader[0], fragmentShader[2]);
	tornado->getChild(0)->getChild(0)->getParent()->initiateShader(vertexShader[0], fragmentShader[2]);
	tornado->initiateOutlineShader(vertexShader[0], fragmentOutlineShader[0]);
	tornado->resetEuler();

	tornado2->initiateBuffer();
	tornado2->initiateShader(vertexShader[0], fragmentShader[0]);
	tornado2->getChild(0)->getParent()->initiateShader(vertexShader[0], fragmentShader[2]);
	tornado2->getChild(0)->getChild(0)->getParent()->initiateShader(vertexShader[0], fragmentShader[2]);
	tornado2->initiateOutlineShader(vertexShader[0], fragmentOutlineShader[0]);
	tornado2->resetEuler();
}

void render() {
	glEnableVertexAttribArray(0);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tornado->drawPolygon();
		tornado->drawPolyline();
		tornado->rotate(tornado->getParent()->getPosition(), Vertex(0, 1, 0), 0.7);
		tornado->rotate(tornado2->getParent()->getPosition(), Vertex(0, 1, 0), 0.5);
		//tornado->getChild(1)->getParent()->rotate(tornado->getChild(0)->getParent()->getPosition(), Vertex(1, 1, 0), 0.7);
		//tornado->getChild(1)->getParent()->rotate(tornado->getParent()->getPosition(), Vertex(1, 0, 0), 0.5);
		tornado->getChild(0)->getParent()->rotate(tornado->getParent()->getPosition(), Vertex(1, 1, 0), 0.7);
		tornado->getChild(0)->getChild(0)->getParent()->rotate(tornado->getParent()->getPosition(), Vertex(1, 1, 0), 0.7);

		//coba->drawPolygon();
		//coba->drawPolyline();
		//coba->rotate(tornado->getParent()->getPosition(), Vertex(0, 1, 0), 0.7);
		//coba->rotate(tornado2->getParent()->getPosition(), Vertex(0, 1, 0), 0.5);

		tornado2->drawPolygon();
		tornado2->drawPolyline();
		tornado2->rotate(tornado2->getParent()->getPosition(), Vertex(0, 1, 0), 0.7);
		tornado2->rotate(tornado->getParent()->getPosition(), Vertex(0, 1, 0), 0.5);
		//tornado2->getChild(0)->getParent()->rotate(tornado2->getChild(0)->getParent()->getPosition(), Vertex(1, 1, 0), 0.7);
		//tornado2->getChild(0)->getParent()->rotate(tornado2->getParent()->getPosition(), Vertex(1, 0, 0), 0.5);
		tornado2->getChild(0)->getParent()->rotate(tornado2->getParent()->getPosition(), Vertex(1, 1, 0), 0.7);
		tornado2->getChild(0)->getChild(0)->getParent()->rotate(tornado2
			->getParent()->getPosition(), Vertex(1, 1, 0), 0.7);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	glDisableVertexAttribArray(0);
}

void cleanup() {
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
}

int main() {
	initializeGLFW();
	initializeWindow();
	initializeGLEW();
	initializeShapes();

	render();
}