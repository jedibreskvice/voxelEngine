#include "ModelList.h"
#include "Model.h"

Shader* ourShader = nullptr;

Model* assaultTroop = nullptr;
Model* arrow = nullptr;
Model* airport = nullptr;
Model* artillery = nullptr;
Model* battleship = nullptr;
Model* boat = nullptr;
Model* bomber = nullptr;
Model* carrier = nullptr;
Model* chemicalplant = nullptr;
Model* colony = nullptr;
Model* combustionPlant = nullptr;
Model* cram = nullptr;
Model* drone = nullptr;
Model* droneTroop = nullptr;
Model* factory = nullptr;
Model* fort = nullptr;
Model* garage = nullptr;
Model* helicopter = nullptr;
Model* jeep = nullptr;
Model* jet = nullptr;
Model* liberator = nullptr;
Model* lifeboat = nullptr;
Model* mine = nullptr;
Model* mlrs = nullptr;
Model* nuclearPlant = nullptr;
Model* quary = nullptr;
Model* recruitmentCenter = nullptr;
Model* samSite = nullptr;
Model* smeltingFacility = nullptr;
Model* sniperTroop = nullptr;
Model* submarine = nullptr;
Model* tank = nullptr;
Model* trench = nullptr;
Model* truck = nullptr;
Model* warehouse = nullptr;
Model* windPlant = nullptr;
Model* moveTo = nullptr;

void InitializeModels() {
    ourShader = new Shader("shader.vs", "shader.fs");

    assaultTroop = new Model("resources/models/assaultTroopPlaceholder/assaultTroopPlaceholder.obj");
    arrow = new Model("resources/models/arrow/arrow.obj");
    airport = new Model("resources/models/airport/airport.obj");
    artillery = new Model("resources/models/artillery/artillery.obj");
    battleship = new Model("resources/models/battleship/battleship.obj");
    boat = new Model("resources/models/boat/boat.obj");
    bomber = new Model("resources/models/bomber/bomber.obj");
    carrier = new Model("resources/models/carrier/carrier.obj");
    chemicalplant = new Model("resources/models/chemicalplant/chemicalplant.obj");
    colony = new Model("resources/models/colony/colony.obj");
    combustionPlant = new Model("resources/models/combustionplant/combustionplant.obj");
    cram = new Model("resources/models/Cram/Cram.obj");
    drone = new Model("resources/models/drone/drone.obj");
    droneTroop = new Model("resources/models/droneTroopPlaceholder/droneTroopPlaceholder.obj");
    factory = new Model("resources/models/factory/factory.obj");
    fort = new Model("resources/models/fort/fort.obj");
    garage = new Model("resources/models/Garage/Garage.obj");
    helicopter = new Model("resources/models/helicopter/helicopter.obj");
    jeep = new Model("resources/models/jeep/jeep.obj");
    jet = new Model("resources/models/jet/jet.obj");
    liberator = new Model("resources/models/liberator/liberator.obj");
    lifeboat = new Model("resources/models/lifeboat/lifeboat.obj");
    mine = new Model("resources/models/Mine/Mine.obj");
    mlrs = new Model("resources/models/MLRS/MLRS.obj");
    nuclearPlant = new Model("resources/models/nuclearplant/nuclearplant.obj");
    quary = new Model("resources/models/Quary/Quary.obj");
    recruitmentCenter = new Model("resources/models/RecruitmentCenter/RecruitmentCenter.obj");
    samSite = new Model("resources/models/SAMsite/SAMsite.obj");
    smeltingFacility = new Model("resources/models/smeltingfacility/smeltingfacility.obj");
    sniperTroop = new Model("resources/models/sniperTroopPlaceholder/sniperTroopPlaceholder.obj");
    submarine = new Model("resources/models/submarine/submarine.obj");
    tank = new Model("resources/models/tank/tank.obj");
    trench = new Model("resources/models/trench/trench.obj");
    truck = new Model("resources/models/truck/truck.obj");
    warehouse = new Model("resources/models/Warehouse/Warehouse.obj");
    windPlant = new Model("resources/models/windplant/windplant.obj");
    moveTo = new Model("resources/models/moveTo/moveTo.obj");
}

void CleanupModels() {
    delete assaultTroop;
    delete arrow;
    delete airport;
    delete artillery;
    delete battleship;
    delete boat;
    delete bomber;
    delete carrier;
    delete chemicalplant;
    delete colony;
    delete combustionPlant;
    delete cram;
    delete drone;
    delete droneTroop;
    delete factory;
    delete fort;
    delete garage;
    delete helicopter;
    delete jeep;
    delete jet;
    delete liberator;
    delete lifeboat;
    delete mine;
    delete mlrs;
    delete nuclearPlant;
    delete quary;
    delete recruitmentCenter;
    delete samSite;
    delete smeltingFacility;
    delete sniperTroop;
    delete submarine;
    delete tank;
    delete trench;
    delete truck;
    delete warehouse;
    delete windPlant;
    delete moveTo;

    delete ourShader;
}