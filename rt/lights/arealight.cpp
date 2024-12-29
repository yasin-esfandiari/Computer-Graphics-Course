#include <rt/lights/arealight.h>
#include <core/color.h>

namespace rt
{

    AreaLight::AreaLight(Solid *source) : source(source) {}

    LightHit AreaLight::getLightHit(const Point &p) const
    {
        Solid::Sample obj = this->source->sample();
        Point hit_pint = obj.point;

        LightHit light_hit_point;
        Vector dir = hit_pint - p;

        light_hit_point.distance = dir.length() - 0.00014; // Can't find the sweet spot for offset. 1e-4 is too much but 1e-5 is too little
        light_hit_point.direction = dir.normalize();
        light_hit_point.normal = obj.normal;
        return light_hit_point;
    }

    RGBColor AreaLight::getIntensity(const LightHit &irr) const
    {
        RGBColor intensity = this->source->material->getEmission(Point::rep(1.0f), Vector::rep(1.0f), Vector::rep(1.0f));
        float cosTheta = std::max(dot(irr.normal.normalize(), -irr.direction), 0.0f);
        return intensity * cosTheta * this->source->getArea() / (irr.distance * irr.distance);
    }
}
