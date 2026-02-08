#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <imgui.h>
#include <iostream>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Forward declare your functions to handle different UI sections
extern int perspective;
extern int currentTeam;
extern int modelIDselected;
extern bool selectScreen;
extern bool showUnitStats;
extern bool actionMode;
extern int vehicleTeam;
extern float atValue;
extern float stValue;
extern float dtValue;
extern float atValueIV;
extern float stValueIV;
extern float dtValueIV;
extern float lastRAYx;
extern float lastRAYy;
extern float lastRAYz;
extern bool includeVehicle;
extern bool selectValuesChanged;
extern bool selectValuesChangedIV;
extern bool renderUnitStatsinitialized;
extern unsigned int actionSelected;
extern bool visionRecalculationRequired;
extern bool leftclickselect;
void DisplayMainUI(bool& showUI, float SCR_WIDTH, float SCR_HEIGHT);
void DisplayVersionInfo(float SCR_WIDTH, float SCR_HEIGHT);
void DisplayCurrentGameInfo(float SCR_WIDTH, float SCR_HEIGHT);
void DisplayLookingAtInfo(float SCR_WIDTH, float SCR_HEIGHT, float x, float y, float z, int modelID);
void DisplayTeamSelector(bool& showUI, float SCR_WIDTH, float SCR_HEIGHT);
void DisplayUnitInfo(float SCR_WIDTH, float SCR_HEIGHT, const glm::vec3& troopPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, unsigned int factionID);
void DisplayAllUnitInfo(float SCR_WIDTH, float SCR_HEIGHT, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
void DisplayAllUnitInfoInView(float SCR_WIDTH, float SCR_HEIGHT, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
void RenderCrosshair(float SCR_WIDTH, float SCR_HEIGHT);
void RenderUnitStats(float SCR_WIDTH, float SCR_HEIGHT, unsigned int selectedFaction);
void RenderActionSelect(float SCR_WIDTH, float SCR_HEIGHT, unsigned int selectedFaction);
void DisplayPerspectiveSelector(bool& showUI, float SCR_WIDTH, float SCR_HEIGHT);
void DisplayTurnControl(float SCR_WIDTH, float SCR_HEIGHT);
void SetupImGuiStyle();
void RenderImGui();
bool OnUI(double& mouseX, double& mouseY, unsigned int uiChosen);

#endif // UI_MANAGER_H