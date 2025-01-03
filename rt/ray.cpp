#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d)
{
    rt_assert(d != Vector::rep(0.0f));
    this->o = o;
    this->d = d;
}

Point Ray::getPoint(float distance) const {
    // Vector d_norm = d.normalize();   // 
    Vector d_norm = d;

    return Point(o.x + distance*d_norm.x, o.y + distance*d_norm.y, o.z + distance*d_norm.z);
}

}
