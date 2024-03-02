//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"

#include <iostream>

int glWindowWidth = 1500;
int glWindowHeight = 1000;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN};
gps::Camera myCamera(
				glm::vec3(0.0f, 0.0f, 2.5f), 
				glm::vec3(0.0f, 0.0f, -10.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.01f;

bool pressedKeys[1024];
float angleY = 0.0f;

gps::Model3D vagon;
glm::vec3 wagonPosition = glm::vec3(-3.5f, -0.03f, -2.5f);
gps::Shader myCustomShader;
gps::Model3D house;
gps::Model3D dulap;
glm::vec3 dulapPosition = glm::vec3(2.8f, 0.0f, -3.86385f);
gps::Model3D usa;
glm::vec3 usaPosition = glm::vec3(2.8f, 0.0f, -3.86385f);
float doorAngle = 0.0f;
float doorWidth = 0.2f;




//fog
int fogReady;
GLint foginitLoc;

//point light 
int startPoint = 0;
glm::vec3 lightPos1;
GLuint lightPos1Loc;


GLenum glCheckError_(const char *file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}
bool mousedFirstMoved = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = glWindowWidth/2.0f, tlastY = glWindowWidth/2.0f;

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	// grab the pointer to our window as this is a static function
	
	if (mousedFirstMoved)
	{
		lastX = xPos;
		tlastY = yPos;
		mousedFirstMoved = false;
	}
	float xChange = xPos - lastX;
	float tyChange = tlastY - yPos;

	lastX = xPos;
	tlastY = yPos;

	float sensitivity = 0.4f;
	xChange *= sensitivity;
	tyChange *= sensitivity;


	yaw += xChange;
	pitch += tyChange;


	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	myCamera.rotate(pitch, yaw);


}

float angle = 0;
float angle1 = 0;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

void processMovement()
{

	if (glfwGetKey(glWindow, GLFW_KEY_F)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_E)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_V)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	model = glm::mat4(1.0f);
	if (glfwGetKey(glWindow, GLFW_KEY_R))
	{
		angle += 0.5f;
		if (angle > 360.0f)
			angle -= 360.0f;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_L))
	{
		angle -= 0.5f;
		if (angle < 0.0f)
			angle += 360.0f;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_LEFT))
	{
		angle1 -= 0.002f;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_RIGHT))
	{
		angle1 += 0.002f;
	}
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 1.0f * deltaTime;
	if (glfwGetKey(glWindow, GLFW_KEY_W))
	{
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);

	}
	if (glfwGetKey(glWindow, GLFW_KEY_S))
	{
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);

	}
	if (glfwGetKey(glWindow, GLFW_KEY_D))
	{
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);

	}
	if (glfwGetKey(glWindow, GLFW_KEY_A))
	{
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);

	}
	//ADDED BY ME
	if (glfwGetKey(glWindow, GLFW_KEY_Y))
	{
		myCamera.move(gps::MOVE_UP, cameraSpeed);

	}
	if (glfwGetKey(glWindow, GLFW_KEY_H))
	{
		myCamera.move(gps::MOVE_DOWN, cameraSpeed);

	}

	//fog
	if (glfwGetKey(glWindow, GLFW_KEY_Q))
	{
		myCustomShader.useShaderProgram();
		fogReady = 1;
		foginitLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fogReady");
		glUniform1i(foginitLoc, fogReady);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_Z))
	{
		myCustomShader.useShaderProgram();
		fogReady = 0;
		foginitLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fogReady");
		glUniform1i(foginitLoc, fogReady);
	}

	//point light 
	if (glfwGetKey(glWindow, GLFW_KEY_O)) {
		myCustomShader.useShaderProgram();
		startPoint = 1;
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "startPoint"), startPoint);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_P)) {
		myCustomShader.useShaderProgram();
		startPoint = 0;
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "startPoint"), startPoint);
	}

	//wagon movement
	if (glfwGetKey(glWindow, GLFW_KEY_N)) {
		wagonPosition.x += 0.1f; // Adjust this value for speed
	}

	// Move wagon backward
	if (glfwGetKey(glWindow, GLFW_KEY_M)) {
		wagonPosition.x -= 0.1f; // Adjust this value for speed
	}
	if (glfwGetKey(glWindow, GLFW_KEY_K)) {
		if (doorAngle < 90.0f) { // Assuming we want to open the door to 90 degrees
			doorAngle += 1.0f; // Adjust this value for speed
		}
	}

	// Close the door with 'L' key
	if (glfwGetKey(glWindow, GLFW_KEY_J)) {
		if (doorAngle > 0.0f) {
			doorAngle -= 1.0f; // Adjust this value for speed
		}
	}



	// create a translation matrix
	model = glm::translate(model, glm::vec3(2, 0, 0));
	// send matrix data to vertex shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// create rotation matrix
	model = glm::rotate(model, angle1, glm::vec3(0, 1, 0));
	// send matrix data to vertex shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	myCustomShader.useShaderProgram();
	glm::mat4 view = myCamera.getViewMatrix();
	GLint viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	//myModel.LoadModel("objects/nanosuit/nanosuit.obj", "objects/nanosuit/");
	house.LoadModel("objects/scene/myScene.obj", "objects/scene/");
	vagon.LoadModel("objects/scene/wagon.obj", "objects/scene/");
	dulap.LoadModel("objects/scene/dulap/dulap.obj", "objects/scene/dulap/");
	usa.LoadModel("objects/scene/usa/usa1.obj", "objects/scene/usa/");
}

void initShaders() {

	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();

}

void initUniforms() {
	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	//point light 
	lightPos1 = glm::vec3(3.3f, 0.3f, -1.5347f);
	lightPos1Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos1");
	glUniform3fv(lightPos1Loc, 1, glm::value_ptr(lightPos1));
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	house.Draw(myCustomShader);

	model = glm::translate(glm::mat4(1.0f), wagonPosition);
	model = glm::scale(model, glm::vec3(0.21f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	vagon.Draw(myCustomShader);

	model = glm::translate(glm::mat4(1.0f), dulapPosition);
	model = glm::scale(model, glm::vec3(0.228f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dulap.Draw(myCustomShader);

	glm::mat4 doorModel = glm::mat4(1.0f);
	doorModel = glm::translate(doorModel, usaPosition); // Move to door position

	// The pivot translation might need to be adjusted depending on your model's pivot point
	doorModel = glm::translate(doorModel, glm::vec3(doorWidth /1.5, 0.0f, 0.0f)); // Translate to the hinge
	doorModel = glm::rotate(doorModel, glm::radians(doorAngle), glm::vec3(0, 1, 0)); // Rotate the door
	doorModel = glm::translate(doorModel, glm::vec3(-doorWidth / 1.5, 0.0f, 0.0f)); // Translate back

	doorModel = glm::scale(doorModel, glm::vec3(0.228f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(doorModel));
	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	usa.Draw(myCustomShader);
}

void cleanup() {

	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char * argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
