#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
    int img_height;             // rendered image height (in pixels)
    point3 camera_center;       // camera center in world space

    vec3 pix_delta_u;
    vec3 pix_delta_v;
    point3 pixel00_loc;         // location of pixel (0, 0) center in world space

    vec3 u, v, w;               // camera frame basis vectors (u = right, v = up, z = opposite of look direction)

    double defocus_radius;        // Defocus disk radius

    void init() {
        camera_center = lookfrom;
        img_height = std::max(1, int(img_width / aspect_ratio));
        
        // Determine Viewport Dimensions
        // double focal_length = (lookfrom - lookat).length();
        double theta = deg2rad(vfov);
        double h = tan(theta / 2);
        double viewport_height = 2 * h * focus_dist;
        double viewport_width = viewport_height * (double)(img_width) / img_height;

        // Calculate u, v, w basis vectors for camera coordinate
        w = -normalize(lookat - lookfrom);          // opposite of camera view direction
        u = normalize(cross(vup, w));               // camera right angle
        v = cross(w, u);                            // camera up angle

        // Calculate Viewport Values
        vec3 viewport_u = viewport_width * u;       // vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;     // vector down viewport vertical edge

        pix_delta_u = viewport_u / img_width;
        pix_delta_v = viewport_v / img_height;

        point3 viewport_top_left = camera_center - viewport_u/2 - viewport_v/2 - (focus_dist * w); // center viewport in front of camera
        pixel00_loc = viewport_top_left + 0.5 * (pix_delta_u + pix_delta_v); // half a pixel offset in viewport

        // Calculate camera defocus disk basis vectors
        defocus_radius = focus_dist * tan(deg2rad(defocus_angle / 2));
    }

    colour ray_colour(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0) return colour(0);
        
        hit_record rec;
        if (world.hit(r, interval(0.0001, infinity), rec)) {
            colour attenuation;
            ray scattered_ray;
            if (rec.mat->scatter(r, rec, attenuation, scattered_ray)) {
                
                return attenuation * ray_colour(scattered_ray, depth-1, world);

            } else return colour(0);
        }

        vec3 unit_dir = normalize(r.direction());
        return lerp(
            colour(1.0, 1.0, 1.0),
            colour(0.5, 0.7, 1.0),
            0.5 * (unit_dir.y() + 1.0)
        );
    }

    ray get_ray_sample(int i, int j) const {
        // Construct camera ray originating from camera defocus disk
        // and directed at randomly sampled point around pixel (j, i)

        vec3 sample_offset = sample_square();
        point3 pixel_sample = 
            pixel00_loc +
            (i + sample_offset.y()) * pix_delta_v +
            (j + sample_offset.x()) * pix_delta_u;

        vec3 ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        vec3 ray_dir = pixel_sample - ray_origin;
        return ray(ray_origin, ray_dir);
    }

    vec3 sample_square() const {
        // returns vec3 to a random point in the ([-0.5, 0.5], [-0.5, 0.5], 0) unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    vec3 defocus_disk_sample() const {
        // returns random point in camera defocus disk
        vec3 p = random_in_unit_disk();
        return camera_center + (p[0] * u * defocus_radius) + (p[1] * v * defocus_radius);
    }

public:

    double aspect_ratio = 16.0 / 9.0;       // ratio of image w / h
    int img_width = 400;                    // in pixels
    int samples_per_pixel = 10;             // # of random samples for each pixel
    int max_depth = 10;                     // max # of light bounces in scene

    double vfov = 90;                       // vertical fov (in degrees)
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0;               // variation angle of through each pixel
    double focus_dist = 10;                 // distance from camera `lookfrom` point to plane of perfect focus

    void render(const hittable& world) {
        init();

        std::cout << "P3\n" << img_width << " " << img_height << "\n255\n";

        for(int i = 0; i < img_height; i++) {
            std::clog << "\rScanlines remaining: " << (img_height - i) << " " << std::flush;

            for(int j = 0; j < img_width; j++) {
                colour pix_col = colour();

                for(int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray_sample(i, j);
                    pix_col += ray_colour(r, max_depth, world);
                }
                pix_col /= samples_per_pixel;

                write_colour(pix_col);
            }
        }

        std::clog << "\rDone.                               " << std::endl;
    }
};


#endif
