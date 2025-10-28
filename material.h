#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        [[maybe_unused]] const ray& ray_in,
        [[maybe_unused]] const hit_record& rec,
        [[maybe_unused]] colour& attenuation,
        [[maybe_unused]] ray& scattered) const {
        return false;
    }
};


class lambertian : public material {
    colour albedo;

public:
    lambertian(const colour& albedo) : albedo{albedo} {};

    bool scatter([[maybe_unused]] const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
        vec3 scatter_dir = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_dir.near_zero()) scatter_dir = rec.normal;

        scattered = ray(rec.p, scatter_dir);
        attenuation = albedo;
        return true;
    }

};

class metal : public material {
    colour albedo;
    double fuzz;

public:
    metal(const colour& albedo, double fuzz = 0) : albedo{albedo}, fuzz{std::fmin(fuzz, 1.0)} {};

    bool scatter(const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(ray_in.direction(), rec.normal);
        reflected = normalize(reflected) + (fuzz * random_unit_vector()); // normalize reflected ray + add fuzz factor
        scattered = ray(rec.p, reflected);
        attenuation = albedo;

        // if fuzz factor makes reflected ray go back under the surface, just dont reflect ray
        return dot(scattered.direction(), rec.normal) > 0;
    }
};

class dielectric : public material {
    double refraction_index; // ratio of material's refractive index over refractive index of enclosing media (ex. vacuum, air)

    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation
        double r0 = pow((1.0 - refraction_index) / (1.0 + refraction_index), 2);
        return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
    }

public:
    dielectric(double refraction_index) : refraction_index{refraction_index} {}

    bool scatter(const ray& ray_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
        attenuation = colour(1.0);
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 ray_in_norm = normalize(ray_in.direction());
        double cos_theta = fmin(dot(-ray_in_norm, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - pow(cos_theta, 2));

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
            direction = reflect(ray_in_norm, rec.normal);
        } else {
            direction = refract(ray_in_norm, rec.normal, ri);
        }

        scattered = ray(rec.p, direction);
        return true;
    }
};

#endif

