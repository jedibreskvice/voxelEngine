#include "UIManager.h"
#include <GLFW/glfw3.h>
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "SpriteList.h"
//#include "RendererSetup.h"
#include "TurnSystem.h"

int perspective;
int currentTeam = 0;
int modelIDselected = 0;
int turnControlUISizeX = 220;
int turnControlUISizeY = 60;
int mainUISizeX = 400;
int mainUISizeY = 960;
int TeamUISizeX = 270;
int TeamUISizeY = 330;
int PerspectiveUISizeX = 800;
int PerspectiveUISizeY = 100;
int ActionUISizeX = 600;
int ActionUISizeY = 120;
int SelectUISizeX = 500;
int SelectUISizeY = 500;
int page = 1;
int unitMaxHousing = 0;
int troopPosX;
float troopPosY;
int troopPosZ;
float atValue = 0;
float stValue = 0;
float dtValue = 0;
float atValueIV = 0;
float stValueIV = 0;
float dtValueIV = 0;
int vehicleX;
int vehicleZ;
float vehicleY;
int vehicleTeam;
int vehicleFaction;
int vehicleID;
bool includeVehicle = true;
bool selectValuesChanged = false;
bool selectValuesChangedIV = false;
bool renderUnitStatsinitialized = false;
float assaultTroopCount = 0;
float sniperTroopCount = 0;
float droneTroopCount = 0;
int assaultTroopCountIV = 0;
int sniperTroopCountIV = 0;
int droneTroopCountIV = 0;
std::string vehicle = "None";
bool selectScreen;
bool showUnitStats;
bool actionMode;
unsigned int actionSelected = 0;
bool visionRecalculationRequired = false;
bool unitControled = true;
float lastRAYx;
float lastRAYy;
float lastRAYz;
bool leftclickselect = false;


ImVec2 windowPos(15, 15);
ImVec2 TeamwindowPos(1594, 346);
ImVec2 perspectiveWindowPos(500, 850);
ImVec2 ActionWindowPos(700, 700);
ImVec2 SelectScreenPos(1360, 170);
ImVec2 turnControlPos(1330, 850);

std::string arr[] = {
    "Alpha",   // A
    "Bravo",   // B
    "Charlie", // C
    "Delta",   // D
    "Echo",    // E
    "Foxtrot", // F
    "Golf",    // G
    "Hotel",   // H
    "India",   // I
    "Juliett", // J
    "Kilo",    // K
    "Lima",    // L
    "Mike",    // M
    "November",// N
    "Oscar",   // O
    "Papa",    // P
    "Quebec",  // Q
    "Romeo",   // R
    "Sierra",  // S
    "Tango",   // T
    "Uniform", // U
    "Victor",  // V
    "Whiskey", // W
    "X-ray",   // X
    "Yankee",  // Y
    "Zulu"     // Z
};

