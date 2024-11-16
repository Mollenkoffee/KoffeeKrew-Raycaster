// KoffeeKrew-Raycaster.cpp : Defines the entry point for the application.
//

// Include GLEW before GLFW or any other OpenGL headers
#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <iostream>


// Map dimensions
int mapXUnits = 8;
int mapYUnits = 8;
int mapUnitSize = 64;

// Map : 0 = empty space : 1 = wall
int map[]=
{
    1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

void drawMap2D()
{
    int x;
    int y;
    int xo;
    int yo;

	for (y = 0; y < mapYUnits; y++)
	{
		for (x = 0; x < mapXUnits; x++)
		{
			if (map[y * mapXUnits + x] == 1)
			{
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			else
			{
				glColor3f(1.0f, 1.0f, 1.0f);
			}

			xo = x * mapUnitSize;
            yo = y * mapUnitSize;

            glBegin(GL_QUADS);
            glVertex2i(xo + 1, yo + 1);                             
            glVertex2i(xo + 1, yo + mapUnitSize - 1);               
            glVertex2i(xo + mapUnitSize - 1, yo + mapUnitSize - 1); 
            glVertex2i(xo + mapUnitSize - 1, yo + 1);               
            glEnd();
            
		}
	}
}

// Player position
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

// Player movement speed
float playerMovementSpeed = 10.0f;

// Move the player
void movePlayer(int key)
{
    switch (key) 
    {
	    case GLFW_KEY_W:
            playerY -= playerMovementSpeed;
		    break;
	    case GLFW_KEY_S:
		    playerY += playerMovementSpeed;
		    break;
	    case GLFW_KEY_A:
		    playerX -= playerMovementSpeed;
		    break;
	    case GLFW_KEY_D:
		    playerX += playerMovementSpeed;
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

void init()
{
    // Set the clear color
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set a gray background

    // Set up the viewport and projection to match pixel dimensions
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1); // Map 0,0 to top-left and 800,600 to bottom-right
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set player starting position
    playerX = 100; 
    playerY = 100;
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

    // Initialize OpenGL settings
    init();

    // Register the key callback function
    glfwSetKeyCallback(window, keyCallback);

    // Main application loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the map
        drawMap2D();

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