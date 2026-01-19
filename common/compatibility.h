#pragma once
#include <numbers>

#ifndef PI
#define PI std::numbers::pi
#endif

#ifndef radians
#define radians(deg) ((deg) * PI / 180.0)
#endif