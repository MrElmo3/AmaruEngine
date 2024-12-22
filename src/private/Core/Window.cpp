#include "Core/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

#include "Core/Scenes/Maze2DScene.h"
#include "Core/Scenes/TestScene.h"
#include "Core/Global.h"
#include "Core/Objects/Maze.h"
#include "Core/Scenes/TestColisionScene.h"
#include "Util/Logger.h"

Window::Window():Singleton(){
	actualScene = nullptr;
}

void Window::Start() {
	Logger::Log("Window Configuration");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		Global::SCREEN_WIDTH,
		Global::SCREEN_HEIGHT,
		"Maze",
		nullptr,
		nullptr);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Logger::Error("Failed to initialize GLAD");
	}
	actualScene = new TestScene();
	actualScene->Awake();
	actualScene->Start();
}

void Window::Update() {
	while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
		ProcessFrame(glfwGetCurrentContext());
		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();
	}
}

void Window::End() {
	glfwTerminate();
}


void Window::Excecute() {
	Start();
	Update();
	End();
}

void Window::ProcessFrame(GLFWwindow* window) {
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
	}
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	actualScene->SceneUpdate();
}

void Window::AddObjectToScene(AObject* object) {
	actualScene->AddObject(object);
}
