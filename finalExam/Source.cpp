// C++ include
#include <iostream>
#include <vector>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM - Math Function for openGL
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Window dimensions
GLuint WIDTH = 800, HEIGHT = 800;

// Function prototypes
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void terminal_output();
void Drawyouwant();
void drawcube(GLfloat, GLfloat, GLfloat);

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 2.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -2.5f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;
GLfloat Euler_yaw = -90.0f, Euler_pitch = -22.0f;
GLfloat fov = 45.0f;
bool Mouse_grabbed = false;
bool keys[1024];

std::vector<glm::vec3> square;

int main(int argc, char* argv[]){
	if (!glfwInit())
		std::cout << "Can not init glfw.\n";

	GLFWwindow *window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "asixTool", NULL, NULL);
	if (!window){
		glfwTerminate();
		std::cout << "Can not open window.\n";
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_click_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.0f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glFlush();
	GLfloat draw_asixs[3][3] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } };
	while (!glfwWindowShouldClose(window)){
		glLoadIdentity();

		glfwPollEvents();
		do_movement();
		terminal_output();

		glClear(GL_COLOR_BUFFER_BIT);
		// + 初始(或重新設定) gluLookAt位置
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.0f, 10.0f);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z, cameraUp.x, cameraUp.y, cameraUp.z);
		//gluLookAt(0.0f, 1.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f);
		// + ---------------------------------------------------------

		// + 畫出坐標系 (先決:開始畫的點、畫多大)
		glPushMatrix();
		for (GLfloat x = -10.0f; x <= 10.0f; x += (GLfloat)0.2){
			glBegin(GL_LINES);
				glVertex3f(x, 0, -10.0f);
				glVertex3f(x, 0, +10.0f);
			glEnd();
		}
		for (GLfloat z = -10.0f; z <= 10.0f; z += (GLfloat)0.2){
			glBegin(GL_LINES);
				glVertex3f(-10.0f, 0.0f, z);
				glVertex3f(+10.0f, 0.0f, z);
			glEnd();
		}
		glPopMatrix();
		for (int asix_state = 0; asix_state < 3; ++asix_state){
			glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
				glPushMatrix();
					glLineWidth(5.0f);
					glBegin(GL_LINES);
						glColor3f(draw_asixs[asix_state][0], draw_asixs[asix_state][1], draw_asixs[asix_state][2]);
						glVertex3f(draw_asixs[asix_state][0] * -10.0f, draw_asixs[asix_state][1] * -10.0f, draw_asixs[asix_state][2] * -10.0f);
						glVertex3f(draw_asixs[asix_state][0] * 10.0f, draw_asixs[asix_state][1] * 10.0f, draw_asixs[asix_state][2] * 10.0f);
					glEnd();
				glPopMatrix();
			glPopAttrib();
		}
		glPushAttrib(GL_POINT_BIT);
			glPushMatrix();
				glPointSize(10.0f);
				glBegin(GL_POINTS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glEnd();
			glPopMatrix();
		glPopAttrib();
		// + ---------------------------------------------------------

		// + 呼叫主要函式
		for (int i = 0; i < square.size();++i)
			drawcube(square[i].x, square[i].y, square[i].z);
		Drawyouwant();

		glFlush();
		glfwSwapBuffers(window);
	}


	glfwTerminate(); return 0;
}

void window_size_callback(GLFWwindow* window, int width, int height){
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.0f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		Mouse_grabbed = true;
	else
		Mouse_grabbed = false;
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos){
	if (!Mouse_grabbed){
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
	}
	else{
		GLfloat x_offset = (GLfloat)xpos - lastX;
		GLfloat y_offset = lastY - (GLfloat)ypos;
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;

		GLfloat sensitivity = (GLfloat)0.05;
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		Euler_yaw += x_offset;
		Euler_pitch += y_offset;

		if (Euler_pitch > 89.0f)
			Euler_pitch = 89.0f;
		if (Euler_pitch < -89.0f)
			Euler_pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(Euler_yaw)) * cos(glm::radians(Euler_pitch));
		front.y = sin(glm::radians(Euler_pitch));
		front.z = sin(glm::radians(Euler_yaw)) * cos(glm::radians(Euler_pitch));
		cameraFront = glm::normalize(front);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS)
			keys[key] = true;
		else 
			if (action == GLFW_RELEASE)
				keys[key] = false;
	}
}

