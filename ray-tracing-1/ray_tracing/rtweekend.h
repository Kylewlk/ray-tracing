//
// Created by wlk12 on 2023/8/13.
//
#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

double random_double();

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}


// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
