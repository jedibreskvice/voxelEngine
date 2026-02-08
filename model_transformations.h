#ifndef MODEL_TRANSFORMATIONS_H
#define MODEL_TRANSFORMATIONS_H

#include <glm/glm.hpp>



struct ModelTransform {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    int modelID;
};

namespace ModelTransformations {
    const ModelTransform assaultTroop = { glm::vec3(0.0f, -0.118f, 0.0f), glm::vec3(0.007f, 0.007f, 0.007f), glm::vec3(0.0f, 270.0f, 0.0f), 1 };
    const ModelTransform arrow = { glm::vec3(0.0f, 0.182f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 0.0f), 2 };
    const ModelTransform airport = { glm::vec3(0.0f, -0.163f, 0.0f), glm::vec3(0.24f, 0.24f, 0.24f), glm::vec3(0.0f, 0.0f, 0.0f), 3 };
    const ModelTransform artillery = { glm::vec3(0.0f, -0.124f, 0.0f), glm::vec3(0.24f, 0.24f, 0.24f), glm::vec3(0.0f, 0.0f, 0.0f), 4 };
    const ModelTransform battleship = { glm::vec3(0.0f, -0.124f, 0.9f), glm::vec3(0.032f, 0.032f, 0.032f), glm::vec3(0.0f, 0.0f, 0.0f), 5 };
    const ModelTransform boat = { glm::vec3(0.0f, -0.124f, 0.0f), glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.0f, 0.0f, 0.0f), 6 };
    const ModelTransform bomber = { glm::vec3(0.0f, 0.326f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 5.0f), 7 };
    const ModelTransform carrier = { glm::vec3(0.0f, -0.124f, 0.9f), glm::vec3(0.032f, 0.032f, 0.032f), glm::vec3(0.0f, 0.0f, 0.0f), 8 };
    const ModelTransform chemicalplant = { glm::vec3(0.0f, -0.104f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 9 };
    const ModelTransform colony = { glm::vec3(0.0f, -0.1466f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 10 };
    const ModelTransform combustionPlant = { glm::vec3(0.0f, -0.162f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 11 };
    const ModelTransform cram = { glm::vec3(0.0f, -0.293f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 12 };
    const ModelTransform drone = { glm::vec3(0.0f, 0.23f, 0.0f), glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.0f, 0.0f, 0.0f), 13 };
    const ModelTransform droneTroop = { glm::vec3(0.0f, -0.118f, 0.0f), glm::vec3(0.007f, 0.007f, 0.007f), glm::vec3(0.0f, 270.0f, 0.0f), 14 };
    const ModelTransform factory = { glm::vec3(0.0f, -0.16f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 15 };
    const ModelTransform fort = { glm::vec3(0.0f, -0.162f, 0.0f), glm::vec3(0.26f, 0.26f, 0.26f), glm::vec3(0.0f, 0.0f, 0.0f), 16 };
    const ModelTransform garage = { glm::vec3(0.0f, -0.164f, 0.0f), glm::vec3(0.22f, 0.18f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 17 };
    const ModelTransform helicopter = { glm::vec3(0.0f, 0.21f, 0.0f), glm::vec3(0.06f, 0.06f, 0.06f), glm::vec3(0.0f, 0.0f, 0.0f), 18 };
    const ModelTransform quary = { glm::vec3(0.0f, -0.165f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 19 };
    const ModelTransform jeep = { glm::vec3(0.0f, -0.08f, 0.07f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.0f, 0.0f, 0.0f), 20 };
    const ModelTransform jet = { glm::vec3(0.0f, 0.35f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.0f, 0.0f, 0.0f), 21 };
    const ModelTransform liberator = { glm::vec3(0.03f, -0.096f, 0.27f), glm::vec3(0.65f, 0.65f, 0.65f), glm::vec3(0.0f, 0.0f, 0.0f), 22 };
    const ModelTransform lifeboat = { glm::vec3(0.0f, -0.124f, 0.0f), glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.0f, 0.0f, 0.0f), 23 };
    const ModelTransform mine = { glm::vec3(0.057f, -0.158f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 24 };
    const ModelTransform mlrs = { glm::vec3(0.0f, -0.087f, -0.2f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.0f, 0.0f, 0.0f), 25 };
    const ModelTransform nuclearPlant = { glm::vec3(0.0f, -0.174f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), 26 };
    const ModelTransform recruitmentCenter = { glm::vec3(0.0f, -0.182f, 0.0f), glm::vec3(0.28f, 0.28f, 0.28f), glm::vec3(0.0f, 0.0f, 0.0f), 27 };
    const ModelTransform samSite = { glm::vec3(0.0f, -0.128f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 28 };
    const ModelTransform smeltingFacility = { glm::vec3(0.0f, -0.138f, 0.0f), glm::vec3(0.24f, 0.24f, 0.24f), glm::vec3(0.0f, 0.0f, 0.0f), 29 };
    const ModelTransform sniperTroop = { glm::vec3(0.0f, -0.118f, 0.0f), glm::vec3(0.007f, 0.007f, 0.007f), glm::vec3(0.0f, 270.0f, 0.0f), 30 };
    const ModelTransform submarine = { glm::vec3(0.0f, -0.124f, 0.0f), glm::vec3(0.06f, 0.06f, 0.06f), glm::vec3(0.0f, 0.0f, 0.0f), 31 };
    const ModelTransform tank = { glm::vec3(0.02f, -0.09f, -0.029f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.0f, 0.0f, 0.0f), 32 };
    const ModelTransform trench = { glm::vec3(0.0f, -0.158f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 33 };
    const ModelTransform truck = { glm::vec3(0.0f, -0.08f, -0.2f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.0f, 0.0f, 0.0f), 34 };
    const ModelTransform warehouse = { glm::vec3(0.0f, -0.052f, 0.0f), glm::vec3(0.08f, 0.08f, 0.08f), glm::vec3(0.0f, 0.0f, 0.0f), 35 };
    const ModelTransform windPlant = { glm::vec3(0.0f, -0.192f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 36 };
    const ModelTransform moveTo = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.22f, 0.22f, 0.22f), glm::vec3(0.0f, 0.0f, 0.0f), 37 };
}

#endif
