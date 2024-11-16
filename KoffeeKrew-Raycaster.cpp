// KoffeeKrew-Raycaster.cpp : Defines the entry point for the application.
//

#include <GL/glew.h>  // Include GLEW before GLFW or any other OpenGL headers
#include <GLFW/glfw3.h>
#include <iostream>

float playerX = 0.0f;
float playerY = 0.0f;

// Draw the player
void drawPlayer()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex2f(playerX, playerY);
	glEnd();
}

// Move the player
void movePlayer(int key)
{
    switch (key) 
    {
	    case GLFW_KEY_W:
            playerY += 0.1f;
		    break;
	    case GLFW_KEY_S:
		    playerY -= 0.1f;
		    break;
	    case GLFW_KEY_A:
		    playerX -= 0.1f;
		    break;
	    case GLFW_KEY_D:
		    playerX += 0.1f;
		    break;
	}
}

// Callback function for key presses
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) 
	{
		movePlayer(key);
	}
}

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "KoffeeKrew-Raycaster", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Enable modern OpenGL features
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Register the key callback function
    glfwSetKeyCallback(window, keyCallback);

    // Main application loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the player
        drawPlayer();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}