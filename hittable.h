#ifndef HITTABLE_H
#define HITTABLE_H

#include "utility.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    bool front_face; // whether or not hit point is front-side or back-side

    void set_normal(const ray& r, const vec3& outward_normal) {
        // sets hit record normal vector
        // (NOTE: the parameter `outward_normal` is assumed to have unit length.) <- not sure if this is important
        
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
