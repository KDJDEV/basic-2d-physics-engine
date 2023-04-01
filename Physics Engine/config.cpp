#include "math.h"

uint32_t frame_rate = 60;
uint32_t substeps = 700; //for solver
const char* windowTitle = "Physics Engine";
float pixelsPerMeter = 30;
math::Vector2<float> windowSizeInMeters{ 30, 30 };

math::Vector2<float> universalGravity = { 0.0f, 0.0f }; //0.2f is generally a good amount
float gravity = 200;
float minGravityDistance = 2; //When objects are this close together or closer gravity will stop acting; this is to prevent infinity gravity when objects are at the same positions

bool collisions = true;
float restitutionCoefficient = 1;

float dragCoefficient = 0;

bool includePhaseSpace = false;
float phaseSpaceZoomSpeed = 0.001;


//This can be automatically computed
math::Vector2<float> windowSizeInPixels{ pixelsPerMeter * windowSizeInMeters.x, pixelsPerMeter * windowSizeInMeters.y };