// Function to handle the main UI window
void DisplayMainUI(bool& showUI, float SCR_WIDTH, float SCR_HEIGHT)
{
    if (showUI)
    {
        ImGui::SetNextWindowPos(windowPos);
        ImGui::SetNextWindowSize(ImVec2(mainUISizeX, mainUISizeY));
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

        // Push style variables before beginning the window
        SetupImGuiStyle();

        // Before checking for hover, push a default window background color (semi-transparent)
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.1f, 0.1f, 0.1f, 0.2f));          // Scrollbar background color
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.1f, 0.1f, 0.1f, 0.3f));        // Scrollbar grab color
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.1f, 0.1f, 0.1f, 0.6f)); // Scrollbar grab hover color
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));  // Scrollbar grab active color

        // Begin the window
        ImGui::Begin("Build Screen", nullptr, window_flags);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));           // Button default color
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Button hover color
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));     // Button active (pressed) color

        if (page == 1) {

            if (ImGui::Button("Switch to buildings", ImVec2(300, 40)))
            {
                page = 2;
            }
            ImGui::Text("Combat units:");

            if (currentTeam != 0) {
                ImGui::Text("Troop type units:");
                if (ImGui::Button("AssaultTroop", ImVec2(300, 38)))
                {
                    modelIDselected = 1;
                }
                if (ImGui::Button("SniperTroop", ImVec2(300, 38)))
                {
                    modelIDselected = 30;
                }
                if (ImGui::Button("DroneTroop", ImVec2(300, 38)))
                {
                    modelIDselected = 14;
                }
            }
            ImGui::Text("Ground vehicles:");
            if (ImGui::Button("Jeep", ImVec2(300, 38)))
            {
                modelIDselected = 20;
            }
            if (ImGui::Button("Truck", ImVec2(300, 38)))
            {
                modelIDselected = 34;
            }
            if (ImGui::Button("Liberator", ImVec2(300, 38)))
            {
                modelIDselected = 22;
            }
            if (ImGui::Button("Tank", ImVec2(300, 38)))
            {
                modelIDselected = 32;
            }
            if (ImGui::Button("Artillery", ImVec2(300, 38)))
            {
                modelIDselected = 4;
            }
            if (ImGui::Button("MLRS", ImVec2(300, 38)))
            {
                modelIDselected = 25;
            }
            ImGui::Text("Naval vehicles:");
            if (ImGui::Button("Lifeboat", ImVec2(300, 38)))
            {
                modelIDselected = 23;
            }
            if (ImGui::Button("Battleship", ImVec2(300, 38)))
            {
                modelIDselected = 5;
            }
            if (ImGui::Button("Carrier", ImVec2(300, 38)))
            {
                modelIDselected = 8;
            }
            if (ImGui::Button("Submarine", ImVec2(300, 38)))
            {
                modelIDselected = 31;
            }
            ImGui::Text("Aerial vehicles:");
            if (ImGui::Button("Helicopter", ImVec2(300, 38)))
            {
                modelIDselected = 18;
            }
            if (ImGui::Button("Bomber", ImVec2(300, 38)))
            {
                modelIDselected = 7;
            }
            if (ImGui::Button("Jet", ImVec2(300, 38)))
            {
                modelIDselected = 21;
            }

        } else if (page == 2) {

            if (ImGui::Button("Switch to combat units", ImVec2(300, 40)))
            {
                page = 1;
            }
            ImGui::Text("Buildings:");

            ImGui::Text("Production based buildings:");
            if (ImGui::Button("Quary", ImVec2(300, 38)))
            {
                modelIDselected = 19;
            }
            if (ImGui::Button("Mine", ImVec2(300, 38)))
            {
                modelIDselected = 24;
            }
            if (ImGui::Button("Farm", ImVec2(300, 38)))
            {
                
            }
            if (ImGui::Button("Pumpjack", ImVec2(300, 38)))
            {

            }
            ImGui::Text("Military buildings:");
            if (ImGui::Button("Recruitment center", ImVec2(300, 38)))
            {
                modelIDselected = 27;
            }
            if (ImGui::Button("Garage", ImVec2(300, 38)))
            {
                modelIDselected = 17;
            }
            if (ImGui::Button("Factory", ImVec2(300, 38)))
            {
                modelIDselected = 15;
            }
            if (ImGui::Button("Airport", ImVec2(300, 38)))
            {
                modelIDselected = 3;
            }
            ImGui::Text("Defensive buildings:");
            if (ImGui::Button("Trench", ImVec2(300, 38)))
            {
                modelIDselected = 33;
            }
            if (ImGui::Button("Fort", ImVec2(300, 38)))
            {
                modelIDselected = 16;
            }
            if (ImGui::Button("SAM site", ImVec2(300, 38)))
            {
                modelIDselected = 28;
            }
            if (ImGui::Button("C-RAM", ImVec2(300, 38)))
            {
                modelIDselected = 12;
            }
            ImGui::Text("Power plants:");
            if (ImGui::Button("Combustion power plant", ImVec2(300, 38)))
            {
                modelIDselected = 11;
            }
            if (ImGui::Button("Nuclear power plant", ImVec2(300, 38)))
            {
                modelIDselected = 26;
            }
            if (ImGui::Button("Wind farm", ImVec2(300, 38)))
            {
                modelIDselected = 36;
            }
            ImGui::Text("Processing buildings:");
            if (ImGui::Button("Chemical plant", ImVec2(300, 38)))
            {
                modelIDselected = 9;
            }
            if (ImGui::Button("Smelting facility", ImVec2(300, 38)))
            {
                modelIDselected = 29;
            }
            ImGui::Text("Storage buildings:");
            if (ImGui::Button("Warehouse", ImVec2(300, 38)))
            {
                modelIDselected = 35;
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::End();
        ImGui::PopStyleColor(4);
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(5); 
    }
}

void DisplayTeamSelector(bool& showUI, float SCR_WIDTH, float SCR_HEIGHT)
{
    if (showUI) {
        ImGui::SetNextWindowPos(TeamwindowPos);
        ImGui::SetNextWindowSize(ImVec2(TeamUISizeX, TeamUISizeY));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        SetupImGuiStyle();
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

        ImGui::Begin("Team selector display", nullptr, window_flags);
        ImGui::Text("Team of the next unit:");

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));           // Button default color
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Button hover color
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));     // Button active (pressed) color

        if (ImGui::Button("No team", ImVec2(250, 38)))
        {
            currentTeam = 0;
        }
        if (ImGui::Button("Blue", ImVec2(250, 38)))
        {
            currentTeam = 1;
        }
        if (ImGui::Button("Red", ImVec2(250, 38)))
        {
            currentTeam = 2;
        }
        if (ImGui::Button("Green", ImVec2(250, 38)))
        {
            currentTeam = 3;
        }
        if (ImGui::Button("Orange", ImVec2(250, 38)))
        {
            currentTeam = 4;
        }
        if (ImGui::Button("Yellow", ImVec2(250, 38)))
        {
            currentTeam = 5;
        }
        if (ImGui::Button("Purple", ImVec2(250, 38)))
        {
            currentTeam = 6;
        }
        ImGui::PopStyleColor(3);
        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(5);
    }
    
}

