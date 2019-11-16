#include <iostream>
#include <stdlib.h>
#include <glew.h>
#include <glfw3.h>
#include "Shader.h"
#include "Shape.h"
using namespace std;

int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 1000;
Hierarchy arm(new Ovaloid(0, 0.3, 0, 35, Vertex(0.1, 0.1), 1));
float angle = 0;
bool check = false;

GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
GLuint VertexArrayID;
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
void keyboardPressEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_9 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(2)->getAngel() > 50) {
			check = true;
		}
		else if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(2)->getAngel() <= 0) {
			check = false;
		}
		if (check)
			angle *= -1;
		else
			angle *= 1;
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(2)->rotate(arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(1), angle);
	}
	else if (key == GLFW_KEY_8 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(1)->getAngel() > 90) {
			check = true;
		}
		else if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(1)->getAngel() < 45) {
			check = false;
		}
		if (check)
			angle *= -1;
		else
			angle *= 1;
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(1)->rotate(arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(1), angle);
	}
	else if (key == GLFW_KEY_7 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getAngel() > 90) {
			check = true;
		}
		else if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getAngel() < 45) {
			check = false;
		}
		if (check)
			angle *= -1;
		else
			angle *= 1;
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->rotate(arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getChild(0)->getChild(0)->getParent()->getEuler(0), angle);
	}
	else if (key == GLFW_KEY_6 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getAngel() > 180) {
			check = true;
		}
		else if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getAngel() < 0) {
			check = false;
		}
		if (check)
			angle *= -1;
		else
			angle *= 1;
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->rotate(arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(1), angle);
	}
	else if (key == GLFW_KEY_5 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getAngel() > 90) {
			check = true;
		}
		else if (arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getAngel() <= 0) {
			check = false;
		}
		if (check)
			angle *= -1;
		else
			angle *= 1;
		arm.getChild(0)->getChild(0)->getChild(0)->rotate(arm.getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(0), angle);
	}
	else if (key == GLFW_KEY_4 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getChild(0)->getChild(0)->getChild(0)->getAngel() < -80)
			check = true;
		else if (arm.getChild(0)->getChild(0)->getChild(0)->getAngel() > 30)
			check = false;
		if (check)
			angle *= 1;
		else
			angle *= -1;
		arm.getChild(0)->getChild(0)->getChild(0)->rotate(arm.getChild(0)->getChild(0)->getParent()->getPosition(), Vertex(0, 0, 1), angle);
	}
	else if (key == GLFW_KEY_3 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getAngel() < 0)
			check = true;
		else if (arm.getAngel() >= 180)
			check = false;
		if (check)
			angle *= 1;
		else
			angle *= -1;
		arm.rotate(arm.getParent()->getPosition(), Vertex(0, 0, 1), angle);
	}
	else if (key == GLFW_KEY_2 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getAngel() <= 0)
			check = true;
		else if (arm.getAngel() > 180)
			check = false;
		if (check)
			angle *= 1;
		else
			angle *= -1;
		arm.rotate(arm.getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(1), angle);
	}
	else if (key == GLFW_KEY_1 && action == GLFW_REPEAT) {
		angle = 5;
		if (arm.getAngel() < -90)
			check = true;
		else if (arm.getAngel() >= 90)
			check = false;
		if (check)
			angle *= 1;
		else
			angle *= -1;
		arm.rotate(arm.getParent()->getPosition(), Vertex(1, 0, 0), angle);
	}


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
	glfwSetKeyCallback(window, keyboardPressEvent);
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

	char vertexShader[][100] = { "shaders/circle/vertex.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_2.shader","shaders/triangle/vertex_1.shader" };
	char fragmentShader[][100] = { "shaders/triangle/.shader", "shaders/triangle/red.shader", "shaders/triangle/blue.shader","shaders/triangle/brown.shader" };
	char fragmentOutlineShader[][100] = { "shaders/circle/fragment_outline.shader", "shaders/triangle/fragment_outline_1.shader", "shaders/triangle/fragment_outline_2.shader","shaders/triangle/fragment_outline_1.shader" };

	arm.rotate(arm.getParent()->getPosition(), arm.getParent()->getEuler(2), 90);

	arm.addChild(new Hierarchy(new Ovaloid(
		arm.getParent()->getPosition().x,
		arm.getParent()->getPosition().y - 0.22,
		0, 25, Vertex(0.2, 0.08), 1))); //Lengan
	arm.getChild(0)->rotate(arm.getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(2), 90);

	arm.getChild(0)->addChild(new Hierarchy(new Ovaloid(
		arm.getChild(0)->getParent()->getPosition().x,
		arm.getChild(0)->getParent()->getPosition().y - 0.2,
		0, 25, Vertex(0.05, 0.05), 1))); //Sendi bawah

	arm.getChild(0)->getChild(0)->addChild(new Hierarchy(new Ovaloid(
		arm.getChild(0)->getChild(0)->getParent()->getPosition().x + 0.22,
		arm.getChild(0)->getChild(0)->getParent()->getPosition().y + 0,
		0, 25, Vertex(0.25, 0.065), 1))); //Lengan bawah

	arm.getChild(0)->getChild(0)->getChild(0)->addChild(new Hierarchy(new Ovaloid(
		arm.getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().x + 0.25,
		arm.getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().y,
		0, 25, Vertex(0.05, 0.058), 1))); //telapak

	arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->addChild(new Hierarchy(new Ovaloid(
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().x,
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().y + 0.08,
		0, 25, Vertex(0.05, 0.015), 1))); //jempol
	arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->rotate(arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(2), 90);

	arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->addChild(new Hierarchy(new Ovaloid(
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().x + 0.02,
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().y + 0.022,
		0, 25, Vertex(0.05, 0.015), 1))); //tengah
	arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(1)->rotate(arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition(), arm.getChild(0)->getParent()->getEuler(2), 45);

	arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->addChild(new Hierarchy(new Ovaloid(
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().x + 0.07,
		arm.getChild(0)->getChild(0)->getChild(0)->getChild(0)->getParent()->getPosition().y,
		0, 25, Vertex(0.05, 0.015), 1))); //Kelingking

	arm.initiateBuffer();
	arm.initiateShader(vertexShader[1], fragmentShader[2]);
	arm.initiateOutlineShader(vertexShader[1], fragmentOutlineShader[1]);
	arm.resetEuler();
}

void render() {
	glEnableVertexAttribArray(0);
	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		arm.drawPolygon();
		arm.drawPolyline();

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

int main()
{
	initializeGLFW();
	initializeWindow();
	initializeGLEW();
	initializeShapes();

	render();
}