﻿#include "KoffeeKrew-Raycaster.h"
#include <iostream>

// Window variables
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// Global time variables
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

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
void processInput(GLFWwindow* window, Player& player, const Map& map, Renderer& renderer) 
{
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.move(moveSpeed, map, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.move(moveSpeed, map, -1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.rotate(-rotateSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.rotate(rotateSpeed);
    }

    // Toggle map rendering mode (2D or 3D)
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

// Main application loop
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
    Player player(150.0f, 150.0f, PI / 4);
    Raycaster raycaster;
    Renderer renderer;
    Texture texture;

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