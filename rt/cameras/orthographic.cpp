#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{
    e = center;
    w = -forward.normalize();
    u = cross(up, w).normalize();
    v = cross(w, u);
    // to make a scaleX * scaleY rectangle
    u = u * scaleX / 2.f;
    v = v * scaleY / 2.f;
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
    Vector dir = -w;
    Point origin = Point(e + x * u + y * v);
    return Ray(origin, dir.normalize());
}

}
