#include <rt/lights/spotlight.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity):
direction(direction), angle(angle), exp(power){
    this->position = position;
    this->intensity = intensity;
}

LightHit SpotLight::getLightHit(const Point& p) const {
    LightHit hit_point;
    Vector dir = this->position - p; // towards light source
    hit_point.direction = dir.normalize();
    hit_point.distance = dir.length();
    hit_point.normal = -dir;   // the opposite dir
    
    return hit_point;
}


RGBColor SpotLight::getIntensity(const LightHit& irr) const {
    float cosine_value = dot(irr.direction.normalize(), -this->direction.normalize());
    if (cosine_value < cos(this->angle)) 
        return RGBColor::rep(0.0f);
        
    return this->intensity * (pow(cosine_value, this->exp) / (irr.distance * irr.distance));   
}

}
