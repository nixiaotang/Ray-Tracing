#ifndef VEC3_H
#define VEC3_H

#include <stdexcept>

#include "utility.h"

class vec3 {
public:
    double v[3];

    vec3() : v{0, 0, 0} {}
    vec3(double v0) : v{v0, v0, v0} {}
    vec3(double v0, double v1, double v2) : v{v0, v1, v2} {}    

    double x() const { return v[0]; }
    double y() const { return v[1]; }
    double z() const { return v[2]; }

    vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
    
    double operator[](int i) const {
        if (i < 0 || i > 2) throw std::out_of_range("");
        return v[i];
    }

    double& operator[](int i) {
        if (i < 0 || i > 2) throw std::out_of_range("");
        return v[i];
    }

    vec3& operator+=(const vec3& other) {
        v[0] += other.v[0];
        v[1] += other.v[1];
        v[2] += other.v[2];
        return *this;
    }

    vec3& operator-=(const vec3& other) {
        v[0] -= other.v[0];
        v[1] -= other.v[1];
        v[2] -= other.v[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        v[0] *= t;
        v[1] *= t;
        v[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        v[0] /= t;
        v[1] /= t;
        v[2] /= t;
        return *this;
    }

    double length() const {
        return std::sqrt(sqlength());
    }

    double sqlength() const {
        return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    }

    bool near_zero() {
        double s = 1e-8;
        return (std::fabs(v[0]) < s) && (std::fabs(v[1]) < s) && (std::fabs(v[2]) < s);
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

};

using point3 = vec3;
using colour = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.v[0] << ", " << v.v[1] << ", " << v.v[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.v[0] + v.v[0], u.v[1] + v.v[1], u.v[2] + v.v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.v[0] - v.v[0], u.v[1] - v.v[1], u.v[2] - v.v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.v[0] * v.v[0], u.v[1] * v.v[1], u.v[2] * v.v[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.v[0], t * v.v[1], t * v.v[2]);
}
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.v[0] * v.v[0]
         + u.v[1] * v.v[1]
         + u.v[2] * v.v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.v[1] * v.v[2] - u.v[2] * v.v[1],
                u.v[2] * v.v[0] - u.v[0] * v.v[2],
                u.v[0] * v.v[1] - u.v[1] * v.v[0]);
}

inline vec3 normalize(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while (true) {
        vec3 v = vec3::random(-1, 1);
        double sqlen = v.sqlength();
        if (sqlen > 1e-160 && sqlen <= 1) {
            return normalize(v);
        }
    }
}

inline vec3 random_in_unit_disk() {
    while (true) {
        vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.sqlength() < 1) return p;
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 v = random_unit_vector();
    return dot(v, normal) > 0.0 ? v : -v;
}

inline vec3 reflect(const vec3& v, const vec3& n){
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    double cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_par = -sqrt(1.0 - r_out_perp.sqlength()) * n; // fabs(1.0 - r_out_perp.length_squared())

    return r_out_perp + r_out_par;
}

inline vec3 lerp(const vec3& a, const vec3& b, float t) {
    return (1.0-t) * a + t * b;
}

#endif
