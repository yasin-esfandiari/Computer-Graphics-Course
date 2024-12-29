#include <rt/integrators/castingdist.h>

namespace rt
{

    RayCastingDistIntegrator::RayCastingDistIntegrator(World *world, const RGBColor &nearColor, float nearDist, const RGBColor &farColor, float farDist)
        : Integrator(world)
    {
        this->nearColor = nearColor;
        this->farColor = farColor;
        this->nearDist = nearDist;
        this->farDist = farDist;
    }

    RGBColor RayCastingDistIntegrator::getRadiance(const Ray &ray) const
    {
        Intersection scene_intersect = world->scene->intersect(ray, 0.0f, FLT_MAX);
        if (!scene_intersect)
            return RGBColor(0, 0, 0);

        Vector normal_vector = scene_intersect.normal();
        float intensity = (scene_intersect.distance - nearDist) / (farDist - nearDist);
        intensity = std::min(std::max(intensity, 0.0f), 1.0f);
        RGBColor color = intensity * (farColor - nearColor) + nearColor;
        float dot_product = dot(-ray.d, normal_vector.normalize());
        return color * dot_product;
    }

}
