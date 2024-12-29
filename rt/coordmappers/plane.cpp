#include <rt/coordmappers/plane.h>
#include <rt/intersection.h>
#include <core/matrix.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2):e1(e1), e2(e2) {}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    Vector plane_normal = cross(this->e1, this->e2);
    Matrix transform = Matrix::system(this->e1, this->e2, plane_normal).invert();

    return transform * hit.local();
}

}