void DisplayPerspectiveSelector(bool& showUI, float SCR_WIDTH, float SCR_HEIGHT)
{
    ImGui::SetNextWindowPos(perspectiveWindowPos);
    ImGui::SetNextWindowSize(ImVec2(PerspectiveUISizeX, PerspectiveUISizeY));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Perspective selector display", nullptr, window_flags);
    ImGui::Text("Perspective of:");

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));           // Button default color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Button hover color
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));     // Button active (pressed) color

    if (ImGui::Button("All", ImVec2(100, 38))) { perspective = 0; visionRecalculationRequired = true;}
    ImGui::SameLine();
    if (listOfAllPlayers.size() >= 1) {
        if (ImGui::Button("Blue", ImVec2(100, 38))) { perspective = 1; visionRecalculationRequired = true; }
        ImGui::SameLine();
    }
    if (listOfAllPlayers.size() >= 2) {
        if (ImGui::Button("Red", ImVec2(100, 38))) { perspective = 2; visionRecalculationRequired = true; }
        ImGui::SameLine();
    }
    if (listOfAllPlayers.size() >= 3) {
        if (ImGui::Button("Green", ImVec2(100, 38))) { perspective = 3; visionRecalculationRequired = true; }
        ImGui::SameLine();
    }
    if (listOfAllPlayers.size() >= 4) {
        if (ImGui::Button("Orange", ImVec2(100, 38))) { perspective = 4; visionRecalculationRequired = true; }
        ImGui::SameLine();
    }
    if (listOfAllPlayers.size() >= 5) {
        if (ImGui::Button("Yellow", ImVec2(100, 38))) { perspective = 5; visionRecalculationRequired = true; }
        ImGui::SameLine();
    }
    if (listOfAllPlayers.size() >= 6) {
        if (ImGui::Button("Purple", ImVec2(100, 38))) { perspective = 6; visionRecalculationRequired = true; }
    }

    ImGui::PopStyleColor(3);
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
}

void DisplayTurnControl(float SCR_WIDTH, float SCR_HEIGHT)
{
    ImGui::SetNextWindowPos(turnControlPos);
    ImGui::SetNextWindowSize(ImVec2(turnControlUISizeX, turnControlUISizeY));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Turn control display", nullptr, window_flags);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));           // Button default color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Button hover color
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));     // Button active (pressed) color

    if (ImGui::Button("Pass turn to next player", ImVec2(200, 38))) { 
        passToNextPlayer();
    }
    ImGui::PopStyleColor(3);
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
}

void DisplayVersionInfo(float SCR_WIDTH, float SCR_HEIGHT)
{
    ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH * 0.83f, SCR_HEIGHT * 0.87f));
    ImGui::SetNextWindowSize(ImVec2(270, 25));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Version display", nullptr, window_flags);
    ImGui::Text("Alpha version - Work in progress");
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
}

void DisplayCurrentGameInfo(float SCR_WIDTH, float SCR_HEIGHT)
{
    ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH * 0.33f, SCR_HEIGHT * 0.03f));
    ImGui::SetNextWindowSize(ImVec2(580, 80));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Current game display", nullptr, window_flags);
    ImGui::Text("Time remaining: %.0f seconds", timeRemaining);
    ImGui::Text("Currently playing: Player: %u ", currentPlayer);
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
}

void DisplayLookingAtInfo(float SCR_WIDTH, float SCR_HEIGHT, float x, float y, float z, int modelID)
{
    ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH * 0.83f, SCR_HEIGHT * 0.67f));
    ImGui::SetNextWindowSize(ImVec2(270, 200));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Looking at display", nullptr, window_flags);
    ImGui::Text("Looking at:");
    ImGui::Text("X: %.2f", x);
    ImGui::Text("Y: %.2f", y);
    ImGui::Text("Z: %.2f", z);
    ImGui::Text("Model ID: %d", modelID);
    ImGui::Text("Team: %d", currentTeam);
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
}

