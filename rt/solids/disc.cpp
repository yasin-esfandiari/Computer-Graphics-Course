#include <rt/solids/disc.h>

namespace rt
{

    Disc::Disc(const Point &center, const Vector &normal, float radius, CoordMapper *texMapper, Material *material)
        : Solid(texMapper, material), center(center), normal(normal), radius(radius)
    {
    }

    BBox Disc::getBounds() const
    {
        float centroid_x = sqrtf(1.0f - this->normal.x * this->normal.x) * this->radius;
        float centroid_y = sqrtf(1.0f - this->normal.y * this->normal.y) * this->radius;
        float centroid_z = sqrtf(1.0f - this->normal.z * this->normal.z) * this->radius;

        Point start = Point(this->center.x - centroid_x, this->center.y - centroid_y, this->center.z - centroid_z);
        Point end = Point(this->center.x + centroid_x, this->center.y + centroid_y, this->center.z + centroid_z);
        return BBox(start, end);
    }

    Intersection Disc::intersect(const Ray &ray, float tmin, float tmax) const
    {
        float denominator = dot(this->normal, ray.d);
        Vector tip_on_the_disk = this->center - ray.o;
        float numerator = dot(tip_on_the_disk, this->normal);
        float t = numerator / denominator;

        if (fabs(denominator) < epsilon || t > tmax || t <= tmin)
            return Intersection::failure();

        Vector local_vector = ray.getPoint(t) - this->center;
        Point local_point = Point(local_vector.x, local_vector.y, local_vector.z);

        if (local_vector.length() > this->radius)
            return Intersection::failure();

        return Intersection(t, ray, this, this->normal, local_point);
    }

    Solid::Sample Disc::sample() const
    {
        NOT_IMPLEMENTED;
    }

    float Disc::getArea() const
    {
        return pi * radius * radius;
    }

}
