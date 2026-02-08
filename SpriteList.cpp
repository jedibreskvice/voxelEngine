#include "SpriteList.h"
#include <algorithm>
#include <iostream>
#include "model_transformations.h"
#include "ModelList.h"
#include "Shader.h"
#include "RendererSetup.h"
#include <set>
#include "UIManager.h"
#include <chrono>
#include <cmath>

std::vector<Sprite> allSprites;
std::vector<Sprite> allSpritesInView;
std::vector<Sprite> allSpritesBeingRemoved;
std::vector<Faction> allFactions;
std::vector<Faction> allFactionsInView;
std::vector<VoxiPoxy> confirmedVoxels;
std::vector<glm::vec3> validTilesToMoveTo;
std::vector<glm::vec3> highlightedVoxels;
std::vector<glm::vec3> attackHighlightedVoxels;
std::vector<VehicleContinents> vehicleContinentsList;

//limit for all the model classes per tile. Limit of 0 means there is no limit
unsigned int vehicleCap = 1;
unsigned int buildingCap = 1;
unsigned int defenseCap = 1;
unsigned int troopCap = 0;
unsigned int antiClipPos;
unsigned int maximumTraversableHeight = 2;
float movementPenaltyPerHeightLevel = 0.5;
unsigned int visionObscuredAtDeltaY = 3;

unitStat unitStatistics[] = {
    {5, true, false, false, 10, 0, 1, 10, true, 0, false, 10, 10, false}, //assaultTroop
    {1, true, false, false, 10, 0, 2, 10, false, 0, false, 10, 10, false}, //sniperTroops
    {1, true, false, false, 0, 0, 3, 10, false, 0, false, 10, 10, true},  //droneTroop
    {2, false, false, false, 40, 0, 1, 10, true, 10, false, 20, 40, false}, //jeep
    {2, false, false, true, 0, 0, 0, 20, false, 20, false, 20, 0, false}, //truck
    {1, false, true, false, 0, 80, 1, 40, true, 10, false, 20, 80, false},  //liberator
    {1, true, false, false, 0, 240, 1, 40, false, 10, true, 40, 80, true}, //tank
    {1, true, false, false, 0, 200, 0, 10, false, 5, false, 0, 120, true}, //artillery
    {1, true, false, false, 0, 80, 2, 10, false, 10, false, 0, 40, true}  //mlrs
};

glm::vec3 randomTroopPositions[] = {
    glm::vec3(0.15f, 0.0f, 0.0f),
    glm::vec3(-0.15f, 0.0f, 0.0f),
    glm::vec3(0.27f, 0.0f, 0.0f),
    glm::vec3(0.21f, 0.0f, 0.1f),
    glm::vec3(-0.39f, 0.0f, 0.1f),
    glm::vec3(0.33f, 0.0f, 0.1f),
    glm::vec3(0.15f, 0.0f, -0.1f),
    glm::vec3(-0.27f, 0.0f, -0.1f),
    glm::vec3(0.39f, 0.0f, 0.2f),
    glm::vec3(-0.09f, 0.0f, 0.2f),
    glm::vec3(-0.21f, 0.0f, 0.2f),
    glm::vec3(0.03f, 0.0f, -0.2f),
    glm::vec3(-0.39f, 0.0f, -0.2f),
    glm::vec3(0.15f, 0.0f, 0.2f),
    glm::vec3(0.21f, 0.0f, -0.1f),
    glm::vec3(0.33f, 0.0f, -0.2f),
    glm::vec3(-0.15f, 0.0f, -0.2f),
    glm::vec3(-0.15f, 0.0f, -0.1f),
    glm::vec3(-0.39f, 0.0f, -0.1f),
    glm::vec3(-0.21f, 0.0f, 0.1f)
};

size_t numberOfAvailablePositions = sizeof(randomTroopPositions) / sizeof(randomTroopPositions[0]);

float checkYlevel(int x, int z, std::vector<glm::vec3> topVoxels) {
    for (auto& topVoxel : topVoxels) {
        if (topVoxel.x == x && topVoxel.z == z) {
            return topVoxel.y;
        }
    }
    return 0.01;
}

void addHighlightedVoxelsVector(int x, float y, int z, float range, std::vector<glm::vec3> topVoxels) {
    for (unsigned int i = 0; i < range; i++) {
        for (unsigned int j = 0; j < range; j++) {
            float X, Y, Z;
            X = x - ((range / 2) - i - 0.5f);
            //std::cout << X << " ";
            Z = z - ((range / 2) - j - 0.5f);
            //std::cout << Z << " ";
            Y = checkYlevel(X, Z, topVoxels) - 0.25f;
            //std::cout << Y << " " << std::endl;
            if (Y != 0.01 /* && ((abs(Y - y) < (visionObscuredAtDeltaY / 4)*/) {
                bool exists = false;
                for (auto& voxel : highlightedVoxels) {
                    if ((voxel.x == X && voxel.y == Y && voxel.z == Z)) {
                        exists = true;
                    }
                }
                if (!exists) {
                    highlightedVoxels.push_back(glm::vec3(X, Y, Z));
                    //determine which sprites should be visible
                    for (auto& sprite : allSprites) {
                        if (sprite.x == X && sprite.y == (Y + 0.25f) && sprite.z == Z ) {
                            allSpritesInView.push_back(sprite);
                        }
                    }
                }
            }
        }
    }
    updateHighlightedVoxels(outsideVoxels, highlightedVoxels);
}

int unitIDtoStat(int modelID) {
    if (modelID == 1) {
        return 0;
    }
    else if (modelID == 14) {
        return 2;
    }
    else if (modelID == 30) {
        return 1;
    }
    else if (modelID == 20) {
        return 3;
    }
    else if (modelID == 34) {
        return 4;
    }
    else if (modelID == 22) {
        return 5;
    }
    else if (modelID == 32) {
        return 6;
    }
    else if (modelID == 4) {
        return 7;
    }
    else if (modelID == 25) {
        return 8;
    }
}

