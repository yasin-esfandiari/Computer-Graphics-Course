#include <rt/integrators/casting.h>

namespace rt
{

	RGBColor RayCastingIntegrator::getRadiance(const Ray &ray) const
	{
		Intersection scene_intersect = world->scene->intersect(ray, 0.0f, FLT_MAX);
		if (!scene_intersect)
			return RGBColor(0, 0, 0);

		Vector normal_vector = scene_intersect.normal();
		float dot_product = fmax(0, dot(-ray.d, normal_vector.normalize()));
		return RGBColor(dot_product, dot_product, dot_product);
	}

}
