#ifndef CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class EnvironmentCamera : public Camera {
public:
    EnvironmentCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float fovx,
        float fovy
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    Point e;
    Vector w;
    Vector u;
    Vector v;
    float fovx;
    float fovy;
};

}


#endif