void recalculateAllVision(std::vector<Faction> allFactions, std::vector<glm::vec3> topVoxels, int team) {
    highlightedVoxels.clear();
    if (team == 0) {
        updateHighlightedVoxels(outsideVoxels, highlightedVoxels);
    }
    else {
        highlightedVoxels.clear();
        allSpritesInView.clear();
        allFactionsInView.clear();
        for (auto& faction : allFactions) {
            if (team == faction.team) {
                addHighlightedVoxelsVector(faction.x, faction.y, faction.z, 3, topVoxels);
                allFactionsInView.push_back(faction);
            } 
        }
    }
}

bool validityCHeck(int x, float y, int z, int modelID, int team) {
    if (modelID == 1 || modelID == 14 || modelID == 30) {
        int unitCount = 0;
        for (const auto& sprite : allSprites) {
            if (sprite.x == x && sprite.y == y && sprite.z == z) {
                if (sprite.team == team || sprite.team == 0) {
                    if ((sprite.modelID == 1 || sprite.modelID == 14 || sprite.modelID == 30)) {
                        unitCount++;
                    }
                }
                else {
                    std::cout << "Cannot add a troop to the tile controled by the enemy" << std::endl;
                    return false;
                }
            }
        }
        if (troopCap == 0) {
            antiClipPos = unitCount;
            return true;
        }
        else
        if (unitCount >= troopCap) {
            std::cout << "Troop limit on this tile has been reached" << std::endl;
        }
        else {
            antiClipPos = unitCount;
            return true;
        }
    } else if (modelID == 20 || modelID == 34 || modelID == 22 || modelID == 32 || modelID == 4 || modelID == 25) {
        int unitCount = 0;
        for (const auto& sprite : allSprites) {
            if (sprite.x == x && sprite.y == y && sprite.z == z) {
                if (sprite.team == team) {
                    if ((sprite.modelID == 20 || sprite.modelID == 34 || sprite.modelID == 22 || sprite.modelID == 32 || sprite.modelID == 4 || sprite.modelID == 25)) {
                        unitCount++;
                    }
                }
                else {
                    std::cout << "Cannot add a vehicle to the tile controled by the enemy" << std::endl;
                    return false;
                }
            }
        }
        if (vehicleCap == 0) {

            antiClipPos = unitCount;
            return true;
        }
        else
        if (unitCount >= vehicleCap) {
            std::cout << "Vehicle limit on this tile has been reached" << std::endl;
        }
        else {
            antiClipPos = unitCount;
            return true;
        }
    } else if (modelID == 19 || modelID == 24 || modelID == 27 || modelID == 17 || modelID == 15 || modelID == 3 || modelID == 11 || modelID == 26 || modelID == 36 || modelID == 9 || modelID == 29 || modelID == 35) {

        int unitCount = 0;
        for (const auto& sprite : allSprites) {
            if (sprite.x == x && sprite.y == y && sprite.z == z) {
                if (sprite.team == team) {
                    if (sprite.modelID == 19 || sprite.modelID == 24 || sprite.modelID == 27 || sprite.modelID == 17 ||
                        sprite.modelID == 15 || sprite.modelID == 3 || sprite.modelID == 11 || sprite.modelID == 26 ||
                        sprite.modelID == 36 || sprite.modelID == 9 || sprite.modelID == 29 || sprite.modelID == 35) {
                        unitCount++;
                    }
                }
                else {
                    std::cout << "Cannot add a buidling to the tile controlled by the enemy" << std::endl;
                    return false;
                }
            }
        }
        if (buildingCap == 0) {
            antiClipPos = unitCount;
            return true;
        }
        else
        if (unitCount >= buildingCap) {
            std::cout << "Building limit on this tile has been reached" << std::endl;
        }
        else {
            antiClipPos = unitCount;
            return true;
        }
    }
    else if (modelID == 33 || modelID == 16 || modelID == 28 || modelID == 12) {

        int unitCount = 0;
        for (const auto& sprite : allSprites) {
            if (sprite.x == x && sprite.y == y && sprite.z == z) {
                if (sprite.team == team) {
                    if (sprite.modelID == 33 || sprite.modelID == 16 ||
                        sprite.modelID == 28 || sprite.modelID == 12) {
                        unitCount++;
                    }
                }
                else {
                    std::cout << "Cannot add a defense to the tile controlled by the enemy" << std::endl;
                    return false;
                }
            }
        }
        if (defenseCap == 0) {
            antiClipPos = unitCount;
            return true;
        } else
        if (unitCount >= defenseCap) {
            std::cout << "Defense limit on this tile has been reached" << std::endl;
        }
        else {
            antiClipPos = unitCount;
            return true;
        }
    }
}

void movementValidityCheck(int x, float y, int z) {
    int baseMovement = 100;
    for (auto& sprite : allSprites) {
        if (sprite.selected) {
            if (sprite.movementRemaining < baseMovement) {
                baseMovement = sprite.movementRemaining;
            }
        }
    }
    confirmedVoxels.clear();
    confirmedVoxels.shrink_to_fit();
    if (baseMovement > 0) {
        for (auto& voxel : topVoxels) {
            int deltaY = (y * 4) - (voxel.y * 4);
            if (deltaY < 0) {
                deltaY = (deltaY * -1);
            }
            if (deltaY < maximumTraversableHeight + 1) {
                bool notSameVoxel = true;
                if ((voxel.x == x && voxel.z == z)) {
                    notSameVoxel = false;
                }
                if (voxel.x < (x + 2) && voxel.x > (x - 2) && voxel.z < (z + 2) && voxel.z > (z - 2) && notSameVoxel) {
                    float moveSubtracted = (1 + deltaY * movementPenaltyPerHeightLevel);
                    confirmedVoxels.push_back({ voxel, moveSubtracted });
                }
            }
        }
    }
}

void resetMovementValues() {
    for (auto& sprite : allSprites) {
        sprite.movementRemaining = unitStatistics[unitIDtoStat(sprite.modelID)].movement;
    }
}

void printConfirmedVoxels() {
    for (const auto& voxel : confirmedVoxels) {
        std::cout << "Position: ("
            << voxel.position.x << ", "
            << voxel.position.y << ", "
            << voxel.position.z << "), "
            << "Movement Subtracted: " << voxel.movementSubtracted << std::endl;
    }
}

