#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Shader.h"
#include "Window.h"
#include "PlayerGameObject.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Transform Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);

// Global texture info
GLuint tex[3];

// Global game object info
std::vector<GameObject*> gameObjects;

/*---------------new attributes--------------------------- */
//Global bullets num
int maxBullets = 10;
//Global shooting time interval
double shootInterval = 2;
int numFlyingBullets = 0;

/*------------------------------------------ */

// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}


void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(void)
{
//	tex = new GLuint[4];
	glGenTextures(4, tex);
	setthisTexture(tex[0], "Illidan.png");
	setthisTexture(tex[1], "saw.png");
	setthisTexture(tex[2], "rock.png");
	setthisTexture(tex[3], "orb.png");

	glBindTexture(GL_TEXTURE_2D, tex[0]);
}


// Main function that builds and runs the game
int main(void){
	try {
		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create geometry of the square
		int size = CreateSquare();

		// Set up shaders
		Shader shader("shader.vert", "shader.frag");

		// Set up the textures
		setallTexture();

		// Setup the player object (position, texture, vertex count)
		gameObjects.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[0], size,0));

		// Setup asteroids
		for (int i = 0; i < 20; i++) {
			double randX = 5* (rand() / double(RAND_MAX)-0.5f);
			double randY = 10 * (rand() / double(RAND_MAX))-1 ;
			gameObjects.push_back(new GameObject(glm::vec3(randX, randY, 0.0f), tex[2], size, 2));
		}
		
		for (int i = 0; i < 10; i++) {
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), gameObjects[0]->getPosition());
			translationMatrix = glm::translate(translationMatrix, glm::vec3(0.8f, 0.0f, 0.0f));
			glm::mat4 rotationMatrix = glm::rotate(translationMatrix, 0.5f*i, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 iniMat = rotationMatrix;// *translationMatrix;
			glm::vec4 iniPos = (iniMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			
			GameObject	*ringOrb = new GameObject(glm::vec3(iniPos.x, iniPos.y, 1.0f), tex[1], size, 1);
			ringOrb->setRotationMatrix(glm::rotate(glm::mat4(1.0f), 0.1f, glm::vec3(0.0f, 0.0f, 1.0f)));
			gameObjects.push_back(ringOrb);
		}
		
		// Run the main loop
		double lastShootTime = glfwGetTime();
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(viewport_background_color_g);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			

			// Select proper shader program to use
			shader.enable();

			// Setup camera to focus on the player object (the first object in the gameObjects array)
			glm::vec3 cameraTranslatePos(-gameObjects[0]->getPosition());
			float cameraZoom = 0.5f;
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);


			// Update and render all game objects
			for (int i = 0; i < gameObjects.size(); i++) {
				// Get the current object
				GameObject* currentGameObject = gameObjects[i];


				// Updates game objects
				currentGameObject->update(deltaTime);

				// Check for collision between game objects
				for (int j = i + 1; j < gameObjects.size(); j++) {
					GameObject* otherGameObject = gameObjects[j];

					float distance = glm::length(currentGameObject->getPosition() - otherGameObject->getPosition());
					if (distance < 0.1f) {
						// This is where you would perform collision response between objects
					}
				}

				// Render game objects
				currentGameObject->render(shader);
			}


			if (glfwGetKey(Window::getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS
				&& (currentTime - lastShootTime) > shootInterval
				&&  numFlyingBullets < maxBullets
				) {
				gameObjects.push_back(new GameObject(gameObjects[0]->getPosition(), tex[1], size,1));
				++numFlyingBullets;
				lastShootTime = currentTime;
			}


			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window.getWindow());
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}
