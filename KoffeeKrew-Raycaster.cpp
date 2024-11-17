// KoffeeKrew-Raycaster.cpp : Defines the entry point for the application.
//

// Include GLEW before GLFW or any other OpenGL headers
#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#define PI 3.14159265358979323846
#define NO_HIT_DISTANCE 1e30f
#define FOV (PI / 3)
#define MAX_DEPTH_OF_FIELD 8
#define PLAYER_SPEED 5.0f

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Map dimensions
const int mapXUnits = 8;
const int mapYUnits = 8;
const int mapUnitSize = 64;

// Macros for grid coordinates
#define GRID_X(worldX) ((int)(worldX) / mapUnitSize)
#define GRID_Y(worldY) ((int)(worldY) / mapUnitSize)

// Map : 0 = empty space : 1 = wall
const int map[] =
{
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 1, 1, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 1, 1, 0, 1, 1,
    1, 1, 0, 1, 1, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

// Player state
float playerX = 150.0f;
float playerY = 150.0f;
float playerAngle = PI / 4;
float playerDeltaX = cos(playerAngle);
float playerDeltaY = sin(playerAngle);

// Normalize angle to be between 0 and 2*PI
float normalizeAngle(float angle)
{
    while (angle < 0)
    {
        angle += 2 * PI;
    }
    while (angle > 2 * PI)
    {
        angle -= 2 * PI;
    }
    return angle;
}

// Adjust the OpenGL viewport to fill the window
void adjustViewport(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);

    // Update the orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Draw the 2D map
void drawMap2D()
{
    // Calculate tile width and height
    float tileWidth = (float) windowWidth / mapXUnits;
    float tileHeight = (float) windowHeight / mapYUnits;

    // Loop through map rows
    for (int gridY = 0; gridY < mapYUnits; gridY++)
    {
        // Loop through map columns
        for (int gridX = 0; gridX < mapXUnits; gridX++)
        {
            // Set tile color based on map value
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

            // Calculate tile position
            float tileX = gridX * tileWidth;
            float tileY = gridY * tileHeight;

            // Draw the tile as a quad
            glBegin(GL_QUADS);
            // Top-left
            glVertex2f(tileX + 1, tileY + 1);
            // Bottom-left
            glVertex2f(tileX + 1, tileY + tileHeight - 1);
            // Bottom-right
            glVertex2f(tileX + tileWidth - 1, tileY + tileHeight - 1);
            // Top-right
            glVertex2f(tileX + tileWidth - 1, tileY + 1);
            glEnd();
        }
    }
}

// Draw the player
void drawPlayer()
{
    // Calculate scale factors
    float scaleX = (float) windowWidth / (mapXUnits * mapUnitSize);
    float scaleY = (float) windowHeight / (mapYUnits * mapUnitSize);

    // Convert player position to screen coordinates
    float playerScreenX = playerX * scaleX;
    float playerScreenY = playerY * scaleY;

    // Player position
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(playerScreenX, playerScreenY);
    glEnd();

    // Player direction
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(playerScreenX, playerScreenY);
    glVertex2f(playerScreenX + playerDeltaX * 10 * scaleX, playerScreenY + playerDeltaY * 10 * scaleY);
    glEnd();
}

    // Cast horizontal rays
    float castRayHorizontal(float rayAngle, float& hitX, float& hitY)
    {
        rayAngle = normalizeAngle(rayAngle);
        float inverseSlope = -1.0f / tan(rayAngle);
        float rayStepX, rayStepY;
        int depthOfField = 0;

        if (rayAngle > PI)
        {
            hitY = GRID_Y(playerY) * mapUnitSize - 0.0001f;
            hitX = playerX + (playerY - hitY) * inverseSlope;
            rayStepY = -mapUnitSize;
            rayStepX = -rayStepY * inverseSlope;
        }
        else if (rayAngle < PI)
        {
            hitY = GRID_Y(playerY) * mapUnitSize + mapUnitSize;
            hitX = playerX + (playerY - hitY) * inverseSlope;
            rayStepY = mapUnitSize;
            rayStepX = -rayStepY * inverseSlope;
        }
        else
        {
            return NO_HIT_DISTANCE;
        }

        while (depthOfField < MAX_DEPTH_OF_FIELD)
        {
            int mapGridX = GRID_X(hitX);
            int mapGridY = GRID_Y(hitY);
            int mapTileIndex = mapGridY * mapXUnits + mapGridX;

            if (mapTileIndex >= 0 && mapTileIndex < mapXUnits * mapYUnits && map[mapTileIndex] == 1)
            {
                return sqrt((hitX - playerX) * (hitX - playerX) + (hitY - playerY) * (hitY - playerY));
            }

            hitX += rayStepX;
            hitY += rayStepY;
            depthOfField++;
        }
        return NO_HIT_DISTANCE;
    }

    // Cast vertical rays
    float castRayVertical(float rayAngle, float& hitX, float& hitY)
    {
        rayAngle = normalizeAngle(rayAngle);
        float slope = -tan(rayAngle);
        float rayStepX, rayStepY;
        int depthOfField = 0;

        if (rayAngle > PI / 2 && rayAngle < 3 * PI / 2)
        {
            hitX = GRID_X(playerX) * mapUnitSize - 0.0001f;
            hitY = (playerX - hitX) * slope + playerY;
            rayStepX = -mapUnitSize;
            rayStepY = -rayStepX * slope;
        }
        else if (rayAngle < PI / 2 || rayAngle > 3 * PI / 2)
        {
            hitX = GRID_X(playerX) * mapUnitSize + mapUnitSize;
            hitY = (playerX - hitX) * slope + playerY;
            rayStepX = mapUnitSize;
            rayStepY = -rayStepX * slope;
        }
        else
        {
            return NO_HIT_DISTANCE;
        }

        while (depthOfField < MAX_DEPTH_OF_FIELD)
        {
            int mapGridX = GRID_X(hitX);
            int mapGridY = GRID_Y(hitY);
            int mapTileIndex = mapGridY * mapXUnits + mapGridX;

            if (mapTileIndex >= 0 && mapTileIndex < mapXUnits * mapYUnits && map[mapTileIndex] == 1)
            {
                return sqrt((hitX - playerX) * (hitX - playerX) + (hitY - playerY) * (hitY - playerY));
            }

            hitX += rayStepX;
            hitY += rayStepY;
            depthOfField++;
        }
        return NO_HIT_DISTANCE;
    }

    // Draw wall slice
    void drawWallSlice(float x, float wallHeight, bool isVertical)
    {
        float wallTop = (windowHeight / 2.0f) - (wallHeight / 2.0f);
        float wallBottom = (windowHeight / 2.0f) + (wallHeight / 2.0f);
        float wallSliceWidth = (float) windowWidth / 60.0f;

        if (isVertical)
        {
            // Slightly darker for vertical walls
            glColor3f(0.6f, 0.6f, 0.6f); 
        }
        else
        {
            // Brighter for horizontal walls
            glColor3f(0.8f, 0.8f, 0.8f);
        }

        glBegin(GL_QUADS);
        glVertex2f(x, wallTop);
        glVertex2f(x + wallSliceWidth, wallTop);
        glVertex2f(x + wallSliceWidth, wallBottom);
        glVertex2f(x, wallBottom);
        glEnd();
    }

    // Draw rays
    void drawRays3D()
    {
        // Start ray at the left edge of FOV
        float rayAngle = playerAngle - (FOV / 2); 

        rayAngle = normalizeAngle(rayAngle);

        float sliceWidth = (float) windowWidth / 60.0f;

        for (int rayIndex = 0; rayIndex < 60; rayIndex++)
        {
            float horizontalHitX;
            float horizontalHitY; 
            float verticalHitX; 
            float verticalHitY;

            // Cast horizontal and vertical rays
            float horizontalDistance = castRayHorizontal(rayAngle, horizontalHitX, horizontalHitY);
            float verticalDistance = castRayVertical(rayAngle, verticalHitX, verticalHitY);

            // Determine the closer hit
            bool isVerticalHit = verticalDistance < horizontalDistance;
            float rayDistance = isVerticalHit ? verticalDistance : horizontalDistance;

            // Correct fisheye distortion
            rayDistance *= cos(playerAngle - rayAngle);

            // Calculate wall height
            float wallHeight = (mapUnitSize * (windowHeight / 2.0f)) / rayDistance;

            // Draw the wall slice
            drawWallSlice(rayIndex * sliceWidth, wallHeight, isVerticalHit);

            // Move to the next ray
            rayAngle += (FOV / 60.0f);
            rayAngle = normalizeAngle(rayAngle);
        }
    }

// Start in 2D map mode
bool renderMap = true;

// Callback function for key presses
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            playerX += playerDeltaX * PLAYER_SPEED;
            playerY += playerDeltaY * PLAYER_SPEED;
            break;
        case GLFW_KEY_S:
            playerX -= playerDeltaX * PLAYER_SPEED;
            playerY -= playerDeltaY * PLAYER_SPEED;
            break;
        case GLFW_KEY_A:
            playerAngle -= 0.1f;
            playerAngle = normalizeAngle(playerAngle);
            playerDeltaX = cos(playerAngle);
            playerDeltaY = sin(playerAngle);
            break;
        case GLFW_KEY_D:
            playerAngle += 0.1f;
            playerAngle = normalizeAngle(playerAngle);
            playerDeltaX = cos(playerAngle);
            playerDeltaY = sin(playerAngle);
            break;
        case GLFW_KEY_M:
            renderMap = !renderMap;
            break;
        }
    }
}

// Initialize OpenGL settings
void init()
{
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Set the clear color (gray background)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "KoffeeKrew-Raycaster", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Enable modern OpenGL features
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Register the viewport adjustment callback function
    glfwSetFramebufferSizeCallback(window, adjustViewport);
    // Register the key callback function
    glfwSetKeyCallback(window, keyCallback);

    // Initialize OpenGL settings
    init();

    int width;
    int height;
    // Get the size of the framebuffer
    glfwGetFramebufferSize(window, &width, &height);
    // Adjust the viewport to the framebuffer size
    adjustViewport(window, width, height);

    // Main application loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
        if (renderMap)
        {
            // Draw the map and player in 2D mode
            drawMap2D();
            drawPlayer();
        }
        else
        {
            // Draw the 3D walls
            drawRays3D();
        }
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