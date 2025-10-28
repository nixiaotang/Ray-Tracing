#ifndef COLOUR_H
#define COLOUR_H

#include "utility.h"
#include "vec3.h"
#include "interval.h"

using colour = vec3;

inline double linear_to_gamma(const double linear_component) {
    if (linear_component > 0) return sqrt(linear_component);
    return 0;
}

void write_colour(const colour& c, std::ostream& out = std::cout) {
    int r = int(255.999 * linear_to_gamma(c.x()));
    int g = int(255.999 * linear_to_gamma(c.y()));
    int b = int(255.999 * linear_to_gamma(c.z()));
    
    out << r << ' ' << g << ' ' << b << "\n";
}

#endif
