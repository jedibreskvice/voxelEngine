/*--------------------------------------------------------------------------------------------------------------
Main cpp file. Engine includes:
- Functioning Shader program, which handles converting verticies positioned in the 3D space by firsty specifying
  which of them are located in the camera view, and assigning them screen coordinates (VERTEX SHADER). It then
  assigns the colors of the pixels generated in the window based on those positions (FRAGMENT SHADER).
- Functioning Camera class, which moves verticies around the screen in very specific ways, generating an ilusion
  of a camera moving around the 3D scene. The camera can be easily controlled with a few lines of code.
- Model loading works partially. There is still issues when loading materials, but loading vertecies and 
  textures mapped to them worked without much trouble so far. It was very painful to get model loader this far.
--------------------------------------------------------------------------------------------------------------*/

#include <glad/glad.h> //the library points to secific function locations, which need to be retrieved during run-time, which would be cumbersome to do manually. 
#include <GLFW/glfw3.h> //bare necessities for working with OpenGL: Simple window generation, ability to easily specify window parameters...
#include "stb_image.h" //image loading library. Supports popular formats and allows us to retrieve texture info from several image file types easily.
#include "Shader.h" //includes our shader class, which reads shaders from a disk, compiles and links them. Having it in a seperate file also helps with organisation.
#include <iostream> //integrate the INPUT-OUTPUT STREAM library. Allows us to use input / output operations.
#include <vector>  //allows including vectors.
#include <glm/glm.hpp> //includes the OpenGL specific Mathematics library, which we mainly use to simplify matrix operations.
#include <glm/gtc/matrix_transform.hpp> //more glm.
#include <glm/gtc/type_ptr.hpp> //even more glm.
#include "Camera.h" //includes our camera class, which allows for camera position, FOV and euler angle manipulation. In seperate file for organisation.
#include "RendererSetup.h" //includes another header file, which exist for organisation and handles the VBO and VAO setup as well as rendering textures.
#include "Model.h" //include model loader header file.
#include "VoxelGrid.h"
#include <iomanip> 
#include <random>
#include <cmath> 
#include "model_transformations.h"
#include "Raycasting.h"
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "SpriteList.h"
#include "ModelList.h"
#include "UIManager.h"
#include "TurnSystem.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float lightRadius = 10.0f;  
const float lightSpeed = 1.0f;    

//callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);                  //gets called every time a window gets resized. Ensures that the OpenGL viewport matches the new window dimensions. 
void processInput(GLFWwindow* window);                                                      //called each frame to check and process user input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);                          //called whenever the mouse moves, xpos and ypos are the new mouse coordinates
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);                   //callback for mouse scroll events. Currently yoffset is used for zooming in
//define screen resoultion
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
//define random variables
bool bToggle;
bool fToggle;
bool vToggle;
bool bReleased;
bool fReleased;
bool vReleased;
bool bDown = true;
bool fDown = true;
bool vDown = true;
bool spaceDown = false;
int deformSpeed = 1;
float blazValue = 0.2f;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float deltaTime;
float lastFrame;
bool qDown;
bool eDown;
bool leftMouseReleased;
bool leftMouseDown;
bool rightMouseDown;
float rotationAngle = 0.0f;
float rotationSpeed = 45.0f;
float bobbingAmplitude = 0.2f; 
float bobbingFrequency = 2.0f;
bool showUI = false;
float rayX = 0;
float rayY = 0;
float rayZ = 0;
int rayModelID = 0;
unsigned int selectedFaction = 0;
bool rayCastHasTarget = false;
bool cursorCaptured = true;
bool focusedOnFaction = false;

/*Creates an instance of a Camera class/------------------------------------------------------------------------
* First parameter sets the initial position of the camera in world space
* Second parameter defines the UP direction of the camera, which defines which way is up for the cameras view
* Third and fourth parameters set initial euler angles (which define where the camera is looking based on its
  position) The exact initial values are set in the Shader.h header filew
* In the last parameter we calculate the aspect ratio. We need this for projection processes so that the 
  scene does not appear streched
--------------------------------------------------------------------------------------------------------------*/
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH, (float)SCR_WIDTH / (float)SCR_HEIGHT);  

