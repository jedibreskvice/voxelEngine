#include "VoxelGrid.h"
#include "RendererSetup.h"
#include "Shader.h"
#include <cmath>
#include <array>
#include <iomanip>
#include <sstream>
#include <vector>
#include <set>
#include <utility>
#include "Randomizer.h"
#include <tuple>

// Global pointer to the shader object
Shader* voxelShader;
int timesCalled;

void setup() {
    // Initialize the shader using the paths provided
    voxelShader = new Shader("shader.vs", "shader.fs");
    setupBuffersAndTextures();
}

void cleanup() {
    // Clean up the dynamically allocated shader
    delete voxelShader;
}

void generateWorld(Shader& shader, unsigned int mapSize, unsigned int mapDepth, const std::string& mapType, float potency, float level) {
    bool notMap = false;
    shader.setBool("notMap", notMap);

    if (mapType == "superflat") {
        generateWorldSuperflat(mapSize, mapDepth);
    }
    else if (mapType == "pyramid") {
        generateWorldPyramid(mapSize, mapDepth, potency);
    }
    else if (mapType == "hill") {
        generateWorldHill(mapSize, mapDepth, potency);
    }
    else if (mapType == "platenau") {
        generateWorldPlateau(mapSize, mapDepth, potency);
    }
    else if (mapType == "hillyFlats") {
        generateWorldHillyFlats(mapSize, mapDepth, potency);
    }
    else {
        std::cerr << "Unknown map type: " << mapType << std::endl;
    }
    if (level > 0) {
        drawWater(level, voxelShader->ID);
    }
}

