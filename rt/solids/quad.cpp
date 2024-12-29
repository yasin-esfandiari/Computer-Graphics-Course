#include <rt/solids/quad.h>

namespace rt
{

    Quad::Quad(const Point &origin, const Vector &span1, const Vector &span2, CoordMapper *texMapper, Material *material)
        : Solid(texMapper, material), origin(origin), span1(span1), span2(span2)
    {
        this->normal_vector = cross(span1, span2).normalize();
    }

    BBox Quad::getBounds() const
    {
        Point k = this->origin + this->span1 + this->span2;
        return BBox(min(this->origin, k), max(this->origin, k));
    }

    Intersection Quad::intersect(const Ray &ray, float tmin, float tmax) const
    {
        Vector d = this->origin - ray.o;
        float t = dot(d, this->normal_vector) / dot(ray.d, this->normal_vector);

        if (t <= tmin || t >= tmax)
            return Intersection::failure();

        Point V2 = this->origin + this->span1;
        Point V3 = this->origin + this->span2;
        Vector V1V2 = cross(V2 - ray.o, this->origin - ray.o);
        Vector V2V3 = cross(V3 - ray.o, V2 - ray.o);
        Vector V1V3 = cross(this->origin - ray.o, V3 - ray.o);

        float gamma3 = dot(V1V2, ray.d);
        float gamma2 = dot(V1V3, ray.d);
        float gamma1 = dot(V2V3, ray.d);

        float sum = gamma1 + gamma2 + gamma3;
        gamma2 /= sum;
        gamma1 /= sum;
        gamma3 /= sum;

        if (gamma2 < 0 || 1 < gamma2 || gamma3 < 0 || 1 < gamma3)
            return Intersection::failure();

        Point local_point = Point(gamma2, gamma3, 0);
        return Intersection(t, ray, this, this->normal_vector, local_point);
    }

    Solid::Sample Quad::sample() const
    {
        Point hit = this->origin + random() * this->span1 + random() * this->span2;
        Sample obj;
        obj.point = hit;
        obj.normal = this->normal_vector;
        return obj;
    }

    float Quad::getArea() const
    {
        return cross(this->span1, this->span2).length();
    }

}
