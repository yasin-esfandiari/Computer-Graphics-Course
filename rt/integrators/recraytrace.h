#ifndef CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER

#include <rt/integrators/integrator.h>
#include <rt/coordmappers/world.h>

namespace rt {

class RecursiveRayTracingIntegrator : public Integrator {
    static WorldMapper defaultMapper;
public:
    RecursiveRayTracingIntegrator(World* world) : Integrator(world) {}
    virtual RGBColor getRadianceRec(const Ray& ray, int depth) const;
    virtual RGBColor getRadiance(const Ray& ray) const;
    int MAX_DEPTH = 6;
};

}

#endif