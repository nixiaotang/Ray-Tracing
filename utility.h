#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.14159265358979323846;

inline double deg2rad(double deg) { return deg * PI / 180.0; }
inline double rad2deg(double rad) { return rad / PI * 180.0; }

inline double random_double() {
    static std::uniform_real_distribution<double> unif(0.0, 1.0);
    static std::mt19937 generator;
    return unif(generator);
}

inline double random_double(double lower, double upper) {
    return lower + (upper - lower) * random_double();
}

#include "colour.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"

#endif

