#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

class GameObject {
public:
	GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements,GLint type);

	// Updates the GameObject's state. Can be overriden for children
	virtual void update(double deltaTime);

	// Renders the GameObject using a shader
	void render(Shader &shader);


	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getVelocity() { return velocity; }
	inline glm::mat4& getTranslationMatrix() { return translationMatrix; };
	inline glm::mat4& getRotationMatrix() { return rotationMatrix; };
	inline glm::mat4& getScaleMatrix() { return scaleMatrix; };
	inline glm::mat4& getTranslationSelfMatrix() { return translationSelfMatrix; };


	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setVelocity(glm::vec3& newVelocity) { velocity = newVelocity; }
	inline void setTranslationMatrix(glm::mat4 newTranslationMatrix) { translationMatrix = newTranslationMatrix; };
	inline void setRotationMatrix(glm::mat4 newRotationMatrix) { rotationMatrix = newRotationMatrix; };
	inline void setScaleMatrix(glm::mat4 newScaleMatrix) { scaleMatrix = newScaleMatrix; };
	inline void setTranslationSelfMatrix(glm::mat4 newTranslationSelfMatrix) { translationSelfMatrix = newTranslationSelfMatrix; };

protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	glm::vec3 position;
	glm::vec3 velocity;
	glm::mat4 translationMatrix ;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationSelfMatrix;
	GLuint type;

	// Object's details
	GLint numElements;
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)

	// Object's texture
	GLuint texture;
};