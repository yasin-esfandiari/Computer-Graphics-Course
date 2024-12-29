#include "interpolate.h"

namespace rt {

template <typename T>
T lerp(const T& px0, const T& px1, float xPoint) {
    return (1 - xPoint) * px0 + xPoint * px1;
}

template <typename T>
T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
    float cWeight = 1 - aWeight - bWeight;
    return a * aWeight + b * bWeight + c * cWeight;
}

template <typename T>
T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {
    T px1 = (1 - xWeight) * px0y0 + xWeight * px1y0;
    T px2 = (1 - xWeight) * px0y1 + xWeight * px1y1;
    T y = (1 - yWeight) * px1 + yWeight * px2;

    return y;
}

template <typename T>
T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
    const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
    float xPoint, float yPoint, float zPoint)
{
    T px0y0 = (1 - xPoint) * px0y0z0 + xPoint * px1y0z0;
    T px0y1 = (1 - xPoint) * px0y0z1 + xPoint * px1y0z1;
    T px1y0 = (1 - xPoint) * px0y1z0 + xPoint * px1y1z0;
    T px1y1 = (1 - xPoint) * px0y1z1 + xPoint * px1y1z1;

    T px0   = (1 - yPoint) * px0y0 + yPoint * px1y0;
    T px1   = (1 - yPoint) * px0y1 + yPoint * px1y1;

    return (1 - zPoint) * px0 + zPoint * px1;
}

}