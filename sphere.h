#ifndef SPHERE_H
#define SPHERE_H

#include "utility.h"
#include "hittable.h"

class sphere : public hittable {
    point3 center;
    double radius;
    shared_ptr<material> mat;

public :

    sphere(const point3& center, double radius, shared_ptr<material> mat) : center{center}, radius{std::fmax(0.0, radius)}, mat{mat} {};
    
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

        vec3 temp = center - r.origin();
        double a = pow(r.direction().length(), 2);
        double h = dot(r.direction(), temp);
        double c = pow(temp.length(), 2) - pow(radius, 2);
        double discriminant = pow(h, 2) - a*c;

        if (discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);

        double root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.p = r.at(root);
        rec.t = root;
        rec.set_normal(r, (rec.p - center)/radius);
        rec.mat = mat;
        
        return true;
    }

};




#endif

