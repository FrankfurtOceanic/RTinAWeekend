#include <iostream>

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"


int main()
{
	// World

	hittable_list world;

	// Materials
	auto material_ground = make_shared<lambertian>(color(0.2, 0.8, 0.2));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.9);


	// Adding to the world
	world.add(make_shared<sphere>(point3(0, -1000.5, -1), 1000.0, material_ground));
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));

	// Camera

	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 100;
	
	// Render
	cam.render(world);

	return 0;
}
