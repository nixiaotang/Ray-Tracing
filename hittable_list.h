#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "utility.h"
#include "hittable.h"


class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {};
    hittable_list(shared_ptr<hittable> obj) { add(obj); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> obj) { objects.push_back(obj); }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

        bool hit_anything = false;
        hit_record temp_rec;
        double closest_hit = ray_t.max;

        for(const shared_ptr<hittable>& object : objects) {
            bool hasHit = object->hit(r, ray_t, temp_rec);

            if (hasHit && temp_rec.t < closest_hit) {
                rec = temp_rec;
                closest_hit = rec.t;
                hit_anything = true;
            }
        }

        return hit_anything;
    }

};

#endif
