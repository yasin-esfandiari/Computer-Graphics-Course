#include <rt/integrators/recraytrace.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>

namespace rt {
    WorldMapper RecursiveRayTracingIntegrator::defaultMapper = WorldMapper();

    RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
        return getRadianceRec(ray, 1);
    }

    RGBColor RecursiveRayTracingIntegrator::getRadianceRec(const Ray& ray, int depth) const {
        if (depth >= MAX_DEPTH)
            return RGBColor::rep(0.0f);

        Intersection intersection = world->scene->intersect(ray);
        if (intersection)
        {
            CoordMapper* mapper = intersection.solid->texMapper;
            if (!mapper)
                mapper = &defaultMapper;
            RGBColor irradiance = RGBColor::rep(0.0f);
            Point uv = mapper->getCoords(intersection);
            Point hitPoint = intersection.hitPoint();
            Point offsetHitPoint = hitPoint + intersection.normal() * epsilon * 10;

            if (intersection.solid->material->useSampling() != Material::SAMPLING_ALL) {

                for (long unsigned int i = 0; i < world->light.size(); i++) {
                    LightHit shadow_ray = world->light[i]->getLightHit(offsetHitPoint);
                    if (rt::dot(shadow_ray.direction, intersection.normal()) > epsilon && rt::dot(shadow_ray.direction, shadow_ray.normal) < 0)
                    {
                        Primitive* scene = world->light[i]->scene ? world->light[i]->scene : world->scene;
                        Intersection obstruction = scene->intersect(Ray(offsetHitPoint, shadow_ray.direction));
                        if (!obstruction || obstruction.distance >= shadow_ray.distance - epsilon * 10)
                        {
                            irradiance = irradiance +
                                world->light[i]->getIntensity(shadow_ray) *
                                intersection.solid->material->getReflectance(uv, intersection.normal(), -ray.d, shadow_ray.direction);
                        }
                    }
                }
            }

            if (intersection.solid->material->useSampling() != Material::SAMPLING_NOT_NEEDED) {
                Material::SampleReflectance sr = intersection.solid->material->getSampleReflectance(uv, intersection.normal(), -ray.d);
                if (rt::dot(sr.direction, intersection.normal()) < 0)
                    offsetHitPoint = hitPoint - intersection.normal() * epsilon * 10;
                irradiance = irradiance + sr.reflectance * this->getRadianceRec(Ray(offsetHitPoint, sr.direction), depth + 1);
            }

            irradiance = irradiance + intersection.solid->material->getEmission(uv, intersection.normal(), -ray.d);
            return irradiance;
        }
        else
            return RGBColor::rep(0.f);
    }
}