unsigned int declareNewFaction(int team) {
    if (team == 0) {
        return 0;
    }
    else {
        unsigned int factionNum = 1;
        bool foundDuplicate = false;

        while (true) {
            foundDuplicate = false;
            for (const auto& faction : allFactions) {
                if (faction.factionID == factionNum && faction.team == team) {
                    foundDuplicate = true;
                    break;
                }
            }
            if (!foundDuplicate) {
                return factionNum;
            }
            else {
                factionNum++;
            }
        }
    }
}

void addSprite(int x, float y, int z, int modelID, int team) {
    if (validityCHeck(x, y, z, modelID, team)) {
        
        if (allSprites.size() > 0) {
            bool factionAlreadyExists = false;
            unsigned int existingFaction;
            for (const auto& sprite : allSprites) {
                if (sprite.x == x && sprite.y == y && sprite.z == z) {
                    factionAlreadyExists = true;
                    existingFaction = sprite.faction;
                }
            }
            if (factionAlreadyExists) {
                Sprite newSprite = { x, y, z, modelID, team, antiClipPos, existingFaction, 0, false, 0.0f, unitStatistics[unitIDtoStat(modelID)].movement};
                allSprites.push_back(newSprite);
            }
            else {
                unsigned int originalFaction = declareNewFaction(currentTeam);
                if (originalFaction != 0) {
                    Faction newFaction(x, y, z, team, originalFaction, false);
                    allFactions.push_back(newFaction);
                }
                Sprite newSprite = { x, y, z, modelID, team, antiClipPos, originalFaction, 0, false, 0.0f, unitStatistics[unitIDtoStat(modelID)].movement};
                allSprites.push_back(newSprite);
                if (team != 0) {
                    if (modelID == 20) {
                        vehicleContinentsList.push_back({ x, z, unitStatistics[3].housing, 0, 0 });
                    }
                    else if (modelID == 34) {
                        vehicleContinentsList.push_back({ x, z, unitStatistics[4].housing, 0, 0 });
                    }
                    else if (modelID == 22) {
                        vehicleContinentsList.push_back({ x, z, unitStatistics[5].housing, 0, 0 });
                    }
                    else if (modelID == 32) {
                        vehicleContinentsList.push_back({ x, z, unitStatistics[6].housing, 0, 0 });
                    }
                    else if (modelID == 4) {
                        vehicleContinentsList.push_back({ x, z, unitStatistics[7].housing, 0, 0 });
                    }
                    else if (modelID == 25) {
                        vehicleContinentsList.push_back({ x, z, unitStatistics[8].housing, 0, 0 });
                    }
                }
            }
        }
        else {
            unsigned int originalFaction = declareNewFaction(currentTeam);
            if (originalFaction != 0) {
                Faction newFaction(x, y, z, team, originalFaction, false);
                allFactions.push_back(newFaction);
            }
            Sprite newSprite = { x, y, z, modelID, team, antiClipPos, originalFaction, 0, false, 0.0f, unitStatistics[unitIDtoStat(modelID)].movement};
            allSprites.push_back(newSprite);
            if (team != 0) {
                if (modelID == 20) {
                    vehicleContinentsList.push_back({ x, z, unitStatistics[3].housing, 0, 0 });
                }
                else if (modelID == 34) {
                    vehicleContinentsList.push_back({ x, z, unitStatistics[4].housing, 0, 0 });
                }
                else if (modelID == 22) {
                    vehicleContinentsList.push_back({ x, z, unitStatistics[5].housing, 0, 0 });
                }
                else if (modelID == 32) {
                    vehicleContinentsList.push_back({ x, z, unitStatistics[6].housing, 0, 0 });
                }
                else if (modelID == 4) {
                    vehicleContinentsList.push_back({ x, z, unitStatistics[7].housing, 0, 0 });
                }
                else if (modelID == 25) {
                    vehicleContinentsList.push_back({ x, z, unitStatistics[8].housing, 0, 0 });
                }
            }
        }
    }
}

void removeSprite(std::vector<Sprite>& allSprites, const Sprite& spriteToRemove) {
    auto it = std::find(allSprites.begin(), allSprites.end(), spriteToRemove);
    if (it != allSprites.end()) {
        allSpritesBeingRemoved.push_back(spriteToRemove);
        allSprites.erase(it);
    }
    else {
        std::cout << "Sprite not found.\n";
    }
}

void printAllSprites() {
    if (allSprites.empty()) {
        std::cout << "No sprites available for printing." << std::endl;
        return;
    }

    std::cout << "List of all sprites:" << std::endl;
    for (const auto& sprite : allSprites) {
        std::cout << "X: " << sprite.x << ", Y: " << sprite.y << ", Z: " << sprite.z << ", Model ID: " << sprite.modelID << ", Team: " << sprite.team << ", Faction: " << sprite.faction << ", Previous faction: " << sprite.previousFaction << ", selected: " << sprite.selected << ", movement remaining: " << sprite.movementRemaining << std::endl;
    }
}

void changeSelectValueBySprite(unsigned int faction, bool includeVehicle, float atValue, float stValue, float dtValue, int team) {
    int currentAtCount = 0;
    int currentStCount = 0;
    int currentDtCount = 0;
    for (auto& sprite : allSprites) {
        if (sprite.faction == faction && sprite.team == team) {
            if (sprite.modelID == 1 && atValue > currentAtCount) {
                currentAtCount++;
                sprite.selected = true;
            }
            else if (sprite.modelID == 30 && stValue > currentStCount) {
                currentStCount++;
                sprite.selected = true;
            }
            else if (sprite.modelID == 14 && dtValue > currentDtCount) {
                currentDtCount++;
                sprite.selected = true;
            }
            else if (sprite.modelID == 20 || sprite.modelID == 34 || sprite.modelID == 22 || sprite.modelID == 32 || sprite.modelID == 4 || sprite.modelID == 25) {
                if (includeVehicle) {
                    sprite.selected = true;
                }
                else {
                    sprite.selected = false;
                }
            }
            else {
                sprite.selected = false;
            }
        }
    }
}

