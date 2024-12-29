#include <rt/cameras/fisheye.h>
#include <rt/ray.h>
#include <math.h>

namespace rt {

FishEyeCamera::FishEyeCamera(const Point& center, const Vector& forward, const Vector& up, float fov)
{
    e = center;
    w = -forward.normalize();
    u = cross(up, w).normalize();
    v = cross(w, u);
    // fov = fov;
}

Ray FishEyeCamera::getPrimaryRay(float x, float y) const {
    Point origin = e;

    // polar parameterization
    float r = sqrt(x*x + y*y);
    float phi = atan2(y, x);

    // wrap onto a sphere
    float theta = r * this->fov / 2;

    // Convert to cartesian
    float x_p = sin(theta) * cos(phi);
    float y_p = sin(theta) * sin(phi);
    float z_p = cos(theta);
    Vector dir = -w + Vector(x_p, y_p, z_p);
    return Ray(origin, dir.normalize());
}

}
