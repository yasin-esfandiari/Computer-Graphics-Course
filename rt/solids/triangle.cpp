#include <rt/solids/triangle.h>

namespace rt
{

    Triangle::Triangle(Point vertices[3], CoordMapper *texMapper, Material *material)
        : Solid(texMapper, material), v1(vertices[0]), v2(vertices[1]), v3(vertices[2])
    {
        this->normal_vector = cross(v3 - v2, v1 - v2).normalize();
    }

    Triangle::Triangle(const Point &v1, const Point &v2, const Point &v3, CoordMapper *texMapper, Material *material)
        : Solid(texMapper, material), v1(v1), v2(v2), v3(v3)
    {
        this->normal_vector = cross(v3 - v2, v1 - v2).normalize();
    }

    BBox Triangle::getBounds() const
    {
        return BBox(min(v1, min(v2, v3)), max(v1, max(v2, v3)));
    }

    Intersection Triangle::intersect(const Ray &ray, float tmin, float tmax) const
    {
        Vector d = this->v1 - ray.o;
        float t = dot(d, this->normal_vector) / dot(ray.d, this->normal_vector);

        if (t <= tmin || t > tmax)
            return Intersection::failure();

        Vector V1V2 = cross(this->v2 - ray.o, this->v1 - ray.o);
        Vector V2V3 = cross(this->v3 - ray.o, this->v2 - ray.o);
        Vector V1V3 = cross(this->v1 - ray.o, this->v3 - ray.o);
        float gamma3 = dot(V1V2, ray.d);
        float gamma2 = dot(V1V3, ray.d);
        float gamma1 = dot(V2V3, ray.d);

        float sum = gamma1 + gamma2 + gamma3;
        gamma2 /= sum;
        gamma1 /= sum;
        gamma3 /= sum;

        if (gamma1 < 0.0f || gamma1 > 1.0f || gamma2 < 0.0f || gamma2 > 1.0f || gamma3 < 0.0f || gamma3 > 1.0f)
            return Intersection::failure();

        Point local_point = Point(gamma1, gamma2, gamma3);
        return Intersection(t, ray, this, this->normal_vector, local_point);
    }

    Solid::Sample Triangle::sample() const
    {
        float gamma1 = random();
        float gamma2 = random();
        float gamma3 = random();
        float norm = gamma1 + gamma2 + gamma3;

        gamma1 /= norm;
        gamma2 /= norm;
        gamma3 /= norm;

        Point hit(
            this->v1.x * gamma1 + this->v2.x * gamma2 + this->v3.x * gamma3,
            this->v1.y * gamma1 + this->v2.y * gamma2 + this->v3.y * gamma3,
            this->v1.z * gamma1 + this->v2.z * gamma2 + this->v3.z * gamma3
        );  

        Sample obj;
        obj.point = hit;
        obj.normal = this->normal_vector;
        
        return obj;
    }

    float Triangle::getArea() const
    {
        return cross(this->v3 - this->v2, this->v1 - this->v2).length() / 2.0f;
    }

}