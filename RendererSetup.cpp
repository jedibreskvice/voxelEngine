#include "RendererSetup.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "RendererSetup.h"
#include <bitset>
#include <tuple>
#include <iostream>
#include <set>

std::vector<Voxel> allVoxels;
std::vector<Face> outsideVoxels;

// Vertex data (positions and texture coordinates)
float vertices[] = {
    // Positions         // Texture Coords  // Normals
    // Front face
    -0.5f, -0.125f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-left
     0.5f, -0.125f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-right
     0.5f,  0.125f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // Top-right
    -0.5f,  0.125f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // Top-left

    // Back face
    -0.5f, -0.125f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Bottom-left
     0.5f, -0.125f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Bottom-right
     0.5f,  0.125f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // Top-right
    -0.5f,  0.125f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f, // Top-left

    // Left face
    -0.5f, -0.125f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f, // Bottom-left
    -0.5f, -0.125f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f, // Bottom-right
    -0.5f,  0.125f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, // Top-right
    -0.5f,  0.125f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f, // Top-left

    // Right face
     0.5f, -0.125f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom-left
     0.5f, -0.125f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom-right
     0.5f,  0.125f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Top-right
     0.5f,  0.125f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Top-left

     // Top face
     -0.5f,  0.125f, -0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-left
      0.5f,  0.125f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-right
      0.5f,  0.125f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Top-right
     -0.5f,  0.125f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Top-left

     // Bottom face
     -0.5f, -0.125f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f, // Bottom-left
      0.5f, -0.125f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f, // Bottom-right
      0.5f, -0.125f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, // Top-right
     -0.5f, -0.125f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f  // Top-left
};

// Indices for each face
unsigned int indices[] = {
    // Front face
    0, 3, 2, 2, 1, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Left face
    8, 11, 10, 10, 9, 8,
    // Right face
    12, 13, 14, 14, 15, 12,
    // Top face
    16, 17, 18, 18, 19, 16,
    // Bottom face
    20, 23, 22, 22, 21, 20
};

unsigned int indicesFRONT[] = {
    // Front face
    0, 3, 2, 2, 1, 0,
};
unsigned int indicesBACK[] = {
    // Back face
    4, 5, 6, 6, 7, 4,
};
unsigned int indicesLEFT[] = {
    // Left face
    8, 11, 10, 10, 9, 8,
};
unsigned int indicesRIGHT[] = {
    // Right face
    12, 13, 14, 14, 15, 12,
};
unsigned int indicesUP[] = {
    // Top face
    16, 17, 18, 18, 19, 16,
};
unsigned int indicesDOWN[] = {
    // Bottom face
    20, 23, 22, 22, 21, 20
};

float cloudBottomVertices[] = {
    // Positions         // Texture Coords  // Normals
    // Front face
    -0.5f, -0.0625f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-left
     0.5f, -0.0625f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom-right
     0.5f,  0.0625f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // Top-right
    -0.5f,  0.0625f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // Top-left

    // Back face
    -0.5f, -0.0625f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Bottom-left
     0.5f, -0.0625f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Bottom-right
     0.5f,  0.0625f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // Top-right
    -0.5f,  0.0625f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f, // Top-left

    // Left face
    -0.5f, -0.0625f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f, // Bottom-left
    -0.5f, -0.0625f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f, // Bottom-right
    -0.5f,  0.0625f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, // Top-right
    -0.5f,  0.0625f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f, // Top-left

    // Right face
     0.5f, -0.0625f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom-left
     0.5f, -0.0625f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom-right
     0.5f,  0.0625f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Top-right
     0.5f,  0.0625f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Top-left

     // Top face
     -0.5f,  0.0625f, -0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-left
      0.5f,  0.0625f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-right
      0.5f,  0.0625f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Top-right
     -0.5f,  0.0625f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Top-left

     // Bottom face
     -0.5f, -0.0625f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f, // Bottom-left
      0.5f, -0.0625f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f, // Bottom-right
      0.5f, -0.0625f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, // Top-right
     -0.5f, -0.0625f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f  // Top-left
};

