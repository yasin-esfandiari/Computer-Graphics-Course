#include <rt/solids/sphere.h>

namespace rt
{

    Sphere::Sphere(const Point &center, float radius, CoordMapper *texMapper, Material *material)
        : Solid(texMapper, material), center(center), radius(radius)
    {
    }

    BBox Sphere::getBounds() const
    {
        Point start = Point(center.x - radius, center.y - radius, center.z - radius);
        Point end = Point(center.x + radius, center.y + radius, center.z + radius);
        return BBox(start, end);
    }

    Intersection Sphere::intersect(const Ray &ray, float tmin, float tmax) const
    {
        Vector o_minus_c = ray.o - this->center;
        float a = ray.d.lensqr();
        float b = 2 * dot(ray.d, o_minus_c);
        float c = o_minus_c.lensqr() - (this->radius * this->radius);
        float discriminant = b * b - 4 * a * c;

        float min_hit_distance;
        // no root
        if (discriminant < 0)
            return Intersection::failure();
        // 1 root
        else if (discriminant == 0)
        {
            min_hit_distance = -(b / (2 * a));
            if (min_hit_distance < tmin) // tmin instead of 0
                return Intersection::failure();
        }
        // 2 roots
        else
        {
            float root1 = (-b + sqrtf(discriminant)) / (2 * a);
            float root2 = (-b - sqrtf(discriminant)) / (2 * a);
            // find the smalles positive root
            if (min(root1, root2) >= tmin) // tmin instead of 0
                min_hit_distance = min(root1, root2);
            else if (max(root1, root2) >= tmin) // tmin instead of 0
                min_hit_distance = max(root1, root2);
            else
                return Intersection::failure();
        }
        // if our calculated distance is worse than before
        if (min_hit_distance > tmax)
            return Intersection::failure();

        // we have found a better hit distance
        Point intersection_point = ray.getPoint(min_hit_distance);
        Vector normalVector = 2 * (intersection_point - center);
        // Vector local_vector = ray.getPoint(min_hit_distance) - center;
        // Point local_point = Point(local_vector.x, local_vector.y, local_vector.z);
        // return Intersection(min_hit_distance, ray, this, normalVector.normalize(), local_point);
        return Intersection(min_hit_distance, ray, this, normalVector.normalize(), intersection_point);
    }

    Solid::Sample Sphere::sample() const
    {
        NOT_IMPLEMENTED;
    }

    float Sphere::getArea() const
    {
        return 4.0f * pi * radius * radius;
    }

}
