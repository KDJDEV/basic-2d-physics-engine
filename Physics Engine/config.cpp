#include "math.h"

uint32_t frame_rate = 60;
uint32_t substeps = 700; //for solver
const char* windowTitle = "Physics Engine";
float pixelsPerMeter = 30;
math::Vector2<float> windowSizeInMeters{ 30, 30 };

math::Vector2<float> gravity = { 0.0f, 0.2f };
float restitutionCoefficient = 1;
float dragCoefficient = 0.1;

float phaseSpaceZoomSpeed = 0.001;


//This can be automatically computed
math::Vector2<float> windowSizeInPixels{ pixelsPerMeter * windowSizeInMeters.x, pixelsPerMeter * windowSizeInMeters.y };