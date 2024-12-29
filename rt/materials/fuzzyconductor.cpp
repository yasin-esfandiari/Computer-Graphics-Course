#include <rt/materials/fuzzyconductor.h>

namespace rt
{

    FuzzyConductorMaterial::FuzzyConductorMaterial(float eta, float kappa, float fuzzyangle)
        : eta(eta), kappa(kappa), fuzzyangle(fuzzyangle) {}

    RGBColor FuzzyConductorMaterial::getReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir, const Vector &inDir) const
    {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        CG_UNUSED(inDir);
        return RGBColor::rep(0.0f);
    }

    RGBColor FuzzyConductorMaterial::getEmission(const Point &texPoint, const Vector &normal, const Vector &outDir) const
    {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        return RGBColor::rep(0.0f);
    }

    Material::SampleReflectance FuzzyConductorMaterial::getSampleReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir) const
    {
        float d = powf(this->eta, 2.0f) + powf(this->kappa, 2.0f);
        Vector Ref = 2 * dot(normal.normalize(), outDir) * normal.normalize() - outDir;
        Ref.normalize();

        float r = tanf(this->fuzzyangle) * sqrtf(random());
        float theta = 2 * pi * random();

        Point center = texPoint + Ref;

        float x = r * cosf(theta);
        float y = r * sinf(theta);

        Point rayPoint = Point(center.x + x, center.y + y, center.z);
        Vector rayDir = (rayPoint - texPoint).normalize();

        if (dot(rayDir, normal) >= 0)
            Ref = rayDir;

        float cosTheta = dot(normal.normalize(), Ref);

        float rParallel = (d * powf(cosTheta, 2) - 2 * this->eta * cosTheta + 1) /
                          (d * powf(cosTheta, 2) + 2 * this->eta * cosTheta + 1);
        float rPerp = (d - 2 * this->eta * cosTheta + powf(cosTheta, 2)) /
                      (d + 2 * this->eta * cosTheta + powf(cosTheta, 2));
        float fresnel = 0.5 * (rParallel + rPerp);

        return SampleReflectance(Ref, RGBColor::rep(fresnel));
    }

    Material::Sampling FuzzyConductorMaterial::useSampling() const
    {
        return SAMPLING_ALL;
    }

}
