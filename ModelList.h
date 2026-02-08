#pragma once

#include "Model.h"  

extern Shader* ourShader;

extern Model* assaultTroop;
extern Model* arrow;
extern Model* airport;
extern Model* artillery;
extern Model* battleship;
extern Model* boat;
extern Model* bomber;
extern Model* carrier;
extern Model* chemicalplant;
extern Model* colony;
extern Model* combustionPlant;
extern Model* cram;
extern Model* drone;
extern Model* droneTroop;
extern Model* factory;
extern Model* fort;
extern Model* garage;
extern Model* helicopter;
extern Model* jeep;
extern Model* jet;
extern Model* liberator;
extern Model* lifeboat;
extern Model* mine;
extern Model* mlrs;
extern Model* nuclearPlant;
extern Model* quary;
extern Model* recruitmentCenter;
extern Model* samSite;
extern Model* smeltingFacility;
extern Model* sniperTroop;
extern Model* submarine;
extern Model* tank;
extern Model* trench;
extern Model* truck;
extern Model* warehouse;
extern Model* windPlant;
extern Model* moveTo;

void InitializeModels();

void CleanupModels();