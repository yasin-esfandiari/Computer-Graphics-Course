#include <rt/solids/infiniteplane.h>

namespace rt
{

    InfinitePlane::InfinitePlane(const Point &origin, const Vector &normal, CoordMapper *texMapper, Material *material)
        : Solid(texMapper, material), origin(origin), normal(normal)
    {
    }

    BBox InfinitePlane::getBounds() const
    {
        return BBox::full();
    }

    Intersection InfinitePlane::intersect(const Ray &ray, float tmin, float tmax) const
    {
        float denominator = dot(this->normal, ray.d);
        Vector tip_on_the_plane = this->origin - ray.o;
        float numerator = dot(tip_on_the_plane, this->normal);
        float t = numerator / denominator;

        if (fabs(denominator) <= epsilon || t > tmax || t <= tmin) // fov is parallel to plane
            return Intersection::failure();

        Vector local_vector = ray.getPoint(t) - this->origin;
        Point local_point = Point(local_vector.x, local_vector.y, local_vector.z);
        return Intersection(t, ray, this, this->normal, local_point);
    }

    Solid::Sample InfinitePlane::sample() const
    {
        /* TODO */ NOT_IMPLEMENTED;
    }

    float InfinitePlane::getArea() const
    {
        return FLT_MAX;
    }

}
