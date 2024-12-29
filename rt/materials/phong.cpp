#include <rt/materials/phong.h>
#include <core/scalar.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent):specular(specular), exponent(exponent){}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    // Vector v = inDir - 2 * dot(inDir, normal) * normal;
    // v.normalize();
    // if (dot(v, outDir) < 0 || dot(-inDir, normal) < 0) 
    //     return RGBColor::rep(0.0f);
    
    // float cos_theta = dot(outDir, v);
    // RGBColor color = this->specular->getColor(texPoint);
    // float power = pow(cos_theta, this->exponent);
    // float exponent = power * (this->exponent + 2) * dot(-inDir, normal) / (2 * pi);
    
    // return color * exponent;

    Vector r_e = - inDir + 2 * rt::dot(inDir, normal) * normal;
    
    return this->specular->getColor(texPoint) * std::min(powf(std::max(0.f, rt::dot(r_e, outDir)), this->exponent), 1.f) * rt::dot(inDir, normal) * (this->exponent + 2) / (2 * pi);

}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    // return SampleReflectance();
    UNREACHABLE;
}

Material::Sampling PhongMaterial::useSampling() const {
    return SAMPLING_NOT_NEEDED;
}

}