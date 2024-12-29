#include <rt/cameras/perspective.h>
#include <cmath>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle)
{
    e = center;
    w = -forward.normalize();
    u = cross(up, w).normalize();
    v = cross(w, u);
    u = u * tanf(horizontalOpeningAngle / 2.f);
    v = v * tanf(verticalOpeningAngle / 2.f);
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    Point origin = e;
    Vector dir = -w + x * u + y * v;
    return Ray(origin, dir.normalize());
}

}
