#ifndef CG1RAYTRACER_CAMERAS_FISHEYE_HEADER
#define CG1RAYTRACER_CAMERAS_FISHEYE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class FishEyeCamera : public Camera {
public:
    FishEyeCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float fov
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    Point e;
    Vector w;
    Vector u;
    Vector v;
    float fov;
    // float r;
    // float phi;
    // float theta;
};

}


#endif