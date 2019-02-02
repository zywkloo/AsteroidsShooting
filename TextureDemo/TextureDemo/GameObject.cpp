#include "GameObject.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	translationMatrix = glm::translate(glm::mat4(1.0f), position);
	rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.4f, 1.0f));
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime) {
	// Update object position
	position += velocity * (float)deltaTime;
}

// Renders the GameObject using a shader
void GameObject::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations

	// Set the transformation matrix in the shader
	// TODO: Multiply your new transformations to make the transformationMatrix
	//glm::mat4 transformationMatrix = translationMatrix;
	glm::mat4 renderMatrix = translationMatrix*rotationMatrix*scaleMatrix;
	shader.setUniformMat4("transformationMatrix", renderMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
