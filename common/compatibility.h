#pragma once
#include <numbers>

#ifndef PI
#define PI std::numbers::pi_v<float>
#endif

#ifndef radians
#define radians(deg) ((deg) * PI / 180.0f)
#endif