glm::vec2 WorldToScreen(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, float SCR_WIDTH, float SCR_HEIGHT)
{
    // Convert world position to clip space by multiplying with view and projection matrices
    glm::vec4 clipSpacePos = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1.0f);

    // Perform perspective division to normalize to screen coordinates
    glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos) / clipSpacePos.w;

    // Convert to window/screen space
    glm::vec2 screenSpacePos;
    screenSpacePos.x = (ndcSpacePos.x * 0.5f + 0.5f) * SCR_WIDTH;
    screenSpacePos.y = (1.0f - (ndcSpacePos.y * 0.5f + 0.5f)) * SCR_HEIGHT;  // Y is flipped in screen space

    return screenSpacePos;
}

void DisplayUnitInfo(float SCR_WIDTH, float SCR_HEIGHT, const glm::vec3& troopPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int factionID, int team)
{
    // Get 2D screen position from troop's world position
    glm::vec2 screenPos = WorldToScreen(troopPos, viewMatrix, projectionMatrix, SCR_WIDTH, SCR_HEIGHT);

    // Adjust to position the UI above the unit (e.g., offset the Y position)
    screenPos.y -= 50.0f;  // Offset to position the window above the unit

    unsigned int sizeX = static_cast<unsigned int>(270);
    unsigned int sizeY = static_cast<unsigned int>(270);

    // Set the window position using the 2D screen position
    ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY));
    ImGui::SetNextWindowPos(ImVec2(screenPos.x - sizeX / 2, screenPos.y - sizeY / 2));

    // Window flags to remove unnecessary UI elements
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;

    // Custom ImGui style
    SetupImGuiStyle();

    // Create a unique window name based on the factionID
    std::string windowName = "Unit info display " + std::to_string(factionID) + std::to_string(team);

    // Start ImGui window with a unique name
    ImGui::Begin(windowName.c_str(), nullptr, window_flags);

    // Draw the circle and text (same as before)
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 center = ImVec2(window_pos.x + window_size.x / 2, window_pos.y + window_size.y / 2);
    float radius = 60.0f ;  // Circle radius scaled

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddCircleFilled(center, radius, ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.9f, 0.15f)), 64);

    // Position text inside the circle
    ImGui::SetCursorScreenPos(ImVec2(center.x - 40, center.y - 35));

    // Scale the font for larger text
    ImFont* currentFont = ImGui::GetFont();
    float originalSize = currentFont->FontSize;  // Store the original font size
    currentFont->FontSize *= 0.5;         // Scale up the font size

    ImGui::Text(arr[factionID - 1].c_str());     // Draw the text with the new size

    // Restore the original font size
    currentFont->FontSize = originalSize;
    ImGui::SetCursorScreenPos(ImVec2(center.x - 50, center.y - 5));
    ImGui::Text("D I V I S I O N");

    // End the ImGui window
    ImGui::End();

    // Pop the style variables that were pushed
    ImGui::PopStyleVar(5);
}

void DisplayAllUnitInfo(float SCR_WIDTH, float SCR_HEIGHT, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    for (const auto& faction : allFactions) {
        glm::vec3 troopPos = glm::vec3(faction.x, faction.y, faction.z);
        DisplayUnitInfo(SCR_WIDTH, SCR_HEIGHT, troopPos, viewMatrix, projectionMatrix, faction.factionID, faction.team);
    }
}

void DisplayAllUnitInfoInView(float SCR_WIDTH, float SCR_HEIGHT, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    for (const auto& faction : allFactionsInView) {
        glm::vec3 troopPos = glm::vec3(faction.x, faction.y, faction.z);
        DisplayUnitInfo(SCR_WIDTH, SCR_HEIGHT, troopPos, viewMatrix, projectionMatrix, faction.factionID, faction.team);
    }
}

void RenderCrosshair(float SCR_WIDTH, float SCR_HEIGHT)
{
    // Get the center of the screen
    ImVec2 center(SCR_WIDTH / 2, SCR_HEIGHT / 2);

    // Define the size of the crosshair lines
    float lineLength = 15.0f;  // Length of the crosshair lines
    float lineThickness = 2.0f; // Thickness of the crosshair lines

    // Get the draw list for ImGui (allows drawing shapes)
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    // Set crosshair color
    ImU32 crosshairColor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White color

    // Draw horizontal line
    drawList->AddLine(
        ImVec2(center.x - lineLength, center.y),  // Start point
        ImVec2(center.x + lineLength, center.y),  // End point
        crosshairColor,                           // Color
        lineThickness                             // Thickness
    );

    // Draw vertical line
    drawList->AddLine(
        ImVec2(center.x, center.y - lineLength),  // Start point
        ImVec2(center.x, center.y + lineLength),  // End point
        crosshairColor,                           // Color
        lineThickness                             // Thickness
    );
}

