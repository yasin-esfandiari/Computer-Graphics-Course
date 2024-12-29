#include <rt/coordmappers/tmapper.h>
#include <core/interpolate.h>

namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3]):v1(ntv[0]), v2(ntv[1]), v3(ntv[2]){}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2):v1(tv0), v2(tv1), v3(tv2){}

Point TriangleMapper::getCoords(const Intersection& hit) const {
    Point intersect = hit.local();
    
    // float x = intersect.x * this->v1.x + intersect.y * this->v2.x + intersect.z * this->v3.x;
    // float y = intersect.x * this->v1.y + intersect.y * this->v2.y + intersect.z * this->v3.y;
    // float z = intersect.x * this->v1.z + intersect.y * this->v2.z + intersect.z * this->v3.z;

    // return Point (x, y, z);

    return lerpbar(v1, v2, v3, intersect.x, intersect.y);
}

}