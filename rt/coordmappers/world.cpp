#include <rt/coordmappers/world.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    Point p = hit.hitPoint();

    return Point(p.x * this->scale.x, p.y * this->scale.y, p.z * this->scale.z);
}

WorldMapper::WorldMapper():scale(Vector::rep(1.0f)){}

WorldMapper::WorldMapper(const Vector& scale):scale(scale){}

}