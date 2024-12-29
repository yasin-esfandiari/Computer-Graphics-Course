#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    this->distance = distance;
    this->ray = ray;
    this->solid = solid;
    this->normal_vector = normal;
    this->local_point = local;
}

Intersection::operator bool() const {
    if (FLT_MAX != distance)
        return true;
    return false;
}

Intersection Intersection::failure() {
    // FLT_MAX means no intersect. you can also compare with this later
	return Intersection(FLT_MAX, Ray(), nullptr, Vector(), Point());
}

Point Intersection::hitPoint() const {
    return this->ray.getPoint(distance);
}

Vector Intersection::normal() const {
    return this->normal_vector;
}

Point Intersection::local() const {
    return this->local_point;
}

}