void RenderUnitStats(float SCR_WIDTH, float SCR_HEIGHT, unsigned int selectedFaction) {
    ImGui::SetNextWindowPos(SelectScreenPos);
    ImGui::SetNextWindowSize(ImVec2(SelectUISizeX, SelectUISizeY));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle(); 
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Unit stats", nullptr, window_flags);
    ImGui::Text("Health:");
    ImGui::Text("Move:");
    ImGui::Text("Attack:");
    ImGui::Text("Split faction:");

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));           // Button default color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Button hover color
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));     // Button active (pressed) color

    if (!renderUnitStatsinitialized) {
        assaultTroopCount = 0;
        sniperTroopCount = 0;
        droneTroopCount = 0;
        assaultTroopCountIV = 0;
        sniperTroopCountIV = 0;
        droneTroopCountIV = 0;
        vehicle = "None";
        for (const auto& sprite : allSprites) {
            if (sprite.x == lastRAYx && sprite.z == lastRAYz && sprite.team != 0) {
                if (sprite.modelID == 1) {
                    assaultTroopCount++;
                    troopPosX = sprite.x;
                    troopPosY = sprite.y;
                    troopPosZ = sprite.z;
                    vehicleTeam = sprite.team;
                    vehicleFaction = sprite.faction;
                } else
                if (sprite.modelID == 30) {
                    sniperTroopCount++;
                    troopPosX = sprite.x;
                    troopPosY = sprite.y;
                    troopPosZ = sprite.z;
                    vehicleTeam = sprite.team;
                    vehicleFaction = sprite.faction;
                } else
                if (sprite.modelID == 14) {
                    droneTroopCount++;
                    troopPosX = sprite.x;
                    troopPosY = sprite.y;
                    troopPosZ = sprite.z;
                    vehicleTeam = sprite.team;
                    vehicleFaction = sprite.faction;
                } else
                if (sprite.modelID == 20) {
                    unitControled = true;
                    vehicle = "Jeep";
                    vehicleID = 20;
                    vehicleX = sprite.x;
                    vehicleZ = sprite.z;
                    vehicleY = sprite.y;
                    vehicleTeam = sprite.team;
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == sprite.x && vehicleCont.z == sprite.z) {
                            assaultTroopCountIV = vehicleCont.ATs;
                            sniperTroopCountIV = vehicleCont.STs;
                            droneTroopCountIV = vehicleCont.DTs;
                            break;
                        }
                    }
                } else
                if (sprite.modelID == 34) {
                    unitControled = true;
                    vehicle = "Truck";
                    vehicleID = 34;
                    vehicleX = sprite.x;
                    vehicleZ = sprite.z;
                    vehicleY = sprite.y;
                    vehicleTeam = sprite.team;
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == sprite.x && vehicleCont.z == sprite.z) {
                            assaultTroopCountIV = vehicleCont.ATs;
                            sniperTroopCountIV = vehicleCont.STs;
                            droneTroopCountIV = vehicleCont.DTs;
                            break;
                        }
                    }
                } else
                if (sprite.modelID == 22) {
                    unitControled = true;
                    vehicle = "Liberator";
                    vehicleID = 22;
                    vehicleX = sprite.x;
                    vehicleZ = sprite.z;
                    vehicleY = sprite.y;
                    vehicleTeam = sprite.team;
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == sprite.x && vehicleCont.z == sprite.z) {
                            assaultTroopCountIV = vehicleCont.ATs;
                            sniperTroopCountIV = vehicleCont.STs;
                            droneTroopCountIV = vehicleCont.DTs;
                            break;
                        }
                    }
                } else
                if (sprite.modelID == 32) {
                    unitControled = true;
                    vehicle = "Tank";
                    vehicleID = 32;
                    vehicleX = sprite.x;
                    vehicleZ = sprite.z;
                    vehicleY = sprite.y;
                    vehicleTeam = sprite.team;
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == sprite.x && vehicleCont.z == sprite.z) {
                            assaultTroopCountIV = vehicleCont.ATs;
                            sniperTroopCountIV = vehicleCont.STs;
                            droneTroopCountIV = vehicleCont.DTs;
                            break;
                        }
                    }
                } else
                if (sprite.modelID == 4) {
                    unitControled = true;
                    vehicle = "Artillery";
                    vehicleID = 4;
                    vehicleX = sprite.x;
                    vehicleZ = sprite.z;
                    vehicleY = sprite.y;
                    vehicleTeam = sprite.team;
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == sprite.x && vehicleCont.z == sprite.z) {
                            assaultTroopCountIV = vehicleCont.ATs;
                            sniperTroopCountIV = vehicleCont.STs;
                            droneTroopCountIV = vehicleCont.DTs;
                            break;
                        }
                    }
                } else
                if (sprite.modelID == 25) {
                    unitControled = true;
                    vehicle = "MLRS";
                    vehicleID = 25;
                    vehicleX = sprite.x;
                    vehicleZ = sprite.z;
                    vehicleY = sprite.y;
                    vehicleTeam = sprite.team;
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == sprite.x && vehicleCont.z == sprite.z) {
                            assaultTroopCountIV = vehicleCont.ATs;
                            sniperTroopCountIV = vehicleCont.STs;
                            droneTroopCountIV = vehicleCont.DTs;
                            break;
                        }
                    }
                }
            }
        }
        if (vehicle == "None") { //check for uncontroled units
            for (auto& sprite : allSprites) {
                if (sprite.x == troopPosX && sprite.z == troopPosZ && sprite.team == 0) {                  
                    unitControled = false;
                    if (sprite.modelID == 20) {
                        vehicle = "Jeep";
                        vehicleID = 20;
                        unitMaxHousing = unitStatistics[3].housing;
                    } else
                    if (sprite.modelID == 34) {
                        vehicle = "Truck";
                        vehicleID = 34;
                        unitMaxHousing = unitStatistics[4].housing;
                    } else
                    if (sprite.modelID == 22) {
                        vehicle = "Liberator";
                        vehicleID = 22;
                        unitMaxHousing = unitStatistics[5].housing;
                    } else
                    if (sprite.modelID == 32) {
                        vehicle = "Tank";
                        vehicleID = 32;
                        unitMaxHousing = unitStatistics[6].housing;
                    } else
                    if (sprite.modelID == 4) {
                        vehicle = "Artillery";
                        vehicleID = 4;
                        unitMaxHousing = unitStatistics[7].housing;
                    } else
                    if (sprite.modelID == 25) {
                        vehicle = "MLRS";
                        vehicleID = 25;
                        unitMaxHousing = unitStatistics[8].housing;
                    }
                }
            }
        }
        if (leftclickselect) {
            atValue = assaultTroopCount;
            stValue = sniperTroopCount;
            dtValue = droneTroopCount;
            leftclickselect = false;
        }
        else {
            atValue = 0;
            stValue = 0;
            dtValue = 0;
        }
        atValueIV = 0;
        stValueIV = 0;
        dtValueIV = 0;
        renderUnitStatsinitialized = true; 
    }
    if (vehicle == "None") {
        ImGui::Text("No vehicle", vehicle.c_str());
        movementValidityCheck(troopPosX, troopPosY, troopPosZ);
    }
    else {
        ImGui::Text("Vehicle:", vehicle.c_str());
        if (ImGui::Button("Include vehicle", ImVec2(250, 38)))
        {
            movementValidityCheck(troopPosX, troopPosY, troopPosZ);
            if (includeVehicle) {
                includeVehicle = false;
                selectValuesChanged = true;
            }
            else {
                includeVehicle = true;
                selectValuesChanged = true;
            }
        }
    }
    if (assaultTroopCount > 0) {
        if (ImGui::SliderFloat(" Assault troops", &atValue, 0, assaultTroopCount)) {
            atValue = roundf(atValue);
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            selectValuesChanged = true;
            movementValidityCheck(troopPosX, troopPosY, troopPosZ);
        }
    }
    if (sniperTroopCount > 0) {
        if (ImGui::SliderFloat(" Sniper troops", &stValue, 0, sniperTroopCount)) {
            stValue = roundf(stValue);
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            selectValuesChanged = true;
            movementValidityCheck(troopPosX, troopPosY, troopPosZ);
        }
    }
    if (droneTroopCount > 0) {
        if (ImGui::SliderFloat(" Drone troops", &dtValue, 0, droneTroopCount)) {
            dtValue = roundf(dtValue);
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            selectValuesChanged = true;
            movementValidityCheck(troopPosX, troopPosY, troopPosZ);
        }
    }
    if (vehicle != "None" && unitControled) {
        unsigned int HP = (assaultTroopCountIV + sniperTroopCountIV + droneTroopCountIV);
        ImGui::Text("Vehicle continents:            (HP = %d)", HP);
        ImGui::Text("Assault troops: %d", assaultTroopCountIV);
        ImGui::Text("Sniper troops: %d", sniperTroopCountIV);
        ImGui::Text("Drone troops: %d", droneTroopCountIV);

        if (assaultTroopCountIV > 0) {
            if (ImGui::SliderFloat(" Assault troops exiting", &atValueIV, 0, assaultTroopCountIV)) {
                atValueIV = roundf(atValueIV);
            }
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                selectValuesChangedIV = true;
            }
        }
        if (sniperTroopCountIV > 0) {
            if (ImGui::SliderFloat(" Sniper troops exiting", &stValueIV, 0, sniperTroopCountIV)) {
                stValueIV = roundf(stValueIV);
            }
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                selectValuesChangedIV = true;
            }
        }
        if (droneTroopCountIV > 0) {
            if (ImGui::SliderFloat(" Drone troops exiting", &dtValueIV, 0, droneTroopCountIV)) {
                dtValueIV = roundf(dtValueIV);
            }
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                selectValuesChangedIV = true;
            }
        }
        if (selectValuesChangedIV) {
            if ((atValueIV + stValueIV + dtValueIV) != 0) {
                if (ImGui::Button("Unload troops", ImVec2(250, 38))) {
                    for (auto& vehicleCont : vehicleContinentsList) {
                        if (vehicleCont.x == vehicleX && vehicleCont.z == vehicleZ) {
                            vehicleCont.ATs = vehicleCont.ATs - atValueIV;
                            vehicleCont.STs = vehicleCont.STs - stValueIV;
                            vehicleCont.DTs = vehicleCont.DTs - dtValueIV;
                            for (unsigned int i = 0; i < atValueIV; i++) {
                                addSprite(vehicleX, vehicleY, vehicleZ, 1, vehicleTeam);
                            }
                            for (unsigned int i = 0; i < stValueIV; i++) {
                                addSprite(vehicleX, vehicleY, vehicleZ, 30, vehicleTeam);
                            }
                            for (unsigned int i = 0; i < dtValueIV; i++) {
                                addSprite(vehicleX, vehicleY, vehicleZ, 14, vehicleTeam);
                            }
                            if ((vehicleCont.ATs + vehicleCont.STs + vehicleCont.DTs) == 0) {
                                for (auto& sprite : allSprites) {
                                    if (sprite.modelID == vehicleID) {
                                        if (sprite.x == vehicleX && sprite.z == vehicleZ) {
                                            sprite.team = 0;
                                            sprite.faction = 0;
                                            sprite.selected = false;
                                        }
                                    }
                                }
                                int localVehicleX = vehicleX;
                                int localVehicleZ = vehicleZ;
                                vehicleContinentsList.erase(
                                    std::remove_if(vehicleContinentsList.begin(), vehicleContinentsList.end(),
                                        [localVehicleX, localVehicleZ](const VehicleContinents& vc) {
                                            return vc.x == vehicleX && vc.z == vehicleZ;
                                        }),
                                    vehicleContinentsList.end());
                            }
                            renderUnitStatsinitialized = false;
                        }
                    }
                    selectValuesChangedIV = false;   
                }
            }
        }
        if ((atValue + stValue + dtValue) <= (unitMaxHousing - HP) && (atValue + stValue + dtValue) > 0) {
            if (ImGui::Button("Add selected units", ImVec2(250, 38)))
            {
                for (auto& stat : vehicleContinentsList) {
                    if (stat.x == vehicleX && stat.z == vehicleZ) {
                        stat.ATs += atValue;
                        stat.STs += stValue;
                        stat.DTs += dtValue;
                    }
                }
                int totalSpritesToRemove = atValue + stValue + dtValue;
                int spritesRemoved = 0;

                for (auto it = allSprites.begin(); it != allSprites.end() && spritesRemoved < totalSpritesToRemove;) {
                    if (it->modelID == 1 && it->selected) {
                        Sprite spriteToRemove = { it->x, it->y, it->z, it->modelID, it->team, it->antiClipPos, it->faction, it->previousFaction, it->selected };
                        removeSprite(allSprites, spriteToRemove);
                        spritesRemoved++;
                        it = allSprites.begin();
                        continue;
                    }
                    if (it->modelID == 30 && it->selected) {
                        Sprite spriteToRemove = { it->x, it->y, it->z, it->modelID, it->team, it->antiClipPos, it->faction, it->previousFaction, it->selected };
                        removeSprite(allSprites, spriteToRemove);
                        spritesRemoved++;
                        it = allSprites.begin();
                        continue;
                    }
                    if (it->modelID == 14 && it->selected) {
                        Sprite spriteToRemove = { it->x, it->y, it->z, it->modelID, it->team, it->antiClipPos, it->faction, it->previousFaction, it->selected };
                        removeSprite(allSprites, spriteToRemove);
                        spritesRemoved++;
                        it = allSprites.begin();
                        continue;
                    }
                    ++it;
                }
                renderUnitStatsinitialized = false;
            }
        }
    }
    if (vehicle != "None" && !unitControled) {
        ImGui::Text("Vehicle uncontroled, add units to control and repair it:");
        if ((atValue + stValue + dtValue) <= unitMaxHousing && (atValue + stValue + dtValue) > 0) {
            if (ImGui::Button("Add selected units", ImVec2(250, 38)))
            {
                for (auto& sprite : allSprites) {
                    if (sprite.team == 0 && sprite.x == troopPosX && sprite.z == troopPosZ) {
                        sprite.team = vehicleTeam;
                        sprite.faction = vehicleFaction;
                        unsigned int atValUint = atValue;
                        unsigned int stValUint = stValue;
                        unsigned int dtValUint = dtValue;
                        vehicleContinentsList.push_back({ troopPosX, troopPosZ, atValUint, stValUint, dtValUint });
                    }
                }

                int totalSpritesToRemove = atValue + stValue + dtValue;
                int spritesRemoved = 0;

                for (auto it = allSprites.begin(); it != allSprites.end() && spritesRemoved < totalSpritesToRemove;) {
                    if (it->modelID == 1 && it->selected) {
                        Sprite spriteToRemove = { it->x, it->y, it->z, it->modelID, it->team, it->antiClipPos, it->faction, it->previousFaction, it->selected };
                        removeSprite(allSprites, spriteToRemove);
                        spritesRemoved++;
                        it = allSprites.begin(); 
                        continue; 
                    }
                    if (it->modelID == 30 && it->selected) {
                        Sprite spriteToRemove = { it->x, it->y, it->z, it->modelID, it->team, it->antiClipPos, it->faction, it->previousFaction, it->selected };
                        removeSprite(allSprites, spriteToRemove);
                        spritesRemoved++; 
                        it = allSprites.begin(); 
                        continue; 
                    }
                    if (it->modelID == 14 && it->selected) {
                        Sprite spriteToRemove = { it->x, it->y, it->z, it->modelID, it->team, it->antiClipPos, it->faction, it->previousFaction, it->selected };
                        removeSprite(allSprites, spriteToRemove);
                        spritesRemoved++; 
                        it = allSprites.begin(); 
                        continue; 
                    }
                    ++it;
                }
                renderUnitStatsinitialized = false;
            }
        }
    }

    ImGui::PopStyleColor(3);
    ImGui::End();
    ImGui::PopStyleColor();  
    ImGui::PopStyleVar(5);   
}

