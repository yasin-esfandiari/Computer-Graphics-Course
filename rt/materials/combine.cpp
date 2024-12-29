#include <rt/materials/combine.h>

namespace rt {

CombineMaterial::CombineMaterial(){}

void CombineMaterial::add(Material* m, float w) {
    this->material_list.push_back(m);
    this->weights.push_back(w);
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    RGBColor ref = RGBColor::rep(0.0f);
    RGBColor temp;
    for(long unsigned int i = 0; i < this->material_list.size(); i++) 
    {
        temp = this->material_list[i]->getReflectance(texPoint, normal, outDir, inDir) * this->weights[i];
        if (this->material_list[i]->useSampling() == SAMPLING_ALL)
            continue;

        ref = ref + temp;
    }

    return ref;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor emission = RGBColor::rep(0.0f);
    for(long unsigned int i = 0; i < this->material_list.size(); i++) 
        emission = emission + (this->weights[i] * this->material_list[i]->getEmission(texPoint, normal, outDir));

    return emission;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    SampleReflectance sample_reflectance = SampleReflectance();
    for(long unsigned int i = 0; i < this->material_list.size(); i++) 
    {
        if(this->material_list[i]->useSampling() == SAMPLING_ALL || this->material_list[i]->useSampling() == SAMPLING_SECONDARY) 
        {
            sample_reflectance = this->material_list[i]->getSampleReflectance(texPoint, normal, outDir);
            sample_reflectance.reflectance = sample_reflectance.reflectance * this->weights[i];
            break;
        }
    }

    return sample_reflectance;
}

Material::Sampling CombineMaterial::useSampling() const {
   for (Material* m : this->material_list) 
    {
        if(m->useSampling() == SAMPLING_ALL || m->useSampling() == SAMPLING_SECONDARY)
            return SAMPLING_SECONDARY;
    }

    return SAMPLING_NOT_NEEDED;
}

}