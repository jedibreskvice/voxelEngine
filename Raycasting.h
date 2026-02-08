
#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <glm/glm.hpp>

// Calculates the direction of the ray from the camera through the mouse position
glm::vec3 getRayDirection(double mouseX, double mouseY, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int windowWidth, int windowHeight);


// Checks if a ray intersects an axis-aligned bounding box (AABB)
bool rayIntersectsVoxel(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 voxelMin, glm::vec3 voxelMax);

#endif 