// Indices remain the same since the geometry structure didn't change
unsigned int cloudBottomIndices[] = {
    // Front face
    0, 3, 2, 2, 1, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Left face
    8, 11, 10, 10, 9, 8,
    // Right face
    12, 13, 14, 14, 15, 12,
    // Top face
    16, 17, 18, 18, 19, 16,
    // Bottom face
    20, 23, 22, 22, 21, 20
};

float waterVertices[] = {
    // Positions            // Texture Coords  // Normals
    -10000.0f, -0.125f, -10000.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-left
     10000.0f, -0.125f, -10000.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom-right
     10000.0f, -0.125f,  10000.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Top-right
    -10000.0f, -0.125f,  10000.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Top-left
};

unsigned int waterIndices[] = {
    0, 1, 2, 2, 3, 0
};

unsigned int voxelVAO, voxelVBO, voxelEBO, grassVoxelTexture, highlightedGrassVoxelTexture, waterVAO, waterVBO, waterEBO, voxelEBO_FRONT, voxelEBO_BACK, voxelEBO_LEFT, voxelEBO_RIGHT, voxelEBO_UP, voxelEBO_DOWN;

void setupBuffersAndTextures() {
    // Initialize voxel buffers
    glGenVertexArrays(1, &voxelVAO);
    glGenBuffers(1, &voxelVBO);
    glGenBuffers(1, &voxelEBO);
    glGenBuffers(1, &voxelEBO_FRONT);
    glGenBuffers(1, &voxelEBO_BACK);
    glGenBuffers(1, &voxelEBO_LEFT);
    glGenBuffers(1, &voxelEBO_RIGHT);
    glGenBuffers(1, &voxelEBO_UP);
    glGenBuffers(1, &voxelEBO_DOWN);

    glBindVertexArray(voxelVAO);

    // Load vertex data for the voxels
    glBindBuffer(GL_ARRAY_BUFFER, voxelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Load index data for the voxels
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_FRONT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesFRONT), indicesFRONT, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_BACK);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBACK), indicesBACK, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_LEFT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLEFT), indicesLEFT, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_RIGHT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRIGHT), indicesRIGHT, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_UP);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesUP), indicesUP, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_DOWN);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesDOWN), indicesDOWN, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind the voxel VAO after setting it up
    glBindVertexArray(0);

    // Initialize water buffers
    glGenVertexArrays(1, &waterVAO);
    glGenBuffers(1, &waterVBO);
    glGenBuffers(1, &waterEBO);

    glBindVertexArray(waterVAO);

    // Load vertex data for the water surface
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

    // Load index data for the water surface
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(waterIndices), waterIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind the water VAO after setting it up
    glBindVertexArray(0);

    // Load and set up texture for grass voxels
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &grassVoxelTexture);
    glBindTexture(GL_TEXTURE_2D, grassVoxelTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/textures/grasstile.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Load and set up texture for grass voxels
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &highlightedGrassVoxelTexture);
    glBindTexture(GL_TEXTURE_2D, highlightedGrassVoxelTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("resources/textures/higlightedGrasstile.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void drawWater(float level, GLuint shaderProgram) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, level, 0.0f));

    glUseProgram(shaderProgram);

    // Set the model matrix uniform
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Enable the water color mode
    GLuint useWaterColorLoc = glGetUniformLocation(shaderProgram, "useWaterColor");
    glUniform1i(useWaterColorLoc, true);

    // Set the water color
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "waterColor");
    glUniform3f(colorLoc,0.0f, 0.0f, 0.5f); 

    // Draw the water
    glBindVertexArray(waterVAO); // Bind water VAO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind VAO

    // Disable the water color mode after rendering
    glUniform1i(useWaterColorLoc, false);
}

void planVoxel(float x, float y, float z, const std::string& texture, GLuint shaderProgram) {
    allVoxels.push_back({ x, y, z, texture });
}

