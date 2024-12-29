#include <rt/lights/pointlight.h>

#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity):position(position), intensity(intensity){}

LightHit PointLight::getLightHit(const Point& p) const {
    LightHit hit_point;
    Vector dir = this->position - p; // towards light source
    hit_point.direction = dir.normalize();
    hit_point.distance = dir.length();
    hit_point.normal = -dir;   // the opposite dir
    
    return hit_point;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    // The effective irradiance dissipates proportionally to the square distance from the source to the hit point.
    return this->intensity * (1.0f / (irr.distance * irr.distance));
}

}
