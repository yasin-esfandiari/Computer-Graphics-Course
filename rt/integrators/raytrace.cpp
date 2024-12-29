#include <rt/integrators/raytrace.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/lights/light.h>
#include <rt/coordmappers/coordmapper.h>
#include <core/scalar.h>

namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {
    // Intersection scene_intersect = world->scene->intersect(ray, FLT_MAX);
    Intersection scene_intersect = world->scene->intersect(ray, 0.0f, FLT_MAX);
    RGBColor color = RGBColor::rep(0.0f);
    Point p;

    if(!scene_intersect)
        return color;

    if (scene_intersect.solid->texMapper == nullptr)
        p = scene_intersect.hitPoint();
    else
        p = scene_intersect.solid->texMapper->getCoords(scene_intersect);

    color = color + scene_intersect.solid->material->getEmission(p, scene_intersect.normal(), -ray.d);

    for (auto& light_source : this->world->light) {
        LightHit light_hit = light_source->getLightHit(scene_intersect.hitPoint());
        // Ray shadow_ray = Ray(Point(p + light_hit.direction * 1e-4), light_hit.direction);
        Ray shadow_ray = Ray(Point(scene_intersect.hitPoint() + light_hit.direction * 1e-4), light_hit.direction);
        // Ray shadow_ray = Ray(Point(scene_intersect.hitPoint() + light_hit.direction * epsilon), light_hit.direction); --> some artifacts


        // Skip if the shadow ray and the primary ray leave the surface on the opposite side.
        float sh_r = dot(shadow_ray.d, scene_intersect.normal());
        float r = dot(-ray.d, scene_intersect.normal());
        if (sh_r * r < 0.0)  
            continue;

        // Trace  a  shadow  ray  into  the  world  to  detect  any  obstacles  between  the  light  source  and  the  hitpoint.
        // Intersection shadow_intersection = this->world->scene->intersect(shadow_ray, light_hit.distance);
        Intersection shadow_intersection = this->world->scene->intersect(shadow_ray, 0.0f, light_hit.distance);
        // Intersection shadow_intersection = this->world->scene->intersect(shadow_ray, epsilon, light_hit.distance);

        if(shadow_intersection) 
            continue;

        RGBColor intensity = light_source->getIntensity(light_hit);
        RGBColor reflectance = scene_intersect.solid->material->getReflectance(p, scene_intersect.normal(), -ray.d.normalize(), light_hit.direction);
        color = color + reflectance * intensity;
    }
    return color;
}

}