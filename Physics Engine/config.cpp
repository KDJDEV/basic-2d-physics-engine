#include "math.h"

uint32_t frame_rate = 60;
uint32_t substeps = 700; //for solver
const char* windowTitle = "Physics Engine";
float pixelsPerMeter = 30;
math::Vector2<float> windowSizeInMeters{ 20, 20 };
bool darkMode = true;
bool circleShapeDotVisible = false;

math::Vector2<float> universalGravity = { 0.0f, 0.0f }; //in m/sec
float gravity = 0; //try 200
float minGravityDistance = 2; //When objects are this close together or closer gravity will stop acting; this is to prevent infinity gravity when objects are at the same positions

bool collisions = true;
float restitutionCoefficient = 1;

float dragCoefficient = 0; //try around 0.1

bool includePhaseSpace = false;
const char* phaseSpaceWindowTitle = "Physics Engine Phase Space";
float phaseSpaceZoomSpeed = 0;
float phaseUnitPixelSizeX = 100;
float phaseUnitPixelSizeY = 80;

float stopAtTime = -1; // -1 if you want it to run forever

//This can be automatically computed
math::Vector2<float> windowSizeInPixels{ pixelsPerMeter * windowSizeInMeters.x, pixelsPerMeter * windowSizeInMeters.y };
sf::Color primaryColor = darkMode ? sf::Color{ 255, 255, 255 } : sf::Color{ 0, 0, 0 };
sf::Color secondaryColor = darkMode ? sf::Color{ 0, 0, 0 } : sf::Color{ 255, 255, 255 };