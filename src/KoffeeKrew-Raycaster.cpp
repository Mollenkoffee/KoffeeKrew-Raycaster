#include "KoffeeKrew-Raycaster.h"
#include <iostream>

// Window variables
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// Global time variables
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

// Mouse variables
double lastMouseX = 0.0;
bool firstMouseInput = true;

// Adjust OpenGL viewport when window size changes
void adjustViewport(GLFWwindow* window, int width, int height) 
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Process user input
void processInput(GLFWwindow* window, Player& player, Map& map, Renderer& renderer)
{
    static bool cursorLocked = true;
    static bool escKeyReleased = true;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && escKeyReleased)
    {
        escKeyReleased = false;
        cursorLocked = !cursorLocked;

        glfwSetInputMode(window, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        glfwSetCursorPos(window, WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        escKeyReleased = true;
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouseInput)
    {
        lastMouseX = mouseX;
        firstMouseInput = false;
    }

    double deltaX = mouseX - lastMouseX;
    lastMouseX = mouseX;

    // Sensitivity scaling
    float sensitivity = 0.0005f;
    if (cursorLocked)
    {
        player.rotate(static_cast<float>(deltaX) * sensitivity);
    }

    float moveSpeed = PLAYER_MOVE_SPEED * deltaTime;
    float rotateSpeed = PLAYER_ROTATE_SPEED * deltaTime;

    // Sprinting
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        moveSpeed *= 1.5f;
    }

    // Walking
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        moveSpeed *= 0.5f;
    }

    // Player movement
    float moveX = 0.0f;
    float moveY = 0.0f;

    // Forward/backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        moveX += player.deltaX;
        moveY += player.deltaY;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        moveX -= player.deltaX;
        moveY -= player.deltaY;
    }

    // Strafe left/right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveX += player.deltaY;
        moveY -= player.deltaX;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveX -= player.deltaY;
        moveY += player.deltaX;
    }

    // Normalize and move
    float length = sqrt(moveX * moveX + moveY * moveY);
    if (length > 0.01f)
    {
        moveX = (moveX / length) * moveSpeed;
        moveY = (moveY / length) * moveSpeed;
        player.moveAbsolute(moveX, moveY, map);
    }
    
	// Directional key movement
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player.move(moveSpeed, map, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player.move(moveSpeed, map, -1);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        player.rotate(-rotateSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player.rotate(rotateSpeed);
    }

    // Door interaction
    static bool doorKeyReleased = true;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && doorKeyReleased)
    {
        int playerTileX = static_cast<int>(player.x) / Map::mapUnitSize;
        int playerTileY = static_cast<int>(player.y) / Map::mapUnitSize;
        int currentTile = map.getTile(playerTileX, playerTileY);
        
        // Prevent door interaction while standing inside an open door
        if (currentTile == Map::TileType::DOOR_OPEN)
        {
            return;
        }
        
        float maxInteractDist = 32.0f;
        float rayStepDistance = 2.0f;
        float raySampleX = player.x;
        float raySampleY = player.y;

        // Check for ineract distance and player's facing direction
        for (float dist = 0.0f; dist <= maxInteractDist; dist += rayStepDistance)
        {
            raySampleX = player.x + player.deltaX * dist;
            raySampleY = player.y + player.deltaY * dist;

            int tileX = static_cast<int>(raySampleX) / Map::mapUnitSize;
            int tileY = static_cast<int>(raySampleY) / Map::mapUnitSize;

            int tile = map.getTile(tileX, tileY);

            if (tile == Map::TileType::WALL)
            {
                break;
            }

            if (tile == Map::TileType::DOOR_CLOSED || tile == Map::TileType::DOOR_OPEN)
            {
                map.toggleDoor(tileX, tileY);
                break;
            }
        }
        doorKeyReleased = false;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
    {
        doorKeyReleased = true;
    }

    // Toggle map rendering mode
    static bool lastToggleState = GLFW_RELEASE;
    int currentToggleState = glfwGetKey(window, GLFW_KEY_M);

    if (currentToggleState == GLFW_PRESS && lastToggleState == GLFW_RELEASE)
    {
        renderer.toggleRenderMap();
    }

    lastToggleState = currentToggleState;
}

// Initialize OpenGL
void initOpenGL() 
{
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Driver
int main() 
{
    // Initialize GLFW
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "KoffeeKrew-Raycaster", nullptr, nullptr);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    else
    {
        std::cerr << "Raw mouse motion not supported!" << std::endl;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, adjustViewport);
    initOpenGL();

    glfwSetTime(0.0);
    lastFrameTime = glfwGetTime();

    // Initialize game objects
    Map map;
    Player player((Map::mapXUnits / 2.0f) * Map::mapUnitSize, (Map::mapYUnits / 2.0f) * Map::mapUnitSize, 3 * PI / 2);
    Raycaster raycaster;
    Renderer renderer;
    Texture texture;

    texture.loadFromFile(TEX_FILE_WALL, "n64guy-CC0-textures/Rock 3.png");
    texture.loadFromFile(TEX_FILE_DOOR_CLOSED, "n64guy-CC0-textures/Planks.png");
    texture.loadFromFile(TEX_FILE_CEILING, "n64guy-CC0-textures/Rock 5.png");
    texture.loadFromFile(TEX_FILE_FLOOR, "n64guy-CC0-textures/Dirt.png");

    // Main application loop
    while (!glfwWindowShouldClose(window)) 
    {
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        processInput(window, player, map, renderer);

        glClear(GL_COLOR_BUFFER_BIT);

        renderer.render(map, player, raycaster, WINDOW_WIDTH, WINDOW_HEIGHT, texture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and terminate
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}