void printVehicleContinentsList() {
    if (vehicleContinentsList.empty()) {
        std::cout << "The vehicleContinentsList is empty." << std::endl;
        return;
    }
    for (const auto& vc : vehicleContinentsList) {
        std::cout << "Vehicle Continent at x: " << vc.x << ", z: " << vc.z << std::endl;
        std::cout << "  ATs: " << vc.ATs << std::endl;
        std::cout << "  STs: " << vc.STs << std::endl;
        std::cout << "  DTs: " << vc.DTs << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
}

void printFactionListInfo(const std::vector<Faction>& allFactions) {
    std::cout << "Faction List Info:" << std::endl;

    for (const auto& faction : allFactions) {
        std::cout << "Faction ID: " << faction.factionID << std::endl;
        std::cout << "Position: (" << faction.x << ", " << faction.y << ", " << faction.z << ")" << std::endl;
        std::cout << "Team: " << faction.team << std::endl;
        std::cout << "Selected By Player: " << (faction.selectedByPlayer ? "Yes" : "No") << std::endl;
        std::cout << "--------------------------" << std::endl;
    }
}

void changeSelectValuesByFaction(unsigned int selectedID) {
    for (auto& sprite : allSprites) {
        if (sprite.faction == selectedID) {
            sprite.selected = true;
        }
        else {
            sprite.selected = false;
        }
    }
}

void deselectEverything() {
    for (auto& sprite : allSprites) {
        sprite.selected = false;
    }
}

void animationWhenAdded() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    for (auto& sprite : allSprites) {
        if (sprite.transparency < 1.0f) {
            sprite.transparency += deltaTime.count() * 4; 
            if (sprite.transparency > 1.0f) {
                sprite.transparency = 1.0f;
            }
        }
    }
}

void animationWhenAddedinViewVersion() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    for (auto& sprite : allSpritesInView) {
        if (sprite.transparency < 1.0f) {
            sprite.transparency += deltaTime.count() * 4;
            if (sprite.transparency > 1.0f) {
                sprite.transparency = 1.0f;
            }
        }
    }
}

void animationWhenRemoved() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    for (auto& sprite : allSpritesBeingRemoved) {
        if (sprite.transparency > 0.0f) {
            sprite.transparency -= deltaTime.count() * 4;
            if (sprite.transparency < 0.0f) {
                sprite.transparency = 0.0f;
            }
        }
    }
}

constexpr int ATROOP = 1 << 0;  // Bit 0
constexpr int STROOP = 1 << 1;  // Bit 1
constexpr int DTROOP = 1 << 2;  // Bit 2
constexpr int JEEP = 1 << 3;  // Bit 3
constexpr int TRUCK = 1 << 4;  // Bit 4
constexpr int LIBERATOR = 1 << 5;  // Bit 5
constexpr int TANK = 1 << 6;  // Bit 6
constexpr int ARTILLERY = 1 << 7;  // Bit 7
constexpr int MLRS = 1 << 8;  // Bit 8

bool attackLogicInitialized = false;

void populateAttackHighlightedVoxels() {
    attackHighlightedVoxels.clear();
    attackHighlightedVoxels.shrink_to_fit();
    unsigned int selectedUnitsBoolint = 0;
    int selectedSpriteX;
    int selectedSpriteY;
    selectedUnitsBoolint = 0;
    bool XnYnotAssigned = true;
    for (auto& sprite : allSprites) {
        if (sprite.selected) {
            if (XnYnotAssigned) {
                selectedSpriteX = sprite.x;
                selectedSpriteY = sprite.y;
                XnYnotAssigned = false;
            }
            switch (sprite.modelID) {
            case 1:
                selectedUnitsBoolint |= ATROOP;
                break;
            case 30:
                selectedUnitsBoolint |= STROOP;
                break;
            case 16:
                selectedUnitsBoolint |= DTROOP;
                break;
            case 20:
                selectedUnitsBoolint |= JEEP;
                break;
            case 34:
                selectedUnitsBoolint |= TRUCK;
                break;
            case 22:
                selectedUnitsBoolint |= LIBERATOR;
                break;
            case 32:
                selectedUnitsBoolint |= TANK;
                break;
            case 4:
                selectedUnitsBoolint |= ARTILLERY;
                break;
            case 25:
                selectedUnitsBoolint |= MLRS;
                break;
            }
        }
    }
    if (selectedUnitsBoolint & ATROOP) {
        for (auto& sprite : allSprites) {
            if (abs(sprite.x - selectedSpriteX) == 1 && abs(sprite.y - selectedSpriteY) == 1) {
                glm::vec3 newVoxel = { sprite.x, sprite.y, sprite.z };
                attackHighlightedVoxels.push_back(newVoxel);
                break;
            }
        }
    }
}

