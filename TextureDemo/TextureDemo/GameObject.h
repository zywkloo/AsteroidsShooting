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

	/*Game Obj type
	0: player;
	1: shield
	2: asteroids
	3: bulletts*/
	GLuint type;

	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getVelocity() { return velocity; }
	inline glm::mat4& getTranslationMatrix() { return translationMatrix; };
	inline glm::mat4& getRotationMatrix() { return rotationMatrix; };
	inline glm::mat4& getScaleMatrix() { return scaleMatrix; };
	inline glm::mat4& getTranslationSelfMatrix() { return translationSelfMatrix; };
	inline GLboolean& getVisable() { return isVisable; };
	// save the iniangle
	double angle = 0;
	//obje size
	float objectSize;
	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setVelocity(glm::vec3& newVelocity) { velocity = newVelocity; }
	inline void setTranslationMatrix(glm::mat4 newTranslationMatrix) { translationMatrix = newTranslationMatrix; };
	inline void setRotationMatrix(glm::mat4 newRotationMatrix) { rotationMatrix = newRotationMatrix; };
	inline void setScaleMatrix(glm::mat4 newScaleMatrix) { scaleMatrix = newScaleMatrix; };
	inline void setTranslationSelfMatrix(glm::mat4 newTranslationSelfMatrix) { translationSelfMatrix = newTranslationSelfMatrix; };
	inline void setVisable(GLboolean newVisable){isVisable = newVisable;};
protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	glm::vec3 position;
	glm::vec3 velocity;
	glm::mat4 translationMatrix ;  
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationSelfMatrix;
	GLboolean isVisable;




	// Object's details
	GLint numElements;


	// Object's texture
	GLuint texture;
};