#ifndef CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/random.h>

namespace rt
{

    class Point;
    class Vector;

    class DOFPerspectiveCamera : public Camera
    {
    public:
        DOFPerspectiveCamera(
            const Point &center,
            const Vector &forward,
            const Vector &up,
            float verticalOpeningAngle,
            float horizontalOpeningAngle,
            float focalDistance,
            float apertureRadius);

        Point center, lowerLeftCorner;
        Vector forward, sx, sy, up, w, h;
        float verticalOpeningAngle;
        float horizontalOpeningAngle;
        float focalDistance;
        float apertureRadius;

        virtual Ray getPrimaryRay(float x, float y) const;
    };

}

#endif