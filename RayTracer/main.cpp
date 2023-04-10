#define SDL_MAIN_HANDLED
//#include "SDL.h"
#include <iostream>
#include "vec3.h"
#include "ray.h"
#include <math.h>



double hitSphere(const vec3& center,double radius,const ray r) {
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius*radius;

	auto discriminant = b*b - 4*a*c;

	if (discriminant < 0)
	{
		return -1.0;
	}
	else {
		return (( - b - sqrt(discriminant))/2.0*a);
	}

}


vec3 ray_color(const ray& r) {
	 auto t = hitSphere(vec3(0, 0, -1), 0.5, r);


	if (t > 0.0)
	{
		vec3 hit(r.origin() + r.direction() * t);
		vec3 N = normalize(hit);
		vec3 lightdir = normalize(vec3(-1,-1,-1));
		float d =dot(N, -lightdir);
		vec3 spherecolor(1, 0, 1);
		spherecolor *= d * 0.5 + 0.5;
		return spherecolor ;
	}

	vec3 unit_direction = unit_vector(r.direction());
	 t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, vec3 pixel_color) {
	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(255.999 * pixel_color.x()) << ' '
		<< static_cast<int>(255.999 * pixel_color.y()) << ' '
		<< static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

int main() {
	//Image 
	const auto aspect_ratio = 4.0 / 4.0;
	int width = 400;
	int height = int(width/aspect_ratio);

	//viewport
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * aspect_ratio;
	auto focal_length = 1.0;

	auto origin = vec3(0, 0, 0);
	auto horizontal = vec3(viewport_width,0,0);
	auto vertical = vec3(0, viewport_width, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
	std::cout << "P3\n" << width << ' ' << height << "\n255\n";
	for (int y = height-1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			auto u = double(x) / (width - 1);
			auto v = double(y) / (height - 1);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);

			vec3 pixelcolor = ray_color(r);

			write_color(std::cout,pixelcolor);
		}
	}




	return 0;
}