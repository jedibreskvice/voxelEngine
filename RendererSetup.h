#ifndef RENDERER_SETUP_H
#define RENDERER_SETUP_H

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

extern struct Voxel {
    float x, y, z;
    std::string texture;
};

extern struct Face {
    float x, y, z;
    std::string texture;
    uint8_t face;
};

extern std::vector<Voxel> allVoxels;
extern std::vector<Face> outsideVoxels;
extern std::vector<glm::vec3> topVoxels;

extern float vertices[];
extern unsigned int voxelVAO, voxelVBO, voxelEBO, grassVoxelTexture, waterVAO, waterVBO, waterEBO;

void setupBuffersAndTextures();
void drawVoxel(float x, float y, float z, const std::string& texture, GLuint shaderProgram);
void drawWater(float level, GLuint shaderProgram);
void planVoxel(float x, float y, float z, const std::string& texture, GLuint shaderProgram);
void checkFaces(const std::vector<Voxel>& allVoxels);  
void printFaces(const std::vector<Face>& outsideVoxels);
void drawFaces(const std::vector<Face>& outsideVoxels, GLuint shaderProgram);
void updateHighlightedVoxels(std::vector<Face>& outsideVoxels, std::vector<glm::vec3> newHighlightedVoxels);
void deleteBuffers();

#endif