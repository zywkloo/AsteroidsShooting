#include "PlayerGameObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements) {}

// Update function for moving the player object around
void PlayerGameObject::update(double deltaTime) {

	translationMatrix = glm::translate(translationMatrix,glm::vec3(0.0f, 0.0001f, 0.0f));
	//rotationMatrix = glm::rotate(rotationMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.4f, 1.0f));
	// Checking for player input and changing velocity
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
		// change the status
		translationMatrix = glm::translate(translationMatrix, glm::vec3(0.0f, 0.0002f, 0.0f));
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
		// This is where you should change the velocity vector of the player
		translationMatrix = glm::translate(translationMatrix, glm::vec3(0.0f, 0.00005f, 0.0f));
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
		// This is where you should change the velocity vector of the player
		rotationMatrix = glm::rotate(rotationMatrix, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (glfwGetKey(Window::getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
		// This is where you should change the velocity vector of the player
		rotationMatrix = glm::rotate(rotationMatrix, -0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm:: vec4 transformedPos = (translationMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	position.x = transformedPos.x;
	position.y = transformedPos.y;

	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}