void generateWorldSuperflat(unsigned int mapSize, unsigned int mapDepth) {
    static bool initialized = false;
    if (!initialized) {
        float xSpacing = 1.0f;
        float ySpacing = 0.25f;
        float zSpacing = 1.0f;

        for (int x = 0; x < mapSize; x++) {
            for (int z = 0; z < mapSize; z++) {
                for (int y = 0; y < mapDepth; y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }
        initialized = true;
        checkFaces(allVoxels);
    }
    drawFaces(outsideVoxels, voxelShader->ID);
}

void generateWorldPyramid(unsigned int mapSize, unsigned int mapDepth, float potency) {
    static bool initialized = false;
    if (!initialized) {
        float xSpacing = 1.0f;
        float ySpacing = 0.25f;
        float zSpacing = 1.0f;

        for (int x = 1; x <= mapSize; x++) {
            for (int z = 1; z <= mapSize; z++) {
                int remainder = mapSize % 2;
                unsigned int yAdd1max = (mapSize / 2) + remainder;
                int yAddz = z;
                int yAddx = x;
                if (remainder == 0) {
                    if (yAdd1max < x) {
                        yAddx += ((yAdd1max - x) * 2) + 1;
                    }
                    if (yAdd1max < z) {
                        yAddz += ((yAdd1max - z) * 2) + 1;
                    }
                }
                else {
                    if (yAdd1max < x) {
                        yAddx += (yAdd1max - x) * 2;
                    }
                    if (yAdd1max < z) {
                        yAddz += (yAdd1max - z) * 2;
                    }
                }
                float yAdd1 = std::min(yAddx, yAddz);
                yAdd1 = yAdd1 * potency;
                float piramidY = yAdd1 + mapDepth;

                for (int y = 1; y <= piramidY; y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }
        initialized = true;
        checkFaces(allVoxels);
    }
    
    drawFaces(outsideVoxels, voxelShader->ID);
}

void generateWorldHill(unsigned int mapSize, unsigned int mapDepth, float potency) {
    static bool initialized = false;
    if (!initialized) {
        float xSpacing = 1.0f;
        float ySpacing = 0.25f;
        float zSpacing = 1.0f;

        // Calculate the center of the hill
        float hillCenter = mapSize / 2.0f;
        float hillRadius = hillCenter;  // Hill radius set to half of the map size

        // Generate the base (a box of voxels)
        for (int x = 1; x <= mapSize; x++) {
            for (int z = 1; z <= mapSize; z++) {
                for (int y = 1; y <= mapDepth; y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }

        // Generate the smooth hill without a plateau
        for (int x = 1; x <= mapSize; x++) {
            for (int z = 1; z <= mapSize; z++) {
                // Calculate distance from the center
                float dx = x - hillCenter;
                float dz = z - hillCenter;
                float distance = sqrtf(dx * dx + dz * dz);

                // Calculate hill height based on the distance from the center
                float normalizedDistance = distance / hillRadius;
                float Cy = -0.5f * (normalizedDistance * normalizedDistance) + 0.5f;

                // Convert Cy to integer for voxel placement
                int hillY = static_cast<int>(Cy * potency * mapDepth);

                // Ensure hillY is within a valid range
                if (hillY < 0) hillY = 0;

                for (int y = mapDepth + 1; y <= (hillY + mapDepth); y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }
        initialized = true;
        checkFaces(allVoxels);
    }
    drawFaces(outsideVoxels, voxelShader->ID);
}

void generateWorldPlateau(unsigned int mapSize, unsigned int mapDepth, float potency) {
    static bool initialized = false;
    if (!initialized) {
        float xSpacing = 1.0f;
        float ySpacing = 0.25f;
        float zSpacing = 1.0f;

        // Calculate the center of the hill
        float hillCenter = mapSize / 2.0f;
        float hillRadius = hillCenter;  // Hill radius set to half of the map size
        float plateauRadius = hillRadius * 0.7f;  // Adjust this value for plateau size (30% of the hill radius)

        // Generate the base (a box of voxels)
        for (int x = 1; x <= mapSize; x++) {
            for (int z = 1; z <= mapSize; z++) {
                for (int y = 1; y <= mapDepth; y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }

        // Generate the hill with a plateau on top
        for (int x = 1; x <= mapSize; x++) {
            for (int z = 1; z <= mapSize; z++) {
                // Calculate distance from the center
                float dx = x - hillCenter;
                float dz = z - hillCenter;
                float distance = sqrtf(dx * dx + dz * dz);

                float Cy;

                // Create plateau effect
                if (distance <= plateauRadius) {
                    Cy = 0.5f;  // Flat top of the hill
                }
                else {
                    // Calculate hill height based on distance from the plateau edge
                    float normalizedDistance = (distance - plateauRadius) / (hillRadius - plateauRadius);
                    Cy = -0.5f * (normalizedDistance * normalizedDistance) + 0.5f;
                }

                // Convert Cy to integer for voxel placement
                int hillY = static_cast<int>(Cy * potency * mapDepth);

                // Ensure hillY is within a valid range
                if (hillY < 0) hillY = 0;

                for (int y = mapDepth + 1; y <= (hillY + mapDepth); y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID); //INEFFICIENT
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }
        initialized = true;
       checkFaces(allVoxels);
    }
    drawFaces(outsideVoxels, voxelShader->ID);
}

////////////////////////  Advanced proceduall shit  //////////////////////////////////

float generateRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

struct Point {
    double x, z;
};

void generatePointsLinearly(const Point& A, const Point& B, std::vector<Point>& points) {
    points.clear();  // Clear the points vector to avoid duplication

    if (A.x == B.x) {
        // Handle vertical line
        if (A.z != B.z) {  // Only add points if the z-coordinates are different
            if (A.z > B.z) {
                for (int z = static_cast<int>(B.z); z <= static_cast<int>(A.z); ++z) {
                    points.push_back(Point{ A.x, static_cast<double>(z) });
                }
            }
            else {
                for (int z = static_cast<int>(A.z); z <= static_cast<int>(B.z); ++z) {
                    points.push_back(Point{ A.x, static_cast<double>(z) });
                }
            }
        }
    }
    else {
        double m = (B.z - A.z) / (B.x - A.x);
        double b = A.z - m * A.x;

        if (std::abs(m) > 1) {
            if (A.z > B.z) {
                for (double z = B.z; z <= A.z; ++z) {
                    int x = static_cast<int>(std::round((z - b) / m));
                    points.push_back(Point{ static_cast<double>(x), z });
                }
            }
            else {
                for (double z = A.z; z <= B.z; ++z) {
                    int x = static_cast<int>(std::round((z - b) / m));
                    points.push_back(Point{ static_cast<double>(x), z });
                }
            }
        }
        else {
            if (A.x > B.x) {
                for (double x = B.x; x <= A.x; ++x) {
                    int z = static_cast<int>(std::round(m * x + b));
                    points.push_back(Point{ x, static_cast<double>(z) });
                }
            }
            else {
                for (double x = A.x; x <= B.x; ++x) {
                    int z = static_cast<int>(std::round(m * x + b));
                    points.push_back(Point{ x, static_cast<double>(z) });
                }
            }
        }
    }

    // Remove duplicates if necessary
    std::set<std::pair<double, double>> uniquePoints;
    std::vector<Point> finalPoints;

    for (const auto& point : points) {
        auto p = std::make_pair(point.x, point.z);
        if (uniquePoints.find(p) == uniquePoints.end()) {
            uniquePoints.insert(p);
            finalPoints.push_back(point);
        }
    }

    // Copy back to the original points vector
    points = finalPoints;
}

struct Circle {
    unsigned int circleNum;
    float circleRadius;
};

bool isPointInCircle(const Point& point, const Circle& circle, unsigned int mapSize) {
    float center = mapSize / 2.0f;
    float dx = point.x - center;
    float dz = point.z - center;
    return (dx * dx + dz * dz) <= (circle.circleRadius * circle.circleRadius);
}

int roundToInt(float value) {
    return static_cast<int>(std::round(value));
}

void generateWorldHillyFlats(unsigned int mapSize, unsigned int mapDepth, float potency) {
    static bool initialized = false;
    if(!initialized){
        float xSpacing = 1.0f;
        float ySpacing = 0.25f;
        float zSpacing = 1.0f;

        // Generate the base voxels
        for (int x = 0; x < mapSize; x++) {
            for (int z = 0; z < mapSize; z++) {
                for (int y = 0; y < mapDepth; y++) {
                    float xCoord = x * xSpacing;
                    float yCoord = y * ySpacing;
                    float zCoord = z * zSpacing;

                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                }
            }
        }

        static std::vector<std::pair<Point, Point>> staticLines;
        static bool linesGenerated = false;


        if (!linesGenerated) {
            unsigned int increment = 5;
            unsigned int totalCircles = (mapSize / (increment * 2)) - 1;

            // Generate circles
            std::vector<Circle> circleList;
            for (unsigned int i = 0; i < totalCircles; ++i) {
                Circle circle;
                circle.circleNum = i;
                circle.circleRadius = increment * (i + 1);
                circleList.push_back(circle);
            }

            std::vector<Point> points;
            staticLines.clear();

            // Generate lines of voxels
            for (unsigned int i = 0; i < totalCircles * 2 - 1; ++i) {
                Point A, B;
                bool pointFound = false;

                // Find a valid point A within the current circle but outside the previous circle
                while (!pointFound) {
                    A.x = generateRandomFloat(0, mapSize - 1);
                    A.z = generateRandomFloat(0, mapSize - 1);

                    for (size_t j = 0; j < circleList.size(); ++j) {
                        const auto& circle = circleList[j];
                        if (j == 0) {
                            if (isPointInCircle(A, circle, mapSize)) {
                                pointFound = true;
                                break;
                            }
                        }
                        else {
                            if (isPointInCircle(A, circle, mapSize) && !isPointInCircle(A, circleList[j - 1], mapSize)) {
                                pointFound = true;
                                break;
                            }
                        }
                    }
                }

                pointFound = false;

                // Find a valid point B within the current circle but outside the previous circle
                while (!pointFound) {
                    B.x = generateRandomFloat(0, mapSize - 1);
                    B.z = generateRandomFloat(0, mapSize - 1);

                    for (size_t j = 0; j < circleList.size(); ++j) {
                        const auto& circle = circleList[j];
                        if (j == 0) {
                            if (isPointInCircle(B, circle, mapSize)) {
                                pointFound = true;
                                break;
                            }
                        }
                        else {
                            if (isPointInCircle(B, circle, mapSize) && !isPointInCircle(B, circleList[j - 1], mapSize)) {
                                pointFound = true;
                                break;
                            }
                        }
                    }
                }

                // Round points to the nearest integer
                A.x = roundToInt(A.x);
                A.z = roundToInt(A.z);
                B.x = roundToInt(B.x);
                B.z = roundToInt(B.z);

                // Store the line segment
                staticLines.push_back({ A, B });
            }

            linesGenerated = true;
        }

        std::vector<Point> points;
        std::set<std::tuple<int, int, int>> processedVoxels;

        // Process each line segment
        for (const auto& line : staticLines) {
            generatePointsLinearly(line.first, line.second, points);

            // Add additional voxels on top of each point based on potency
            for (const auto& point : points) {
                float xCoord = static_cast<float>(roundToInt(point.x)) * xSpacing;
                float zCoord = static_cast<float>(roundToInt(point.z)) * zSpacing;

                // Create voxels at increasing y heights based on potency
                for (int yOffset = 0; yOffset < static_cast<int>(potency); ++yOffset) {
                    float yCoord = (ySpacing * mapDepth) + (ySpacing * yOffset);
                    //drawVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID); //INEFFICIENT
                    planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
                    processedVoxels.insert({ roundToInt(point.x), roundToInt(point.z), mapDepth + yOffset });
                }
            }

            points.clear(); // Clear points for the next line
        }

        // Generate additional layers to smooth the terrain
        int maxIterations = 100; // Or any reasonable limit based on your map size
        int iterationCount = 0;
        bool changesMade = true; // Initialize changesMade

        // Determine maximum depth for new layers based on potency
        int maxDepth = static_cast<int>(potency) * 10; // Example: potency directly scales the depth

        while (changesMade && iterationCount < maxIterations) {
            changesMade = false;
            iterationCount++;
            std::vector<std::tuple<int, int, int>> newVoxels;

            for (const auto& voxel : processedVoxels) {
                int x = std::get<0>(voxel);
                int z = std::get<1>(voxel);
                int y = std::get<2>(voxel);

                // Check adjacent voxels to fill gaps
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        if ((dx != 0 || dz != 0) && std::abs(dx) + std::abs(dz) == 1) { // Only check adjacent positions
                            int neighborX = x + dx;
                            int neighborZ = z + dz;
                            int neighborY;

                            if (neighborX > mapSize / 3 && neighborX < mapSize * 0.67 && neighborZ > mapSize / 3 && neighborZ < mapSize * 0.67) {
                                neighborY = y - 1;
                            }
                            else {
                                neighborY = y - 2;
                            }


                            // Only place the voxel if it doesn't already exist, height difference is greater than 2, and within depth limit
                            if (neighborY >= mapDepth && neighborY <= mapDepth + maxDepth &&
                                processedVoxels.find({ neighborX, neighborZ, neighborY }) == processedVoxels.end()) {
                                //drawVoxel(neighborX * xSpacing, neighborY * ySpacing, neighborZ * zSpacing, "grassVoxelTexture", voxelShader->ID); //INEFFICIENT
                                planVoxel(neighborX * xSpacing, neighborY * ySpacing, neighborZ * zSpacing, "grassVoxelTexture", voxelShader->ID);
                                newVoxels.push_back({ neighborX, neighborZ, neighborY });
                                changesMade = true;
                            }
                        }
                    }
                }
            }
            processedVoxels.insert(newVoxels.begin(), newVoxels.end());
        }
        int maxHeightLimit = mapDepth + 10; // Adjust this value as needed

        int numRandomVoxels = (mapSize * mapSize) / 80; // Adjust the number of random voxels as needed
        for (int i = 0; i < numRandomVoxels; ++i) {
            int randX = generateRandom(mapSize / 2, (mapSize * 0.75) - 1);
            int randZ = generateRandom(mapSize / 2, (mapSize * 0.75) - 1);

            // Find the ground level at (randX, randZ)
            int groundY = mapDepth - 1; // Default to the base level
            while (groundY < mapDepth + maxDepth &&
                processedVoxels.find({ randX, randZ, groundY + 1 }) != processedVoxels.end()) {
                groundY++;
            }

            // Ensure the groundY + randHeight does not exceed maxHeightLimit
            int randHeight = generateRandom(1, 5); // Adjust height range as needed
            if (groundY + randHeight > maxHeightLimit) {
                randHeight = maxHeightLimit - groundY; // Adjust the height to stay within the limit
            }

            // Now add random voxels starting from the ground level
            for (int j = 0; j < randHeight; ++j) {
                float xCoord = randX * xSpacing;
                float yCoord = (groundY + j) * ySpacing;
                float zCoord = randZ * zSpacing;

                planVoxel(xCoord, yCoord, zCoord, "grassVoxelTexture", voxelShader->ID);
            }
        }
        initialized = true;
        checkFaces(allVoxels);
    }
    drawFaces(outsideVoxels, voxelShader->ID);
}

