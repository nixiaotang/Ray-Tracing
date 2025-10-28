#ifndef INTERVAL_H
#define INTERVAL_H

#include "utility.h"

class interval {
public:
    static const interval EMPTY, UNIVERSE;

    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Empty interval default
    interval(double min, double max) : min{min}, max{max} {}
    
    double size() const { return max - min; }
    bool contains(double x) { return min <= x && x <= max; }
    bool surrounds(double x) { return min < x && x < max; }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
};

const interval interval::EMPTY = interval(+infinity, -infinity);
const interval interval::UNIVERSE = interval(-infinity, +infinity);

#endif
