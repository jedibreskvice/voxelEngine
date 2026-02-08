#ifndef SPRITE_LIST_H
#define SPRITE_LIST_H

#include <vector>
#include <glm/glm.hpp> 

struct Sprite {
    int x;
    float y;
    int z;
    int modelID;
    int team;
    int antiClipPos;
    unsigned int faction;
    unsigned int previousFaction;
    bool selected;
    float transparency;
    float movementRemaining;

    bool operator==(const Sprite& other) const {
        return x == other.x &&
            y == other.y &&
            z == other.z &&
            modelID == other.modelID &&
            team == other.team &&
            antiClipPos == other.antiClipPos &&
            faction == other.faction &&
            previousFaction == other.previousFaction &&
            selected == other.selected;
    }
};

struct unitStat {
    unsigned int movement;
    bool orLimit;
    bool order;
    bool truckCondition;
    unsigned int dmg;
    unsigned int heavyDmg;
    unsigned int range;
    unsigned int def;
    bool retaliates;
    unsigned int housing;
    unsigned int lightDmgProt;
    unsigned int retaliationDef;
    unsigned int troopDmg;
    bool splash;
};

struct VehicleContinents {
    int x;
    int z;
    unsigned int ATs;
    unsigned int STs;
    unsigned int DTs;
};

extern unitStat unitStatistics[];

struct Faction {
    int x;
    float y;
    int z;
    int team;
    unsigned int factionID;
    bool selectedByPlayer;

    Faction(int x, float y, int z, int team, unsigned int factionID, bool selectedByPlayer)
        : x(x), y(y), z(z), team(team), factionID(factionID), selectedByPlayer(selectedByPlayer){}
};

struct VoxiPoxy {
    glm::vec3 position;
    float movementSubtracted;
};
extern std::vector<VoxiPoxy> confirmedVoxels;
extern std::vector<Sprite> allSprites;
extern std::vector<Sprite> allSpritesInView; 
extern std::vector<Sprite> allSpritesBeingRemoved;
extern std::vector<Faction> allFactions;
extern std::vector<Faction> allFactionsInView;
extern std::vector<glm::vec3> validTilesToMoveTo;
extern std::vector<glm::vec3> highlightedVoxels;
extern std::vector<glm::vec3> attackHighlightedVoxels;
extern std::vector<VehicleContinents> vehicleContinentsList;

void addSprite(int x, float y, int z, int modelID, int team);
void addHighlightedVoxelsVector(int x, float y, int z, float range, std::vector<glm::vec3> topVoxels);
void removeSprite(std::vector<Sprite>& allSprites, const Sprite& spriteToRemove);
bool validityCHeck(int x, float y, int z, int modelID, int team);
void movementValidityCheck(int x, float y, int z);
void printConfirmedVoxels();
void printAllSprites();
void drawAllSprites();
void resetMovementValues();
void drawAllSpritesPerspectiveBased();
unsigned int declareNewFaction(int team);
void changeSelectValuesByFaction(unsigned int selectedID);
void changeSelectValueBySprite(unsigned int faction, bool includeVehicle, float atValue, float stValue, float dtValue, int team);
void printFactionListInfo(const std::vector<Faction>& allFactions);
void recalculateAllVision(std::vector<Faction> allFactions, std::vector<glm::vec3> topVoxels, int team);
void deselectEverything();
void printVehicleContinentsList();
void processAttackLogic(int team);

#endif // SPRITE_MANAGER_H