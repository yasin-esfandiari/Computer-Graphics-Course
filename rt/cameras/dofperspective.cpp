#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <cmath>
#include <core/random.h>
#include<core/vector.h>


namespace rt
{

    DOFPerspectiveCamera::DOFPerspectiveCamera(const Point &center, const Vector &forward, const Vector &up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
        : center(center), forward(forward), up(up), verticalOpeningAngle(verticalOpeningAngle), horizontalOpeningAngle(horizontalOpeningAngle), focalDistance(focalDistance), apertureRadius(apertureRadius)
        {
                sx = cross(up, -forward).normalize();
                sy = cross(-forward, sx).normalize();
        }

    Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const
    {
        Ray ray;
        float rd = apertureRadius * random(-0.5,0.5);
        Vector offset = sx * rd + sy * rd ;

        Vector rayVec = x * tan(horizontalOpeningAngle / 2.0) * sx + y * tan(verticalOpeningAngle / 2.0) * sy + forward.normalize();
        Point focalPt = Ray(center, rayVec.normalize()).getPoint(focalDistance);
        
        ray.d = (focalPt - center - offset).normalize();
        ray.o = center +offset;

        return(ray);
    }

}