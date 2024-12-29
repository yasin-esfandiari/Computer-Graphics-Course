#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color): direction(direction.normalize()), color(color){}

LightHit DirectionalLight::getLightHit(const Point& p) const {
    LightHit hit_point;
    hit_point.normal = this->direction;
    hit_point.direction = - hit_point.normal;
    hit_point.distance = FLT_MAX;
    
    return hit_point;
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return this->color;
}

}
