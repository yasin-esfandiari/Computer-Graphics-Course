#include <rt/materials/conductor.h>
#include <core/scalar.h>

namespace rt {

ConductorMaterial::ConductorMaterial(float eta, float kappa) : eta(eta), kappa(kappa) {}

RGBColor ConductorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float cos_theta = fabs(dot(normal.normalize(), inDir.normalize()));
    float num1 = (this->eta*this->eta + this->kappa*this->kappa) * cos_theta * cos_theta;
    float num2 = 2 * this->eta * cos_theta;

    float parallel = (num1 - num2 + 1) / (num1 + num2 + 1);
    
    num1 = this->eta*this->eta + this->kappa*this->kappa;
    float perpendicular = (num1 - num2 + cos_theta * cos_theta) / (num1 + num2 + cos_theta * cos_theta);

    float clr = 0.5 * (parallel + perpendicular);
    return RGBColor::rep(clr);
}

RGBColor ConductorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance ConductorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    Vector inDir = 2 * dot(normal.normalize(), outDir) * normal.normalize() - outDir;
    inDir.normalize();
    RGBColor clr = this->getReflectance(texPoint, normal, outDir, inDir);

    return SampleReflectance(inDir, clr);
}

Material::Sampling ConductorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}