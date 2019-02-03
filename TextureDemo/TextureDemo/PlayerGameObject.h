#pragma once

#include "GameObject.h"

// Inherits from GameObject
class PlayerGameObject : public GameObject {
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements,GLint type);


	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
};
