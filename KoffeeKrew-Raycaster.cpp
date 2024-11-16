// KoffeeKrew-Raycaster.cpp : Defines the entry point for the application.
//

// Include GLEW before GLFW or any other OpenGL headers
#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#define PI 3.14159265358979323846

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
    // Map grid coordinates
    int gridX; 
    int gridY;

    // Tile position on the screen in pixels
    int tileScreenX; 
    int tileScreenY;

    // Loop through map rows
    for (gridY = 0; gridY < mapYUnits; gridY++)
    {
        // Loop through map columns
        for (gridX = 0; gridX < mapXUnits; gridX++)
        {
            // Set tile color based on map value (1 = wall, 0 = empty space)
            if (map[gridY * mapXUnits + gridX] == 1)
            {
                // Black for walls
                glColor3f(0.0f, 0.0f, 0.0f);
            }
            else
            {
                // White for empty spaces
                glColor3f(1.0f, 1.0f, 1.0f);
            }

            // Convert grid coordinates to screen coordinates
            tileScreenX = gridX * mapUnitSize;
            tileScreenY = gridY * mapUnitSize;

            // Draw the tile as a quad
            glBegin(GL_QUADS);
            // Top-left
            glVertex2i(tileScreenX + 1, tileScreenY + 1);     
            // Bottom-left
            glVertex2i(tileScreenX + 1, tileScreenY + mapUnitSize - 1);         
            // Bottom-right
            glVertex2i(tileScreenX + mapUnitSize - 1, tileScreenY + mapUnitSize - 1);    
            // Top-right
            glVertex2i(tileScreenX + mapUnitSize - 1, tileScreenY + 1);                     
            glEnd();
        }
    }
}

// Player position
float playerX = 0.0f;
float playerY = 0.0f;
float playerAngle = 0.0f;
float playerDeltaX = 0.0f;
float playerDeltaY = 0.0f;

// Draw the player
void drawPlayer()
{
    // Player body
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex2f(playerX, playerY);
	glEnd();

    // Player direction
    glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(playerX, playerY);
	glVertex2f(playerX + playerDeltaX * 10, playerY + playerDeltaY * 10);
	glEnd();
}

// Move the player
void movePlayer(int key)
{
    switch (key) 
    {
	    case GLFW_KEY_W:
            playerX += playerDeltaX;
            playerY += playerDeltaY;
		    break;
	    case GLFW_KEY_S:
            playerX -= playerDeltaX;
            playerY -= playerDeltaY;
		    break;
	    case GLFW_KEY_A:
            playerAngle -= 0.1f;
            if (playerAngle < 0)
            {
                playerAngle += 2 * PI;
            }
            playerDeltaX = cos(playerAngle) * 5;
            playerDeltaY = sin(playerAngle) * 5;
		    break;
	    case GLFW_KEY_D:
            playerAngle += 0.1f;
            if (playerAngle > 0)
            {
                playerAngle -= 2 * PI;
            }
            playerDeltaX = cos(playerAngle) * 5;
            playerDeltaY = sin(playerAngle) * 5;
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
    // Map 0,0 to top-left and 800,600 to bottom-right
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set player starting position
    playerX = 100; 
    playerY = 100;

    // Set player starting direction
    playerDeltaX = cos(playerAngle) * 5;
    playerDeltaY = sin(playerAngle) * 5;
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