void checkFaces(const std::vector<Voxel>& allVoxels) {

    std::set<std::tuple<float, float, float>> voxelSet;
    for (const Voxel& voxel : allVoxels) {
        voxelSet.insert({ voxel.x, voxel.y, voxel.z });
    }

    for (const Voxel& selectedVoxel : allVoxels) {
        uint8_t faceFlags = 0;

        // Bottom face
        /*if (voxelSet.find({selectedVoxel.x, selectedVoxel.y - 0.25f, selectedVoxel.z}) == voxelSet.end()) {
            faceFlags |= 1; 
        }*/
        // Top face
        if (voxelSet.find({ selectedVoxel.x, selectedVoxel.y + 0.25f, selectedVoxel.z }) == voxelSet.end()) {
            faceFlags |= 2; 
        }
        // Right face
        if (voxelSet.find({ selectedVoxel.x + 1.0f, selectedVoxel.y, selectedVoxel.z }) == voxelSet.end()) {
            faceFlags |= 4; 
        }
        // Left face
        if (voxelSet.find({ selectedVoxel.x - 1.0f, selectedVoxel.y, selectedVoxel.z }) == voxelSet.end()) {
            faceFlags |= 8; 
        }
        // Front face
        if (voxelSet.find({ selectedVoxel.x, selectedVoxel.y, selectedVoxel.z + 1.0f }) == voxelSet.end()) {
            faceFlags |= 16; 
        }
        // Back face
        if (voxelSet.find({ selectedVoxel.x, selectedVoxel.y, selectedVoxel.z - 1.0f }) == voxelSet.end()) {
            faceFlags |= 32; 
        }
        if (faceFlags > 0) {
            outsideVoxels.push_back({ selectedVoxel.x, selectedVoxel.y, selectedVoxel.z, selectedVoxel.texture, faceFlags });
        }
    }
}


void printFaces(const std::vector<Face>& outsideVoxels) {
    for (const Face& face : outsideVoxels) {
        std::cout << "Face:" << std::endl;
        std::cout << "  Position: (" << face.x << ", " << face.y << ", " << face.z << ")" << std::endl;
        std::cout << "  Texture: " << face.texture << std::endl;
        std::bitset<6> binary(face.face); 
        std::cout << "  Face Flags: " << binary << std::endl;
    }
}

std::vector<glm::vec3> topVoxels;

void updateHighlightedVoxels(std::vector<Face>& outsideVoxels, std::vector<glm::vec3> newHighlightedVoxels) {
    for (auto& outsideVoxel : outsideVoxels) {
        outsideVoxel.texture = "grassVoxelTexture";
    }
    for (auto& newHighlightedVoxel : newHighlightedVoxels) {
        for (auto& outsideVoxel : outsideVoxels) {
            if (outsideVoxel.x == newHighlightedVoxel.x && outsideVoxel.y == newHighlightedVoxel.y && outsideVoxel.z == newHighlightedVoxel.z) {
                outsideVoxel.texture = "highlightedGrassVoxelTexture";
                break;  
            }
        }
    }
}

void drawFaces(const std::vector<Face>& outsideVoxels, GLuint shaderProgram) {
    for (const Face& face : outsideVoxels) {
        if (face.texture == "grassVoxelTexture") {
            glBindTexture(GL_TEXTURE_2D, grassVoxelTexture);
        }
        else if(face.texture == "highlightedGrassVoxelTexture") {
            glBindTexture(GL_TEXTURE_2D, highlightedGrassVoxelTexture);
        } 
        else {
            std::cerr << "Unknown voxel texture: " << face.texture << std::endl;
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(face.x, face.y, face.z));

        glUseProgram(shaderProgram);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(voxelVAO);

        if (face.face & 1) { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_DOWN);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        if (face.face & 2) { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_UP);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            topVoxels.push_back(glm::vec3(face.x, face.y + 0.25f, face.z));
        }
        if (face.face & 4) { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_RIGHT);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        if (face.face & 8) { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_LEFT);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        if (face.face & 16) { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_FRONT);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        if (face.face & 32) { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelEBO_BACK);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void deleteBuffers() {
    glDeleteVertexArrays(1, &voxelVAO);
    glDeleteBuffers(1, &voxelVBO);
    glDeleteBuffers(1, &voxelEBO);
    glDeleteBuffers(1, &voxelEBO_FRONT);
    glDeleteBuffers(1, &voxelEBO_BACK);
    glDeleteBuffers(1, &voxelEBO_RIGHT);
    glDeleteBuffers(1, &voxelEBO_LEFT);
    glDeleteBuffers(1, &voxelEBO_UP);
    glDeleteBuffers(1, &voxelEBO_DOWN);
}