void RenderActionSelect(float SCR_WIDTH, float SCR_HEIGHT, unsigned int selectedFaction) {
    ImGui::SetNextWindowPos(ActionWindowPos);
    ImGui::SetNextWindowSize(ImVec2(ActionUISizeX, ActionUISizeY));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

    SetupImGuiStyle();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 0.15f));

    ImGui::Begin("Action selector display", nullptr, window_flags);
    ImGui::Text("Team of the next unit:");

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));           // Button default color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));    // Button hover color
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));     // Button active (pressed) color

    if (ImGui::Button("Move", ImVec2(100, 38))) {
        actionSelected = 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("Attack", ImVec2(100, 38))) {
        actionSelected = 2;
    }
    ImGui::SameLine();
    if (ImGui::Button("Action", ImVec2(100, 38))) {
        actionSelected = 3;
    }
    ImGui::SameLine();
    if (ImGui::Button("Deselect", ImVec2(100, 38))) {
        actionSelected = 4;
    }

    ImGui::PopStyleColor(3);
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
}

void SetupImGuiStyle()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));  // Padding inside windows
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(13, 13));   // Padding inside UI elements
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 1));    // Space between elements
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 15.0f);           // Size of scrollbars
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);            // Rounding of frame corner
}

void RenderImGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool OnUI(double &mouseX, double &mouseY, unsigned int uiChosen) {

    if (uiChosen == 1) {
        if (mouseX > windowPos.x && mouseX <= (windowPos.x + mainUISizeX) && mouseY > windowPos.y && mouseY <= (windowPos.y + mainUISizeY)) {
            return true;
        }
        else if (mouseX > TeamwindowPos.x && mouseX <= (TeamwindowPos.x + TeamUISizeX) && mouseY > TeamwindowPos.y && mouseY <= (TeamwindowPos.y + TeamUISizeY)) {
            return true;
        }
        else if (mouseX > perspectiveWindowPos.x && mouseX <= (perspectiveWindowPos.x + PerspectiveUISizeX) && mouseY > perspectiveWindowPos.y && mouseY <= (perspectiveWindowPos.y + PerspectiveUISizeY)) {
            return true;
        }
        else if (mouseX > turnControlPos.x && mouseX <= (turnControlPos.x + turnControlUISizeX) && mouseY > turnControlPos.y && mouseY <= (turnControlPos.y + turnControlUISizeY)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (uiChosen == 2) {
        if (mouseX > ActionWindowPos.x && mouseX <= (ActionWindowPos.x + ActionUISizeX) && mouseY > ActionWindowPos.y && mouseY <= (ActionWindowPos.y + ActionUISizeY)) {
            return true;
        }
        else if (mouseX > SelectScreenPos.x && mouseX <= (SelectScreenPos.x + SelectUISizeX) && mouseY > SelectScreenPos.y && mouseY <= (SelectScreenPos.y + SelectUISizeY)) {
            return true;
        }
        else if (mouseX > turnControlPos.x && mouseX <= (turnControlPos.x + turnControlUISizeX) && mouseY > turnControlPos.y && mouseY <= (turnControlPos.y + turnControlUISizeY)) {
            return true;
        }
        else {
            return false;
        }
    }
}