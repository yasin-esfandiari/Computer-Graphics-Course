#include <rt/materials/lambertian.h>
#include <core/scalar.h>

namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse):emission(emission), diffuse(diffuse){}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float reflectance = fabs(dot(inDir.normalize(), normal.normalize()));
    RGBColor diff_mat = this->diffuse->getColor(texPoint);

    return (diff_mat.clamp() * reflectance / pi).clamp();
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    if (!emission)
        return RGBColor::rep(0.0f);
    
    return this->emission->getColor(texPoint) * dot(normal, outDir);
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return SampleReflectance();
}

Material::Sampling LambertianMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}