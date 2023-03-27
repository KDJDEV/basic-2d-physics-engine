#include "math.h"

uint32_t frame_rate = 60;
const char* windowTitle = "Physics Engine";
float pixelsPerMeter = 30;

math::Vector2<float> windowSizeInMeters{ 30, 30 };


//This can be automatically computed
math::Vector2<float> windowSizeInPixels{ pixelsPerMeter * windowSizeInMeters.x, pixelsPerMeter * windowSizeInMeters.y };