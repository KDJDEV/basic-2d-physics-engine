#include "math.h"

uint32_t frame_rate = 60;
uint32_t substeps = 700; //for solver
const char* windowTitle = "Physics Engine";
float pixelsPerMeter = 30;
math::Vector2<float> windowSizeInMeters{ 30, 30 };

math::Vector2<float> universalGravity = { 0.0f, 9.8f }; //in m/sec
float gravity = 0;
float minGravityDistance = 2; //When objects are this close together or closer gravity will stop acting; this is to prevent infinity gravity when objects are at the same positions

bool collisions = true;
float restitutionCoefficient = 1;

float dragCoefficient = 0; //try around 0.1

bool includePhaseSpace = true;
float phaseSpaceZoomSpeed = 0;
float phaseUnitPixelSizeX = 100 / 1.2;
float phaseUnitPixelSizeY = 80 / 1.2;


//This can be automatically computed
math::Vector2<float> windowSizeInPixels{ pixelsPerMeter * windowSizeInMeters.x, pixelsPerMeter * windowSizeInMeters.y };