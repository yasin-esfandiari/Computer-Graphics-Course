#include <rt/coordmappers/spherical.h>
#include <core/scalar.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef):origin(origin), zenith(zenith), azimuthRef(azimuthRef){}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector intersect = hit.local() - this->origin;
    Vector zenith_normalized = this->zenith.normalize();
    
    float v = acos(dot(intersect.normalize(), zenith_normalized))/pi;

    Vector actual_azim = (this->azimuthRef - dot(zenith_normalized, this->azimuthRef) * zenith);
    Vector hit_projection = (intersect - dot(zenith_normalized, intersect) * zenith_normalized).normalize();

    float u;
    float arccos = acos(dot(actual_azim.normalize(), hit_projection));
    if (dot(cross(actual_azim, hit_projection), zenith) > 0)
        arccos *= -1; 
    // else
    //     u = (0.5 * pi) + ((pi/2.0) - arccos/2.0);
    
    u = arccos/(2.0 * pi); 
    return Point((u / this->azimuthRef.length()) , v / this->zenith.length(), 0.0f);

    // Vector intersect = hit.local() - this->origin;
    // Vector zenith_normalized = this->zenith.normalize();
    
    // // float v = pi - acos(dot(intersect.normalize(), zenith_normalized));
    // float v = acos(dot(intersect.normalize(), zenith_normalized))/pi;

    // Vector actual_azim = (this->azimuthRef - dot(zenith_normalized, this->azimuthRef) * zenith);
    // // Vector hit_projection = (intersect - dot(zenith_normalized, intersect) * zenith_normalized).normalize();
    // Vector hit_projection = (intersect - dot(zenith_normalized, intersect) * zenith_normalized);

    // float u;
    // float cosine = dot(actual_azim, hit_projection);
    // float sinus = (cross(actual_azim, hit_projection)/(actual_azim.length() * hit_projection.length())).length();
    
    // float angle = std::atan2(sinus, cosine);
    // if (dot(cross(actual_azim, hit_projection), zenith) > 0)
    //     angle *= -1;

    // u = angle/(2*pi);
    
    // return Point((u / this->azimuthRef.length()) , v / this->zenith.length(), 0.0f);

}

}