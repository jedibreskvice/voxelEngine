#include "Raycasting.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

// Calculates the direction of the ray from the camera through the mouse position
glm::vec3 getRayDirection(double mouseX, double mouseY, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int windowWidth, int windowHeight) {
    // Convert mouse coordinates to normalized device coordinates
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;
    float z = 1.0f;
    glm::vec4 rayNDC(x, y, z, 1.0f);

    // Convert NDC to clip space
    glm::vec4 rayClip = rayNDC;

    // Convert clip space to eye space
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // Convert eye space to world space
    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(viewMatrix) * rayEye));
    return rayWorld;
}

// Checks if a ray intersects an axis-aligned bounding box (AABB)
bool rayIntersectsVoxel(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 voxelMin, glm::vec3 voxelMax) {
    float tmin = (voxelMin.x - rayOrigin.x) / rayDir.x;
    float tmax = (voxelMax.x - rayOrigin.x) / rayDir.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (voxelMin.y - rayOrigin.y) / rayDir.y;
    float tymax = (voxelMax.y - rayOrigin.y) / rayDir.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (voxelMin.z - rayOrigin.z) / rayDir.z;
    float tzmax = (voxelMax.z - rayOrigin.z) / rayDir.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    return true;
}