void do_movement(){
	GLfloat cameraSpeed = 0.08f;
	if (keys[GLFW_KEY_W]){
		cameraPos.z += cameraFront.z * cameraSpeed;
		cameraPos.x += cameraFront.x * cameraSpeed;
	}
	if (keys[GLFW_KEY_S]){
		cameraPos.z -= cameraFront.z * cameraSpeed;
		cameraPos.x -= cameraFront.x * cameraSpeed;
	}
	if (keys[GLFW_KEY_A]){
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (keys[GLFW_KEY_D]){
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (keys[GLFW_KEY_PAGE_DOWN]){
		cameraPos.y += cameraFront.y * cameraSpeed * 2;
	}
	if (keys[GLFW_KEY_PAGE_UP]){
		cameraPos.y -= cameraFront.y * cameraSpeed * 2;
	}

	if (keys[GLFW_KEY_R]){
		int cacul[3];
		bool judge = false;
		cacul[0] = (cameraPos.x + cameraFront.x) / 0.2f;
		cacul[1] = (cameraPos.y + cameraFront.y) / 0.2f;
		cacul[2] = (cameraPos.z + cameraFront.z) / 0.2f;
		glm::vec3 temp = glm::vec3(cacul[0] * 0.2f, cacul[1] * 0.2f, cacul[2] * 0.2f);
		for (int i = 0; i < square.size(); ++i)
			if (square[i] == temp){
				judge = true;
				break;
			}
		if (!judge)
			square.push_back(glm::vec3(cacul[0] * 0.2f, cacul[1] * 0.2f, cacul[2] * 0.2f));
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (GLfloat)yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void terminal_output(){
	system("cls");
	printf("CameraPosition:(%.2f,%.2f,%.2f)\n", cameraPos.x, cameraPos.y, cameraPos.z);
	printf("CameraFront:(%.2f,%.2f,%.2f)\n", cameraFront.x, cameraFront.y, cameraFront.z);
	printf("CameraUp:(%.2f,%.2f,%.2f)\n", cameraUp.x, cameraUp.y, cameraUp.z);
	printf("Theta of Perspective:%.0f", fov);
}

void drawcube(GLfloat pointX, GLfloat pointY, GLfloat pointZ){
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0, 1.0, 1.0);
	// TOP
	glBegin(GL_POLYGON);
	glVertex3f(pointX + 0.2f, pointY + 0.2f, pointZ-0.2f);
	glVertex3f(pointX, pointY + 0.2f, pointZ - 0.2f);
	glVertex3f(pointX, pointY + 0.2f, pointZ);
	glVertex3f(pointX + 0.2f, pointY + 0.2f, pointZ);
	glEnd();

	// RIGHT
	glBegin(GL_POLYGON);
	glVertex3f(pointX + 0.2f, pointY + 0.2f, pointZ-0.2f);
	glVertex3f(pointX + 0.2f, pointY + 0.2f, pointZ);
	glVertex3f(pointX + 0.2f, pointY, pointZ);
	glVertex3f(pointX + 0.2f, pointY, pointZ-0.2f);
	glEnd();

	// LEFT
	glBegin(GL_POLYGON);
	glVertex3f(pointX, pointY + 0.2f, pointZ - 0.2f);
	glVertex3f(pointX, pointY + 0.2f, pointZ);
	glVertex3f(pointX, pointY, pointZ);
	glVertex3f(pointX, pointY, pointZ - 0.2f);
	glEnd();

	// BACK
	glBegin(GL_POLYGON);
	glVertex3f(pointX + 0.2f, pointY + 0.2f, pointZ - 0.2f);
	glVertex3f(pointX, pointY + 0.2f, pointZ - 0.2f);
	glVertex3f(pointX, pointY, pointZ - 0.2f);
	glVertex3f(pointX + 0.2f, pointY, pointZ - 0.2f);
	glEnd();

	// BOTTOM
	glBegin(GL_POLYGON);
	glVertex3f(pointX + 0.2f, pointY, pointZ - 0.2f);
	glVertex3f(pointX, pointY, pointZ - 0.2f);
	glVertex3f(pointX, pointY, pointZ);
	glVertex3f(pointX + 0.2f, pointY, pointZ);
	glEnd();

	// FRONT
	glBegin(GL_POLYGON);
	glVertex3f(pointX + 0.2f, pointY + 0.2f, pointZ);
	glVertex3f(pointX, pointY + 0.2f, pointZ);
	glVertex3f(pointX, pointY, pointZ);
	glVertex3f(pointX + 0.2f, pointY, pointZ);
	glEnd();
	glPopAttrib();
}

void Drawyouwant(){

	return;
}