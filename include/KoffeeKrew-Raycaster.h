#ifndef KOFFEEKREW_RAYCASTER_H
#define KOFFEEKREW_RAYCASTER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Map.h"
#include "Player.h"
#include "Renderer.h"
#include "Raycaster.h"
#include "Constants.h"

void adjustViewport(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Player& player, const Map& map, Renderer& renderer);
void initOpenGL();

#endif // KOFFEEKREW_RAYCASTER_H