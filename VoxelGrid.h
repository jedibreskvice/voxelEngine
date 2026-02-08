#ifndef VOXELGRID_H
#define VOXELGRID_H

#include <string>
#include "Shader.h"

void generateWorld(Shader& shader, unsigned int mapSize, unsigned int mapDepth, const std::string& mapType, float potency, float level);
void generateWorldSuperflat(unsigned int mapSize, unsigned int mapDepth);
void generateWorldPyramid(unsigned int mapSize, unsigned int mapDepth, float potency);
void generateWorldHill(unsigned int mapSize, unsigned int mapDepth, float potency);
void generateWorldPlateau(unsigned int mapSize, unsigned int mapDepth, float potency);
void generateWorldHillyFlats(unsigned int mapSize, unsigned int mapDepth, float potency);
void setup();

#endif 
