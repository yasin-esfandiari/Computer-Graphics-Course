#include <rt/coordmappers/cylindrical.h>
#include <rt/intersection.h>
#include <core/matrix.h>
#include <core/scalar.h>

namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis):origin(origin), longitudinalAxis(longitudinalAxis), polarAxis(polarAxis){}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {
    float long_magnitude = this->longitudinalAxis.length();
    float polar_magnitude = this->polarAxis.length();
    // Vector e1 = this->polarAxis;
    Vector e2 = this->longitudinalAxis;
    // Vector e3 = cross(e1,e2);


    Vector loc_vector = hit.local() - this->origin;
    float v = dot(this->longitudinalAxis.normalize(), loc_vector) / long_magnitude;
    
    Vector polar_actual = this->polarAxis - dot(this->longitudinalAxis, this->polarAxis) * this->longitudinalAxis;
    Vector loc_polar = (loc_vector - dot(e2.normalize(), loc_vector) * e2.normalize()).normalize();

    float u;
    float arccos = acos(dot(polar_actual.normalize(), loc_polar));
    if (dot(cross(polar_actual, loc_polar), e2) > 0)
        arccos *= -1;
    // else
    //     u = (0.5 * pi) + ((pi/2.0) - arccos/2.0);

    u = arccos/(2.0 * pi); 
    return Point(u / polar_magnitude, v, 0);
}

}