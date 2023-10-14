#include <iostream>

#include "color.h"
#include "vec3.h"
#include "ray.h"

color ray_color(const ray& r);
vec3 lerp(const vec3 a, const vec3 b, const float t);
double hit_sphere(const point3& center, double radius, const ray& r);

double hit_sphere(const point3& center, double radius, const ray& r){
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b*b - 4*a*c;

	if(discriminant >= 0){
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
	else{
		return -1.0;
	}
}

color ray_color(const ray& r){
	auto t = hit_sphere(point3(0,0,-1), 0.5, r);
	if(t > 0.0){
		vec3 Normal = unit_vector(r.at(t) - point3(0,0,-1));
		return 0.5 * (Normal + vec3(1,1,1));
	}

	vec3 unit_dir = unit_vector(r.direction());
	auto a = 0.5*(unit_dir.y() + 1);
	return lerp(color(1.0, 1.0, 1.0),  color(0.5, 0.7, 1.0), a);
}

vec3 lerp(const vec3 a, const vec3 b, const float t){
	return (1-t) * a + t * b;
}

int main()
{
	// Image 
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	// Calculate image Height
	int image_height = static_cast<int>(image_width/aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera
	auto focal_length = 1.0f;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
	auto camera_position = point3(0,0,0);

	// Calculate horizontal and vertical vectors across the viewport
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);
	
	// Calculate horizontal and vertical pixel deltas
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// Calculate location of upper left pixel (0,0)
	auto viewport_upper_left = camera_position
                            - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
	auto pixel00_loc = viewport_upper_left + pixel_delta_u + pixel_delta_v;

	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaing: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_position;

			ray r(camera_position, ray_direction);
			color pixel_color = ray_color(r);

			write_color(std::cout, pixel_color);
		}
	}

	std::clog<< "\rDone.                 \n";

	return 0;
}
