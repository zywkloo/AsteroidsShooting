#include "GameObject.h"
#include "Window.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, GLint entityType) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	translationMatrix = glm::translate(glm::mat4(1.0f), position);
	//	type 3: bullets
	if (entityType == 3) {
		objectSize = 0.1;
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(objectSize, objectSize, 1.0f));
		rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	//	type 2: asteroids
	else if (entityType == 2) {
		objectSize=  0.3 * (rand() / double(RAND_MAX)) +0.3;
		float objectRotation = 0.5 * (rand() / double(RAND_MAX)) + 0.5;
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(objectSize, objectSize, 1.0f));
		rotationMatrix = glm::rotate(glm::mat4(1.0f), objectRotation, glm::vec3(0.0f, 0.0f, 1.0f));
	// type 1: shield
	}  else if (entityType == 1){
		objectSize = 0.2;
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(objectSize, objectSize, 1.0f));
		rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
	// type 0:player
	} else { 
		scaleMatrix = glm::mat4(1.0f); 
		rotationMatrix = glm::mat4(1.0f);
	}
	translationSelfMatrix= glm::mat4(1.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	type = entityType;
}

// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime) {
	// Update object position
	if (type == 2) {
		if (objectSize>0.45) {
			rotationMatrix = glm::rotate(rotationMatrix, 0.02f, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else {
			rotationMatrix = glm::rotate(rotationMatrix, 0.05f, glm::vec3(0.0f, 0.0f, 1.0f));
		}
	} else if (type == 1) {
		rotationMatrix = glm::rotate(rotationMatrix, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		// Checking for player input and changing velocity
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			// speed up
			translationMatrix = glm::translate(translationMatrix, glm::vec3(0.0f, 0.001f, 0.0f));
		}
		else if (glfwGetKey(Window::getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			// speed down
			translationMatrix = glm::translate(translationMatrix, glm::vec3(0.0f, 0.00008f, 0.0f));
		}
		else {
			//normal speed
			translationMatrix = glm::translate(translationMatrix, glm::vec3(0.0f, 0.0002f, 0.0f));
		}
		translationSelfMatrix = glm::translate(translationSelfMatrix, glm::vec3(0.0f, 0.0002f, 0.0f));
	}
	glm::vec4 transformedPos = (translationMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	position.x = transformedPos.x;
	position.y = transformedPos.y;
	//position += velocity * (float)deltaTime;
}

// Renders the GameObject using a shader
void GameObject::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations
	//if (type == 1) {};
	// Set the transformation matrix in the shader
	// TODO: Multiply your new transformations to make the transformationMatrix
	glm::mat4 renderMatrix = glm::mat4(1.0f);
	if (type == 1) {
		glm::mat4 translationInverseMatrix = translationSelfMatrix;
		glm::vec4 orbitColumn = (translationInverseMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::mat4 translationOrbitMatrix= glm::translate(glm::mat4(1.0f), glm::vec3(-orbitColumn.x, -orbitColumn.y,0.0f));
		//glm::mat4 translationOrbitMatrix = glm::inverse(translationSelfMatrix);
		renderMatrix = translationMatrix * scaleMatrix  *rotationMatrix*translationInverseMatrix*rotationMatrix*translationOrbitMatrix;
	} else {
		renderMatrix = translationMatrix *scaleMatrix* rotationMatrix;
	}
	shader.setUniformMat4("transformationMatrix", renderMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
