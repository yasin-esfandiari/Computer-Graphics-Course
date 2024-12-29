#include <rt/materials/dielectric.h>
#include <math.h>

namespace rt
{

    DielectricMaterial::DielectricMaterial(float eta)
        : eta(eta) {}

    RGBColor DielectricMaterial::getReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir, const Vector &inDir) const
    {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        CG_UNUSED(inDir);
        return RGBColor::rep(0.0);
    }

    RGBColor DielectricMaterial::getEmission(const Point &texPoint, const Vector &normal, const Vector &outDir) const
    {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        return RGBColor::rep(0.0f);
    }

    Material::SampleReflectance DielectricMaterial::getSampleReflectance(const Point &texPoint, const Vector &normal, const Vector &outDir) const
    {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);

        Vector inDir;

        float eta_init, eta_final;
        Vector effective_normal;

        float cos_incidence = dot(outDir, normal);

        if (cos_incidence > 0)
        {
            eta_init = 1.0;
            eta_final = this->eta;
            effective_normal = normal;
        }
        else
        {
            eta_init = this->eta;
            eta_final = 1.0;
            effective_normal = -normal;
            cos_incidence = dot(outDir, effective_normal);
        }

        float sin_incidence = sqrt(1.0f - pow(cos_incidence, 2));
        float eta_ratio = eta_final / eta_init;
        float sin_refraction = sin_incidence * eta_init / eta_final;
        float cos_refraction = sqrt(1.0f - pow(sin_refraction, 2));

        float fresnel_paral = (eta_final * cos_incidence - eta_init * cos_refraction) / (eta_final * cos_incidence + eta_init * cos_refraction);
        float fresnel_per = (eta_init * cos_incidence - eta_final * cos_refraction) / (eta_init * cos_incidence + eta_final * cos_refraction);
        float fresnel = 0.5 * (pow(fresnel_paral, 2.0) + pow(fresnel_per, 2.0));

        float choice = random(0.0, 1.0);
        if (sin_incidence > eta_ratio || choice <= fresnel)
        {
            inDir = -outDir + 2 * cos_incidence * effective_normal.normalize();
            if (sin_incidence > eta_ratio)
            {
                return SampleReflectance(inDir.normalize(), RGBColor::rep(1.0));
            }
            else
            {
                return SampleReflectance(inDir.normalize(), RGBColor::rep(fresnel));
            }
        }
        else
        {
            Vector incidence = -outDir.normalize();
            Vector refracted = effective_normal.normalize();
            float init = eta_init / eta_final;
            float transmit = (eta_init / eta_final) * cos_incidence - sqrt(1.0f - pow(sin_refraction, 2));
            inDir = init * incidence + transmit * refracted;
            return SampleReflectance(inDir.normalize(), RGBColor::rep((1 - fresnel) * pow(init, 2.0)));
        }
        return SampleReflectance(inDir.normalize(), RGBColor::rep(0.0));
    }

    Material::Sampling DielectricMaterial::useSampling() const
    {
        return SAMPLING_ALL;
    }

}