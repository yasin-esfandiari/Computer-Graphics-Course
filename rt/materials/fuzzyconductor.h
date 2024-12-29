#ifndef CG1RAYTRACER_MATERIALS_FUZZYCONDUCTOR_HEADER
#define CG1RAYTRACER_MATERIALS_FUZZYCONDUCTOR_HEADER

#include <rt/materials/material.h>
#include <core/point.h>
#include <core/random.h>
#include <core/scalar.h>

namespace rt
{

    /// A "rough" mirror or metal in general
    class FuzzyConductorMaterial : public Material
    {
    public:
        FuzzyConductorMaterial(float eta, float kappa, float fuzzyangle);
        virtual RGBColor getReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir, const Vector &inDir) const;
        virtual RGBColor getEmission(const Point &texPoint, const Vector &normal, const Vector &outDir) const;
        virtual SampleReflectance getSampleReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir) const;
        virtual Sampling useSampling() const;
        float eta, kappa, fuzzyangle;
    };

}

#endif