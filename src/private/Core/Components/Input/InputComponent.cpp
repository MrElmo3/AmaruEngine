#include "Core/Components/Input/InputComponent.h"

#include "Game/Components/MovementComponent.h"
#include "Core/Objects/AObject.h"

InputComponent::InputComponent(AObject* parent) : IComponent(parent) {
}

InputComponent::~InputComponent() {
}

void InputComponent::Update(double deltaTime) {
	SetMovementDirection();
	SetLookDelta();
}

void InputComponent::Start() {
	movementComponent = parent->GetComponent<MovementComponent>();

	// Hide the cursor and lock it to the window so it never leaves while looking around.
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

void InputComponent::SetMovementDirection() {
	glm::vec2 direction = glm::vec2(0);

	//WASD
	if (glfwGetKey(glfwGetCurrentContext(), UP) == GLFW_PRESS) {
		direction += glm::vec2(0, 1);
	}
	if (glfwGetKey(glfwGetCurrentContext(), DOWN) == GLFW_PRESS) {
		direction += glm::vec2(0, -1);
	}
	if (glfwGetKey(glfwGetCurrentContext(), LEFT) == GLFW_PRESS) {
		direction += glm::vec2(-1, 0);
	}
	if (glfwGetKey(glfwGetCurrentContext(), RIGHT) == GLFW_PRESS) {
		direction += glm::vec2(1, 0);
	}

	if (movementComponent != nullptr) {
		movementComponent->SetDirection(direction);
	}
}

void InputComponent::SetLookDelta() {
	glm::dvec2 cursor;
	glfwGetCursorPos(glfwGetCurrentContext(), &cursor.x, &cursor.y);

	// The first frame has no previous position: skip it so the camera does not jump.
	if (firstMouseFrame) {
		lastCursorPosition = cursor;
		firstMouseFrame = false;
		return;
	}

	glm::vec2 delta = glm::vec2(cursor - lastCursorPosition);
	lastCursorPosition = cursor;

	// Screen Y grows downwards, so moving the mouse up must look up.
	if (movementComponent != nullptr && delta != glm::vec2(0)) {
		movementComponent->Look(glm::vec2(delta.x, -delta.y));
	}
}
