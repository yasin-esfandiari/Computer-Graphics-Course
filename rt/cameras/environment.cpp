#include <rt/cameras/environment.h>
#include <rt/ray.h>
#include <math.h>

namespace rt {

EnvironmentCamera::EnvironmentCamera(const Point& center, const Vector& forward, const Vector& up, float fovx, float fovy)
{
    e = center;
    w = -forward.normalize();
    u = cross(up, w).normalize();
    v = cross(w, u);
    
    // fovx = fovx;
    // fovy = fovy;
}

Ray EnvironmentCamera::getPrimaryRay(float x, float y) const {
    Point origin = e;

    // Spherical parameterization
    float theta = y * this->fovy / 2;
    float phi = x * this->fovx / 2;

    // Convert to cartesian
    float x_p = cos(theta) * cos(phi);
    float y_p = cos(theta) * sin(phi);
    float z_p = sin(theta);
    Vector dir = -w + Vector(x_p, y_p, z_p);
    return Ray(origin, dir.normalize());
}

}
