#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "material.h"
#include "color.h"
#include "hittable.h"
#include "iostream"

class camera
{
public:
    double aspect_ratio = 1.0;
	int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10; //Maximum number of bounces per ray

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaing: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color = color(0.0, 0.0, 0.0);

                for (int k=0; k<samples_per_pixel; k++){
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog<< "\rDone.                 \n";
    }
private:
    int image_height;
    point3 position;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialize() {
        // Calculate image Height
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        position = point3(0,0,0);

        // Camera
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate horizontal and vertical vectors across the viewport
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);
        
        // Calculate horizontal and vertical pixel deltas
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate location of upper left pixel (0,0)
        auto viewport_upper_left = position
                                - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + pixel_delta_u + pixel_delta_v;
    }

    color ray_color(const ray& r, const int depth, const hittable& world) const {
        
        if(depth <=0) return color(0.0, 0.0, 0.0);

        hit_record rec;

        if(world.hit(r, interval(0.001, infinity), rec)){
            ray scattered;
            color attenuation;
            
            if(rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0.0, 0.0, 0.0);
        }

        vec3 unit_dir = unit_vector(r.direction());
        auto a = 0.5*(unit_dir.y() + 1);
        return lerp(color(1.0, 1.0, 1.0),  color(0.5, 0.7, 1.0), a);
    }

    ray get_ray(int i, int j){
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();
        
        auto ray_direction = pixel_sample - position; //rays originate from the camera center
        return ray(position, ray_direction);
    }

    vec3 pixel_sample_square() const{
        //Return random point within the size of a pixel
        auto px = -0.5 +random_double();
        auto py = -0.5 +random_double();
    
        return pixel_delta_u * px + pixel_delta_v * py;
    }
};

#endif