void drawAllSprites() {
    animationWhenAdded();
    for (const auto& sprite : allSprites) {
        glm::vec3 position;
        position = glm::vec3(sprite.x, sprite.y, sprite.z);
        if (sprite.modelID == 1) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                assaultTroop->Draw(*ourShader, true, ModelTransformations::assaultTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::assaultTroop.scale, ModelTransformations::assaultTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 2) {
            arrow->Draw(*ourShader, true, ModelTransformations::arrow.position + position, ModelTransformations::arrow.scale, ModelTransformations::arrow.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 3) {
            airport->Draw(*ourShader, true, ModelTransformations::airport.position + position, ModelTransformations::airport.scale, ModelTransformations::airport.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 4) {
            artillery->Draw(*ourShader, true, ModelTransformations::artillery.position + position, ModelTransformations::artillery.scale, ModelTransformations::artillery.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 5) {
            battleship->Draw(*ourShader, true, ModelTransformations::battleship.position + position, ModelTransformations::battleship.scale, ModelTransformations::battleship.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 6) {
            boat->Draw(*ourShader, true, ModelTransformations::boat.position + position, ModelTransformations::boat.scale, ModelTransformations::boat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 7) {
            bomber->Draw(*ourShader, true, ModelTransformations::bomber.position + position, ModelTransformations::bomber.scale, ModelTransformations::bomber.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 8) {
            carrier->Draw(*ourShader, true, ModelTransformations::carrier.position + position, ModelTransformations::carrier.scale, ModelTransformations::carrier.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 9) {
            chemicalplant->Draw(*ourShader, true, ModelTransformations::chemicalplant.position + position, ModelTransformations::chemicalplant.scale, ModelTransformations::chemicalplant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 10) {
            colony->Draw(*ourShader, true, ModelTransformations::colony.position + position, ModelTransformations::colony.scale, ModelTransformations::colony.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 11) {
            combustionPlant->Draw(*ourShader, true, ModelTransformations::combustionPlant.position + position, ModelTransformations::combustionPlant.scale, ModelTransformations::combustionPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 12) {
            cram->Draw(*ourShader, true, ModelTransformations::cram.position + position, ModelTransformations::cram.scale, ModelTransformations::cram.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 13) {
            drone->Draw(*ourShader, true, ModelTransformations::drone.position + position, ModelTransformations::drone.scale, ModelTransformations::drone.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 14) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                droneTroop->Draw(*ourShader, true, ModelTransformations::droneTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::droneTroop.scale, ModelTransformations::droneTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 15) {
            factory->Draw(*ourShader, true, ModelTransformations::factory.position + position, ModelTransformations::factory.scale, ModelTransformations::factory.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 16) {
            fort->Draw(*ourShader, true, ModelTransformations::fort.position + position, ModelTransformations::fort.scale, ModelTransformations::fort.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 17) {
            garage->Draw(*ourShader, true, ModelTransformations::garage.position + position, ModelTransformations::garage.scale, ModelTransformations::garage.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 18) {
            helicopter->Draw(*ourShader, true, ModelTransformations::helicopter.position + position, ModelTransformations::helicopter.scale, ModelTransformations::helicopter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 19) {
            quary->Draw(*ourShader, true, ModelTransformations::quary.position + position, ModelTransformations::quary.scale, ModelTransformations::quary.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 20) {
            jeep->Draw(*ourShader, true, ModelTransformations::jeep.position + position, ModelTransformations::jeep.scale, ModelTransformations::jeep.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 21) {
            jet->Draw(*ourShader, true, ModelTransformations::jet.position + position, ModelTransformations::jet.scale, ModelTransformations::jet.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 22) {
            liberator->Draw(*ourShader, true, ModelTransformations::liberator.position + position, ModelTransformations::liberator.scale, ModelTransformations::liberator.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 23) {
            lifeboat->Draw(*ourShader, true, ModelTransformations::lifeboat.position + position, ModelTransformations::lifeboat.scale, ModelTransformations::lifeboat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 24) {
            mine->Draw(*ourShader, true, ModelTransformations::mine.position + position, ModelTransformations::mine.scale, ModelTransformations::mine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 25) {
            mlrs->Draw(*ourShader, true, ModelTransformations::mlrs.position + position, ModelTransformations::mlrs.scale, ModelTransformations::mlrs.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 26) {
            nuclearPlant->Draw(*ourShader, true, ModelTransformations::nuclearPlant.position + position, ModelTransformations::nuclearPlant.scale, ModelTransformations::nuclearPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 27) {
            recruitmentCenter->Draw(*ourShader, true, ModelTransformations::recruitmentCenter.position + position, ModelTransformations::recruitmentCenter.scale, ModelTransformations::recruitmentCenter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 28) {
            samSite->Draw(*ourShader, true, ModelTransformations::samSite.position + position, ModelTransformations::samSite.scale, ModelTransformations::samSite.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 29) {
            smeltingFacility->Draw(*ourShader, true, ModelTransformations::smeltingFacility.position + position, ModelTransformations::smeltingFacility.scale, ModelTransformations::smeltingFacility.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 30) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                sniperTroop->Draw(*ourShader, true, ModelTransformations::sniperTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::sniperTroop.scale, ModelTransformations::sniperTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 31) {
            submarine->Draw(*ourShader, true, ModelTransformations::submarine.position + position, ModelTransformations::submarine.scale, ModelTransformations::submarine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 32) {
            tank->Draw(*ourShader, true, ModelTransformations::tank.position + position, ModelTransformations::tank.scale, ModelTransformations::tank.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 33) {
            trench->Draw(*ourShader, true, ModelTransformations::trench.position + position, ModelTransformations::trench.scale, ModelTransformations::trench.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 34) {
            truck->Draw(*ourShader, true, ModelTransformations::truck.position + position, ModelTransformations::truck.scale, ModelTransformations::truck.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 35) {
            warehouse->Draw(*ourShader, true, ModelTransformations::warehouse.position + position, ModelTransformations::warehouse.scale, ModelTransformations::warehouse.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 36) {
            windPlant->Draw(*ourShader, true, ModelTransformations::windPlant.position + position, ModelTransformations::windPlant.scale, ModelTransformations::windPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }       
    }
    animationWhenRemoved();
    for (auto it = allSpritesBeingRemoved.begin(); it != allSpritesBeingRemoved.end(); ) {
        if (it->transparency == 0.0f) {
            it = allSpritesBeingRemoved.erase(it); 
        }
        else {
            ++it;
        }
    }
    for (const auto& sprite : allSpritesBeingRemoved) {
        glm::vec3 position;
        position = glm::vec3(sprite.x, sprite.y, sprite.z);
        if (sprite.modelID == 1) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                assaultTroop->Draw(*ourShader, true, ModelTransformations::assaultTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::assaultTroop.scale, ModelTransformations::assaultTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 2) {
            arrow->Draw(*ourShader, true, ModelTransformations::arrow.position + position, ModelTransformations::arrow.scale, ModelTransformations::arrow.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 3) {
            airport->Draw(*ourShader, true, ModelTransformations::airport.position + position, ModelTransformations::airport.scale, ModelTransformations::airport.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 4) {
            artillery->Draw(*ourShader, true, ModelTransformations::artillery.position + position, ModelTransformations::artillery.scale, ModelTransformations::artillery.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 5) {
            battleship->Draw(*ourShader, true, ModelTransformations::battleship.position + position, ModelTransformations::battleship.scale, ModelTransformations::battleship.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 6) {
            boat->Draw(*ourShader, true, ModelTransformations::boat.position + position, ModelTransformations::boat.scale, ModelTransformations::boat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 7) {
            bomber->Draw(*ourShader, true, ModelTransformations::bomber.position + position, ModelTransformations::bomber.scale, ModelTransformations::bomber.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 8) {
            carrier->Draw(*ourShader, true, ModelTransformations::carrier.position + position, ModelTransformations::carrier.scale, ModelTransformations::carrier.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 9) {
            chemicalplant->Draw(*ourShader, true, ModelTransformations::chemicalplant.position + position, ModelTransformations::chemicalplant.scale, ModelTransformations::chemicalplant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 10) {
            colony->Draw(*ourShader, true, ModelTransformations::colony.position + position, ModelTransformations::colony.scale, ModelTransformations::colony.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 11) {
            combustionPlant->Draw(*ourShader, true, ModelTransformations::combustionPlant.position + position, ModelTransformations::combustionPlant.scale, ModelTransformations::combustionPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 12) {
            cram->Draw(*ourShader, true, ModelTransformations::cram.position + position, ModelTransformations::cram.scale, ModelTransformations::cram.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 13) {
            drone->Draw(*ourShader, true, ModelTransformations::drone.position + position, ModelTransformations::drone.scale, ModelTransformations::drone.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 14) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                droneTroop->Draw(*ourShader, true, ModelTransformations::droneTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::droneTroop.scale, ModelTransformations::droneTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 15) {
            factory->Draw(*ourShader, true, ModelTransformations::factory.position + position, ModelTransformations::factory.scale, ModelTransformations::factory.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 16) {
            fort->Draw(*ourShader, true, ModelTransformations::fort.position + position, ModelTransformations::fort.scale, ModelTransformations::fort.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 17) {
            garage->Draw(*ourShader, true, ModelTransformations::garage.position + position, ModelTransformations::garage.scale, ModelTransformations::garage.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 18) {
            helicopter->Draw(*ourShader, true, ModelTransformations::helicopter.position + position, ModelTransformations::helicopter.scale, ModelTransformations::helicopter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 19) {
            quary->Draw(*ourShader, true, ModelTransformations::quary.position + position, ModelTransformations::quary.scale, ModelTransformations::quary.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 20) {
            jeep->Draw(*ourShader, true, ModelTransformations::jeep.position + position, ModelTransformations::jeep.scale, ModelTransformations::jeep.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 21) {
            jet->Draw(*ourShader, true, ModelTransformations::jet.position + position, ModelTransformations::jet.scale, ModelTransformations::jet.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 22) {
            liberator->Draw(*ourShader, true, ModelTransformations::liberator.position + position, ModelTransformations::liberator.scale, ModelTransformations::liberator.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 23) {
            lifeboat->Draw(*ourShader, true, ModelTransformations::lifeboat.position + position, ModelTransformations::lifeboat.scale, ModelTransformations::lifeboat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 24) {
            mine->Draw(*ourShader, true, ModelTransformations::mine.position + position, ModelTransformations::mine.scale, ModelTransformations::mine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 25) {
            mlrs->Draw(*ourShader, true, ModelTransformations::mlrs.position + position, ModelTransformations::mlrs.scale, ModelTransformations::mlrs.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 26) {
            nuclearPlant->Draw(*ourShader, true, ModelTransformations::nuclearPlant.position + position, ModelTransformations::nuclearPlant.scale, ModelTransformations::nuclearPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 27) {
            recruitmentCenter->Draw(*ourShader, true, ModelTransformations::recruitmentCenter.position + position, ModelTransformations::recruitmentCenter.scale, ModelTransformations::recruitmentCenter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 28) {
            samSite->Draw(*ourShader, true, ModelTransformations::samSite.position + position, ModelTransformations::samSite.scale, ModelTransformations::samSite.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 29) {
            smeltingFacility->Draw(*ourShader, true, ModelTransformations::smeltingFacility.position + position, ModelTransformations::smeltingFacility.scale, ModelTransformations::smeltingFacility.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 30) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                sniperTroop->Draw(*ourShader, true, ModelTransformations::sniperTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::sniperTroop.scale, ModelTransformations::sniperTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 31) {
            submarine->Draw(*ourShader, true, ModelTransformations::submarine.position + position, ModelTransformations::submarine.scale, ModelTransformations::submarine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 32) {
            tank->Draw(*ourShader, true, ModelTransformations::tank.position + position, ModelTransformations::tank.scale, ModelTransformations::tank.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 33) {
            trench->Draw(*ourShader, true, ModelTransformations::trench.position + position, ModelTransformations::trench.scale, ModelTransformations::trench.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 34) {
            truck->Draw(*ourShader, true, ModelTransformations::truck.position + position, ModelTransformations::truck.scale, ModelTransformations::truck.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 35) {
            warehouse->Draw(*ourShader, true, ModelTransformations::warehouse.position + position, ModelTransformations::warehouse.scale, ModelTransformations::warehouse.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 36) {
            windPlant->Draw(*ourShader, true, ModelTransformations::windPlant.position + position, ModelTransformations::windPlant.scale, ModelTransformations::windPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
    }
}

void drawAllSpritesPerspectiveBased() {
    animationWhenAddedinViewVersion();
    for (const auto& sprite : allSpritesInView) {
        glm::vec3 position;
        position = glm::vec3(sprite.x, sprite.y, sprite.z);
        if (sprite.modelID == 1) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                assaultTroop->Draw(*ourShader, true, ModelTransformations::assaultTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::assaultTroop.scale, ModelTransformations::assaultTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 2) {
            arrow->Draw(*ourShader, true, ModelTransformations::arrow.position + position, ModelTransformations::arrow.scale, ModelTransformations::arrow.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 3) {
            airport->Draw(*ourShader, true, ModelTransformations::airport.position + position, ModelTransformations::airport.scale, ModelTransformations::airport.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 4) {
            artillery->Draw(*ourShader, true, ModelTransformations::artillery.position + position, ModelTransformations::artillery.scale, ModelTransformations::artillery.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 5) {
            battleship->Draw(*ourShader, true, ModelTransformations::battleship.position + position, ModelTransformations::battleship.scale, ModelTransformations::battleship.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 6) {
            boat->Draw(*ourShader, true, ModelTransformations::boat.position + position, ModelTransformations::boat.scale, ModelTransformations::boat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 7) {
            bomber->Draw(*ourShader, true, ModelTransformations::bomber.position + position, ModelTransformations::bomber.scale, ModelTransformations::bomber.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 8) {
            carrier->Draw(*ourShader, true, ModelTransformations::carrier.position + position, ModelTransformations::carrier.scale, ModelTransformations::carrier.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 9) {
            chemicalplant->Draw(*ourShader, true, ModelTransformations::chemicalplant.position + position, ModelTransformations::chemicalplant.scale, ModelTransformations::chemicalplant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 10) {
            colony->Draw(*ourShader, true, ModelTransformations::colony.position + position, ModelTransformations::colony.scale, ModelTransformations::colony.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 11) {
            combustionPlant->Draw(*ourShader, true, ModelTransformations::combustionPlant.position + position, ModelTransformations::combustionPlant.scale, ModelTransformations::combustionPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 12) {
            cram->Draw(*ourShader, true, ModelTransformations::cram.position + position, ModelTransformations::cram.scale, ModelTransformations::cram.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 13) {
            drone->Draw(*ourShader, true, ModelTransformations::drone.position + position, ModelTransformations::drone.scale, ModelTransformations::drone.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 14) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                droneTroop->Draw(*ourShader, true, ModelTransformations::droneTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::droneTroop.scale, ModelTransformations::droneTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 15) {
            factory->Draw(*ourShader, true, ModelTransformations::factory.position + position, ModelTransformations::factory.scale, ModelTransformations::factory.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 16) {
            fort->Draw(*ourShader, true, ModelTransformations::fort.position + position, ModelTransformations::fort.scale, ModelTransformations::fort.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 17) {
            garage->Draw(*ourShader, true, ModelTransformations::garage.position + position, ModelTransformations::garage.scale, ModelTransformations::garage.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 18) {
            helicopter->Draw(*ourShader, true, ModelTransformations::helicopter.position + position, ModelTransformations::helicopter.scale, ModelTransformations::helicopter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 19) {
            quary->Draw(*ourShader, true, ModelTransformations::quary.position + position, ModelTransformations::quary.scale, ModelTransformations::quary.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 20) {
            jeep->Draw(*ourShader, true, ModelTransformations::jeep.position + position, ModelTransformations::jeep.scale, ModelTransformations::jeep.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 21) {
            jet->Draw(*ourShader, true, ModelTransformations::jet.position + position, ModelTransformations::jet.scale, ModelTransformations::jet.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 22) {
            liberator->Draw(*ourShader, true, ModelTransformations::liberator.position + position, ModelTransformations::liberator.scale, ModelTransformations::liberator.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 23) {
            lifeboat->Draw(*ourShader, true, ModelTransformations::lifeboat.position + position, ModelTransformations::lifeboat.scale, ModelTransformations::lifeboat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 24) {
            mine->Draw(*ourShader, true, ModelTransformations::mine.position + position, ModelTransformations::mine.scale, ModelTransformations::mine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 25) {
            mlrs->Draw(*ourShader, true, ModelTransformations::mlrs.position + position, ModelTransformations::mlrs.scale, ModelTransformations::mlrs.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 26) {
            nuclearPlant->Draw(*ourShader, true, ModelTransformations::nuclearPlant.position + position, ModelTransformations::nuclearPlant.scale, ModelTransformations::nuclearPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 27) {
            recruitmentCenter->Draw(*ourShader, true, ModelTransformations::recruitmentCenter.position + position, ModelTransformations::recruitmentCenter.scale, ModelTransformations::recruitmentCenter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 28) {
            samSite->Draw(*ourShader, true, ModelTransformations::samSite.position + position, ModelTransformations::samSite.scale, ModelTransformations::samSite.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 29) {
            smeltingFacility->Draw(*ourShader, true, ModelTransformations::smeltingFacility.position + position, ModelTransformations::smeltingFacility.scale, ModelTransformations::smeltingFacility.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 30) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                sniperTroop->Draw(*ourShader, true, ModelTransformations::sniperTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::sniperTroop.scale, ModelTransformations::sniperTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 31) {
            submarine->Draw(*ourShader, true, ModelTransformations::submarine.position + position, ModelTransformations::submarine.scale, ModelTransformations::submarine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 32) {
            tank->Draw(*ourShader, true, ModelTransformations::tank.position + position, ModelTransformations::tank.scale, ModelTransformations::tank.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 33) {
            trench->Draw(*ourShader, true, ModelTransformations::trench.position + position, ModelTransformations::trench.scale, ModelTransformations::trench.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 34) {
            truck->Draw(*ourShader, true, ModelTransformations::truck.position + position, ModelTransformations::truck.scale, ModelTransformations::truck.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 35) {
            warehouse->Draw(*ourShader, true, ModelTransformations::warehouse.position + position, ModelTransformations::warehouse.scale, ModelTransformations::warehouse.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 36) {
            windPlant->Draw(*ourShader, true, ModelTransformations::windPlant.position + position, ModelTransformations::windPlant.scale, ModelTransformations::windPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
    }
    animationWhenRemoved();
    for (auto it = allSpritesBeingRemoved.begin(); it != allSpritesBeingRemoved.end(); ) {
        if (it->transparency == 0.0f) {
            it = allSpritesBeingRemoved.erase(it);
        }
        else {
            ++it;
        }
    }
    for (const auto& sprite : allSpritesBeingRemoved) {
        glm::vec3 position;
        position = glm::vec3(sprite.x, sprite.y, sprite.z);
        if (sprite.modelID == 1) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                assaultTroop->Draw(*ourShader, true, ModelTransformations::assaultTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::assaultTroop.scale, ModelTransformations::assaultTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 2) {
            arrow->Draw(*ourShader, true, ModelTransformations::arrow.position + position, ModelTransformations::arrow.scale, ModelTransformations::arrow.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 3) {
            airport->Draw(*ourShader, true, ModelTransformations::airport.position + position, ModelTransformations::airport.scale, ModelTransformations::airport.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 4) {
            artillery->Draw(*ourShader, true, ModelTransformations::artillery.position + position, ModelTransformations::artillery.scale, ModelTransformations::artillery.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 5) {
            battleship->Draw(*ourShader, true, ModelTransformations::battleship.position + position, ModelTransformations::battleship.scale, ModelTransformations::battleship.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 6) {
            boat->Draw(*ourShader, true, ModelTransformations::boat.position + position, ModelTransformations::boat.scale, ModelTransformations::boat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 7) {
            bomber->Draw(*ourShader, true, ModelTransformations::bomber.position + position, ModelTransformations::bomber.scale, ModelTransformations::bomber.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 8) {
            carrier->Draw(*ourShader, true, ModelTransformations::carrier.position + position, ModelTransformations::carrier.scale, ModelTransformations::carrier.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 9) {
            chemicalplant->Draw(*ourShader, true, ModelTransformations::chemicalplant.position + position, ModelTransformations::chemicalplant.scale, ModelTransformations::chemicalplant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 10) {
            colony->Draw(*ourShader, true, ModelTransformations::colony.position + position, ModelTransformations::colony.scale, ModelTransformations::colony.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 11) {
            combustionPlant->Draw(*ourShader, true, ModelTransformations::combustionPlant.position + position, ModelTransformations::combustionPlant.scale, ModelTransformations::combustionPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 12) {
            cram->Draw(*ourShader, true, ModelTransformations::cram.position + position, ModelTransformations::cram.scale, ModelTransformations::cram.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 13) {
            drone->Draw(*ourShader, true, ModelTransformations::drone.position + position, ModelTransformations::drone.scale, ModelTransformations::drone.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 14) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                droneTroop->Draw(*ourShader, true, ModelTransformations::droneTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::droneTroop.scale, ModelTransformations::droneTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 15) {
            factory->Draw(*ourShader, true, ModelTransformations::factory.position + position, ModelTransformations::factory.scale, ModelTransformations::factory.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 16) {
            fort->Draw(*ourShader, true, ModelTransformations::fort.position + position, ModelTransformations::fort.scale, ModelTransformations::fort.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 17) {
            garage->Draw(*ourShader, true, ModelTransformations::garage.position + position, ModelTransformations::garage.scale, ModelTransformations::garage.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 18) {
            helicopter->Draw(*ourShader, true, ModelTransformations::helicopter.position + position, ModelTransformations::helicopter.scale, ModelTransformations::helicopter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 19) {
            quary->Draw(*ourShader, true, ModelTransformations::quary.position + position, ModelTransformations::quary.scale, ModelTransformations::quary.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 20) {
            jeep->Draw(*ourShader, true, ModelTransformations::jeep.position + position, ModelTransformations::jeep.scale, ModelTransformations::jeep.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 21) {
            jet->Draw(*ourShader, true, ModelTransformations::jet.position + position, ModelTransformations::jet.scale, ModelTransformations::jet.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 22) {
            liberator->Draw(*ourShader, true, ModelTransformations::liberator.position + position, ModelTransformations::liberator.scale, ModelTransformations::liberator.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 23) {
            lifeboat->Draw(*ourShader, true, ModelTransformations::lifeboat.position + position, ModelTransformations::lifeboat.scale, ModelTransformations::lifeboat.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 24) {
            mine->Draw(*ourShader, true, ModelTransformations::mine.position + position, ModelTransformations::mine.scale, ModelTransformations::mine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 25) {
            mlrs->Draw(*ourShader, true, ModelTransformations::mlrs.position + position, ModelTransformations::mlrs.scale, ModelTransformations::mlrs.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 26) {
            nuclearPlant->Draw(*ourShader, true, ModelTransformations::nuclearPlant.position + position, ModelTransformations::nuclearPlant.scale, ModelTransformations::nuclearPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 27) {
            recruitmentCenter->Draw(*ourShader, true, ModelTransformations::recruitmentCenter.position + position, ModelTransformations::recruitmentCenter.scale, ModelTransformations::recruitmentCenter.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 28) {
            samSite->Draw(*ourShader, true, ModelTransformations::samSite.position + position, ModelTransformations::samSite.scale, ModelTransformations::samSite.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 29) {
            smeltingFacility->Draw(*ourShader, true, ModelTransformations::smeltingFacility.position + position, ModelTransformations::smeltingFacility.scale, ModelTransformations::smeltingFacility.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 30) {
            if (numberOfAvailablePositions > sprite.antiClipPos) {
                sniperTroop->Draw(*ourShader, true, ModelTransformations::sniperTroop.position + position + randomTroopPositions[sprite.antiClipPos], ModelTransformations::sniperTroop.scale, ModelTransformations::sniperTroop.rotation, sprite.team, sprite.selected, sprite.transparency);
            }
        }
        else if (sprite.modelID == 31) {
            submarine->Draw(*ourShader, true, ModelTransformations::submarine.position + position, ModelTransformations::submarine.scale, ModelTransformations::submarine.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 32) {
            tank->Draw(*ourShader, true, ModelTransformations::tank.position + position, ModelTransformations::tank.scale, ModelTransformations::tank.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 33) {
            trench->Draw(*ourShader, true, ModelTransformations::trench.position + position, ModelTransformations::trench.scale, ModelTransformations::trench.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 34) {
            truck->Draw(*ourShader, true, ModelTransformations::truck.position + position, ModelTransformations::truck.scale, ModelTransformations::truck.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 35) {
            warehouse->Draw(*ourShader, true, ModelTransformations::warehouse.position + position, ModelTransformations::warehouse.scale, ModelTransformations::warehouse.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
        else if (sprite.modelID == 36) {
            windPlant->Draw(*ourShader, true, ModelTransformations::windPlant.position + position, ModelTransformations::windPlant.scale, ModelTransformations::windPlant.rotation, sprite.team, sprite.selected, sprite.transparency);
        }
    }
}