//Main loop, everything that happens is ordered here
int main()
{
    //Configures a window using GLFW library
    glfwInit();                                                                         //needs to be called to initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    std::cout << "GLFW initialized successfully" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                      //Sets major version of OpenGL to 3                                   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                                      //Sets the minor version of OpenGL to 3, so together with the major version its v3.3, which is what we are using
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                      //Sets the OpenGL profile, which in our case tells GLFW to use modern core profile of OpenGL instead of the old versions
#ifdef __APPLE__                                                                        //Checks if this code is being compiled on an macOS system
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                                //If that is the case, we set another hint because apple is special
#endif                                                                                  //If the code is compiled by a normal person, this code is skipped
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();                                     //Gets the primary monitor for fullscreen mode
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);                                //Retrieves video mode, which contains width, height, number of bits used for each color channel and refreshrate of the monitor

    //Creates a windowed with providet resolution, WINDOW NAME, and additional parameters for specifying the type of window we want to create
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Blaz", NULL, NULL);   //change 4th argument from NULL to monitor for fullscreen
    if (window == NULL)                                                                 //lastly we check if window creation was succesful, if not we print an error in the console and terminate GLFW
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);                                                     //OpenGL is a state machine, we therefore set where all the following OpenGL operations should be directed to: Our newly created window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);                  //sets a callback function, which is called when resizing a window. It adjusts the viewport and other operations 
    //initializes GLAD, leaves error if necessary
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                            
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "glad initialised" << std::endl;
    glEnable(GL_DEPTH_TEST);                                                            //enables depth testing in OpenGL. Depth testing is a process that ensures that pixels that are closer to the camera are drawn in front of pixels that are further away
    glEnable(GL_STENCIL_TEST);

    //face culling MAGICCCC!!!!                                                         Causes the faces inside voxels to not be rendered, boosting performance
    /*
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                        //This line hides the mouse cursor and locks it to the center of the window
    glfwSetCursorPosCallback(window, mouse_callback);                                   //This line sets a callback function (mouse_callback) that is called whenever the mouse moves within the window
    glfwSetScrollCallback(window, scroll_callback);                                     //This sets a callback function (scroll_callback) that is triggered whenever the user scrolls using the mouse wheel
    glfwSetWindowUserPointer(window, &camera);                                          //By setting the camera as the user pointer for the window, we can retrieve and manipulate the camera in your callback functions

    // Initialize ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Set ImGui style
    ImGui::StyleColorsDark(); // Or any other style

    // Initialize the platform and renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true); // `window` is your GLFW window
    ImGui_ImplOpenGL3_Init("#version 130"); // GLSL version

    Shader ourShader("shader.vs", "shader.fs");

//The following 2 lines of code ensure that VBO, VAO and texture generation operations occur, to manually change the vertex positions go to the RendererSetup.cpp and modify the vertices array
    // Declare variables for buffers and txtures and initializes renderer setup
    void setupBuffersAndTextures();
    setup();
    //Initialise our Shader program and load required textures
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;
    
    InitializeModels();
    showUnitStats = false;
    //This is where rendering and updating operations occur every frame
    while (!glfwWindowShouldClose(window))
    {
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if (timeDiff >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string title = "StrategyGame - " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, title.c_str());
            prevTime = crntTime;
            counter = 0;
        }
        float timeValue = glfwGetTime();

        processInput(window);

        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), camera.AspectRatio, 0.1f, 100.0f);

        // Set shader uniforms
        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        if (qDown) {
            ourShader.setBool("useLighting", true);
        }
        else if (eDown) {
            ourShader.setBool("useLighting", false);
        }
        float desiredValue = sin(timeValue) / 2.0f + 0.5f;
        ourShader.setVec4("ourColor", 0.0f, 1.0f, desiredValue, 1.0f);
        ourShader.setFloat("blazValue", blazValue);
        float time = glfwGetTime();
        float transitionValue = (sin(time) * 0.5f) + 0.5f;
        ourShader.setVec3("ambientLightColor", 0.8f, 0.8f, 0.8f);

        ourShader.setVec3("lightPos", 23.0f, 10.0f, 23.0f);
        ourShader.setVec3("lightColor", 0.4f, 0.4f, 0.2f);

        glm::vec3 lightSpec = glm::vec3(0.5f, 0.5f, 0.5f);
        ourShader.setVec3("lightSpecular", lightSpec.x, lightSpec.y, lightSpec.z);
        ourShader.setFloat("shininess", 32.0f);
        glm::vec3 materialSpec = glm::vec3(0.5f, 0.5f, 0.5f);
        ourShader.setVec3("materialSpecular", materialSpec.x, materialSpec.y, materialSpec.z);

        generateWorld(ourShader, 25, 1, "hillyFlats", 8, -0.30f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (perspective == 0) {
            drawAllSprites();
        }
        else {
            drawAllSpritesPerspectiveBased();
        }

        runGame(2, glfwGetTime());

        // Call UI functions
        DisplayMainUI(showUI, SCR_WIDTH, SCR_HEIGHT);
        DisplayTeamSelector(showUI, SCR_WIDTH, SCR_HEIGHT);
        if (rayCastHasTarget) {
            DisplayLookingAtInfo(SCR_WIDTH, SCR_HEIGHT, rayX, rayY, rayZ, rayModelID);
        }
        DisplayVersionInfo(SCR_WIDTH, SCR_HEIGHT);
        DisplayCurrentGameInfo(SCR_WIDTH, SCR_HEIGHT);
        if (perspective == 0) {
            DisplayAllUnitInfo(SCR_WIDTH, SCR_HEIGHT, view, projection);
        }
        else {
            DisplayAllUnitInfoInView(SCR_WIDTH, SCR_HEIGHT, view, projection);
        }
        RenderCrosshair(SCR_WIDTH, SCR_HEIGHT);
        if (focusedOnFaction) {
            RenderUnitStats(SCR_WIDTH, SCR_HEIGHT, selectedFaction);
            RenderActionSelect(SCR_WIDTH, SCR_HEIGHT, selectedFaction);
        }
        if (perspective == currentPlayer) {
            DisplayTurnControl(SCR_WIDTH, SCR_HEIGHT);
        }
        DisplayPerspectiveSelector(showUI, SCR_WIDTH, SCR_HEIGHT);
        RenderImGui();

        if (selectValuesChanged) {
            changeSelectValueBySprite(selectedFaction, includeVehicle, atValue, stValue, dtValue, vehicleTeam);
            selectValuesChanged = false;
        }

        if (perspective == 0) {
            if (bDown && bReleased) {
                bReleased = false;
                if (bToggle) {
                    bToggle = false;
                }
                else {
                    bToggle = true;
                }
            }
            if (!bReleased) {
                if (!bDown) {
                    bReleased = true;
                }
            }
        }
        else {
            bToggle = false;
        }

        if (fDown && fReleased) {
            fReleased = false;
            if (fToggle) {
                fToggle = false;
            }
            else {
                fToggle = true;
            }
        }
        if (!fReleased) {
            if (!fDown) {
                fReleased = true;
            }
        }

        rotationAngle += rotationSpeed * deltaTime; 
        
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
        //recapture cursor when outside uncaptured operations
        if (!bToggle && !focusedOnFaction && !fToggle) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorCaptured = true;
            actionSelected = 0;
        }
        //raycast
        glm::vec3 rayDir;
        if (cursorCaptured) {
            rayDir = getRayDirection(SCR_WIDTH / 2, SCR_HEIGHT / 2, view, projection, SCR_WIDTH, SCR_HEIGHT);
        } else {
            rayDir = getRayDirection(lastX, lastY, view, projection, SCR_WIDTH, SCR_HEIGHT);
        }
        glm::vec3 rayOrigin = camera.Position;
        glm::vec3 rayTarget;
        rayCastHasTarget = false;
        for (const auto& voxel : topVoxels) {
            glm::vec3 voxelMin = glm::vec3(voxel.x - 0.5f, voxel.y - 0.125f, voxel.z - 0.5f);
            glm::vec3 voxelMax = glm::vec3(voxel.x + 0.5f, voxel.y + 0.125f, voxel.z + 0.5f);
            if (rayIntersectsVoxel(rayOrigin, rayDir, voxelMin, voxelMax)) {
                rayTarget = glm::vec3(voxel.x, voxel.y, voxel.z);
                rayCastHasTarget = true; 
                break; 
            }
        }
        
        glm::vec3 currentRotation = ModelTransformations::arrow.rotation;
        currentRotation.y += rotationAngle;
        float bobbingOffset = bobbingAmplitude * sin(bobbingFrequency * time);
        float bobbingOffsetReverse = bobbingAmplitude * sin(bobbingFrequency * time - M_PI);
        glm::vec3 bobbingPosition = ModelTransformations::arrow.position + rayTarget;
        bobbingPosition.y += bobbingOffset;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (rayCastHasTarget && (fToggle || bToggle) && !OnUI(mouseX, mouseY, 1)) {
            arrow->Draw(ourShader, true, bobbingPosition, ModelTransformations::arrow.scale, currentRotation, 0, false, 0.5f);
        }
        rayX = rayTarget.x;
        rayY = rayTarget.y;
        rayZ = rayTarget.z;
        rayModelID = modelIDselected;

        bool messagePrinted = false;
        if (!leftMouseReleased) {
            if (!leftMouseDown) {
                leftMouseReleased = true;
                messagePrinted = false;
            }
        }
        if (fToggle) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorCaptured = false;
        }
        ///////////////////////////////////////////////////////////////////
        if (bToggle) {           
            showUI = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorCaptured = false;
            if (leftMouseDown && leftMouseReleased && !messagePrinted) {
                double mouseX, mouseY;
                glfwGetCursorPos(window, &mouseX, &mouseY);
                bool proceed = OnUI(mouseX, mouseY, 1);
                if (!proceed && modelIDselected != 0) {
                    addSprite(rayTarget.x, rayTarget.y, rayTarget.z, modelIDselected, currentTeam);
                    recalculateAllVision(allFactions, topVoxels, currentTeam);
                    visionRecalculationRequired = true;
                }
                messagePrinted = true; 
            }
        }
        else {
            showUI = false;
        }
        if (visionRecalculationRequired) {
            recalculateAllVision(allFactions, topVoxels, perspective);
            visionRecalculationRequired = false;
        }
        if (leftMouseDown && rayCastHasTarget && !bToggle && !focusedOnFaction && leftMouseReleased) {
            for (auto& sprite : allSprites) {
                if (sprite.x == rayTarget.x && sprite.y == rayTarget.y && sprite.z == rayTarget.z && sprite.team != 0) {
                    sprite.selected = true;
                    if (perspective != 0) {
                        for (auto& sprite : allSpritesInView) {
                            if (sprite.x == rayTarget.x && sprite.y == rayTarget.y && sprite.z == rayTarget.z && sprite.team != 0) {
                                sprite.selected = true;
                            }
                        }
                    }
                    focusedOnFaction = true;
                    selectedFaction = sprite.faction;
                    renderUnitStatsinitialized = false;
                }
                else {
                    sprite.selected = false;
                }
            }
            movementValidityCheck(rayTarget.x, rayTarget.y, rayTarget.z);
            lastRAYx = rayTarget.x;
            lastRAYy = rayTarget.y;
            lastRAYz = rayTarget.z;
            leftclickselect = true;
        }
        if (focusedOnFaction) {
            if (actionSelected == 4) {
                deselectEverything();
                focusedOnFaction = false;
            }
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorCaptured = false;
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            bool proceed = OnUI(mouseX, mouseY, 2);
            bool anySpritesSelected = false;
            if (actionSelected == 1) {
                for (auto& voxel : confirmedVoxels) {
                    glm::vec3 bobbingPosition = ModelTransformations::moveTo.position + voxel.position;
                    bobbingPosition.y += bobbingOffset;
                    moveTo->Draw(ourShader, true, bobbingPosition, ModelTransformations::moveTo.scale, currentRotation, 0, false, 0.5f);
                }
            }
            else if (actionSelected == 2) {
                for (auto& voxel : attackHighlightedVoxels) {
                    glm::vec3 bobbingPosition = ModelTransformations::moveTo.position + voxel;
                    bobbingPosition.y += bobbingOffset;
                    moveTo->Draw(ourShader, true, bobbingPosition, ModelTransformations::moveTo.scale, currentRotation, 0, false, 0.5f);
                }
            }

            if (!proceed && leftMouseDown) {
                std::cout << actionSelected << std::endl;
                if (actionSelected == 1) {
                    bool pass1 = false;
                    bool pass2 = false;
                    bool pass = false;
                    for (auto& sprite : allSprites) {
                        if (sprite.selected) {
                            anySpritesSelected = true;
                            if ((sprite.x == rayTarget.x && sprite.y == rayTarget.y && sprite.z == rayTarget.z) || !rayCastHasTarget) {
                                deselectEverything();
                                focusedOnFaction = false;
                                break;
                            }
                            else {
                                float movementLeftIg;
                                pass1 = validityCHeck(rayTarget.x, rayTarget.y, rayTarget.z, sprite.modelID, sprite.team);
                                if (!pass1) {
                                    deselectEverything();
                                    focusedOnFaction = false;
                                    break;
                                }
                                for (auto& voxel : confirmedVoxels) {
                                    if (rayTarget.x == voxel.position.x && rayTarget.y == voxel.position.y && rayTarget.z == voxel.position.z) {
                                        pass2 = true;
                                        sprite.movementRemaining = sprite.movementRemaining - voxel.movementSubtracted;
                                    }
                                }
                                if (pass1 && pass2) {
                                    pass = true;
                                }
                                else {
                                    // std::cout << "Illegal move." << std::endl;
                                }
                            }
                        }
                    }
                    if (!anySpritesSelected) {
                        deselectEverything();
                        focusedOnFaction = false;
                    }
                    if (pass) {
                        int originalX;
                        int teamInQuestion;
                        for (auto& sprite : allSprites) {
                            if (sprite.selected) {
                                teamInQuestion = sprite.team;
                                break;
                            }
                        }
                        float originalY;
                        int originalZ;
                        unsigned int theFaction;
                        for (auto& sprite : allSprites) {
                            if (sprite.selected) {
                                originalX = sprite.x;
                                originalY = sprite.y;
                                originalZ = sprite.z;
                                theFaction = sprite.faction;
                                break;
                            }
                        }
                        bool divide = false;
                        unsigned int stayTroopCounter = 0;
                        for (auto& sprite : allSprites) {
                            if (sprite.selected == false && sprite.x == originalX && sprite.y == originalY && sprite.z == originalZ && sprite.team != 0) {
                                sprite.antiClipPos = stayTroopCounter;
                                stayTroopCounter++;
                                divide = true;
                            }
                        }
                        unsigned int hostFaction;
                        bool hostExists = false;
                        unsigned int troopCounter = 0;
                        for (auto& sprite : allSprites) {
                            if (sprite.x == rayTarget.x && sprite.y == rayTarget.y && sprite.z == rayTarget.z && sprite.team != 0) {
                                hostFaction = sprite.faction;
                                hostExists = true;
                                if (sprite.modelID == 1 || sprite.modelID == 14 || sprite.modelID == 30) {
                                    troopCounter++;
                                }
                            }
                        }
                        if (divide) {
                            unsigned int theNewFaction;
                            if (!hostExists) {
                                theNewFaction = declareNewFaction(vehicleTeam);
                            }
                            for (auto& sprite : allSprites) {
                                if (sprite.selected) {
                                    allSpritesBeingRemoved.push_back(sprite);
                                    sprite.x = rayTarget.x;
                                    sprite.y = rayTarget.y;
                                    sprite.z = rayTarget.z;
                                    sprite.transparency = 0.0f;
                                    sprite.antiClipPos = troopCounter;
                                    if (sprite.modelID == 20 || sprite.modelID == 34 || sprite.modelID == 22 || sprite.modelID == 32 || sprite.modelID == 4 || sprite.modelID == 25) {
                                        for (auto& vehicle : vehicleContinentsList) {
                                            if (vehicle.x == originalX && vehicle.z == originalZ) {
                                                vehicle.x = rayTarget.x;
                                                vehicle.z = rayTarget.z;
                                            }
                                        }
                                    }
                                    troopCounter++;
                                    if (hostExists) {
                                        sprite.faction = hostFaction;
                                    }
                                    else {
                                        sprite.faction = theNewFaction;
                                    }
                                    sprite.previousFaction = theFaction;
                                }
                            }
                            if (!hostExists) {
                                Faction newFaction(rayTarget.x, rayTarget.y, rayTarget.z, vehicleTeam, theNewFaction, false);
                                allFactions.push_back(newFaction);
                            }
                        }
                        else {
                            if (!hostExists) {
                                for (auto& sprite : allSprites) {
                                    if (sprite.selected) {
                                        allSpritesBeingRemoved.push_back(sprite);
                                        sprite.x = rayTarget.x;
                                        sprite.y = rayTarget.y;
                                        sprite.z = rayTarget.z;
                                        sprite.transparency = 0.0f;
                                        sprite.antiClipPos = troopCounter;
                                        if (sprite.modelID == 20 || sprite.modelID == 34 || sprite.modelID == 22 || sprite.modelID == 32 || sprite.modelID == 4 || sprite.modelID == 25) {
                                            for (auto& vehicle : vehicleContinentsList) {
                                                if (vehicle.x == originalX && vehicle.z == originalZ) {
                                                    vehicle.x = rayTarget.x;
                                                    vehicle.z = rayTarget.z;
                                                }
                                            }
                                        }
                                        troopCounter++;
                                    }
                                }
                                for (auto& faction : allFactions) {
                                    if (faction.factionID == theFaction && faction.team == teamInQuestion) {
                                        faction.x = rayTarget.x;
                                        faction.y = rayTarget.y;
                                        faction.z = rayTarget.z;
                                    }
                                }
                            }
                            else {
                                for (auto& sprite : allSprites) {
                                    if (sprite.selected) {
                                        allSpritesBeingRemoved.push_back(sprite);
                                        sprite.x = rayTarget.x;
                                        sprite.y = rayTarget.y;
                                        sprite.z = rayTarget.z;
                                        sprite.transparency = 0.0f;
                                        sprite.antiClipPos = troopCounter;
                                        if (sprite.modelID == 20 || sprite.modelID == 34 || sprite.modelID == 22 || sprite.modelID == 32 || sprite.modelID == 4 || sprite.modelID == 25) {
                                            for (auto& vehicle : vehicleContinentsList) {
                                                if (vehicle.x == originalX && vehicle.z == originalZ) {
                                                    vehicle.x = rayTarget.x;
                                                    vehicle.z = rayTarget.z;
                                                }
                                            }
                                        }
                                        troopCounter++;
                                        sprite.faction = hostFaction;
                                        sprite.previousFaction = theFaction;
                                    }
                                }
                                unsigned int factionToRemove = theFaction;
                                auto factionIt = std::find_if(allFactions.begin(), allFactions.end(),
                                    [factionToRemove](const Faction& faction) {
                                        return faction.factionID == factionToRemove;
                                    });
                                if (factionIt != allFactions.end()) {
                                    allFactions.erase(factionIt);
                                }
                            }
                        }
                        deselectEverything();
                        focusedOnFaction = false;
                        recalculateAllVision(allFactions, topVoxels, perspective);
                        printAllSprites();
                        printConfirmedVoxels();
                    }
                }
                else if (actionSelected == 2) {
                    std::cout << "Function executed";
                }
            } 
        }
        topVoxels.clear();

        if (leftMouseDown && leftMouseReleased) {
            leftMouseReleased = false;
        }
        if (spaceDown)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwPollEvents();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    CleanupModels();
    void deleteBuffers();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        spaceDown = true;
    else
        spaceDown = false;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        qDown = true;
    else
        qDown = false;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        eDown = true;
    else
        eDown = false;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        leftMouseDown = true;
    else
        leftMouseDown = false;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        rightMouseDown = true;
    else
        rightMouseDown = false;

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        bDown = true;
    }
    else {
        bDown = false;
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        vDown = true;
    }
    else {
        vDown = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        fDown = true;
    }
    else {
        fDown = false;
    }


    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    const float cameraSpeed = 2.5f * 0.002f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    if (!bToggle && !focusedOnFaction && !fToggle) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->ProcessMouseScroll((float)yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->UpdateAspectRatio((float)width / (float)height);
}
