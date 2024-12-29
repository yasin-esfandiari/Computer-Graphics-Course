#include <rt/solids/striangle.h>
#include <rt/intersection.h>
#include <cmath>

namespace rt
{

    SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper *texMapper, Material *material)
    {
        this->texMapper = texMapper;
        this->material = material;
        this->v1 = vertices[0];
        this->v2 = vertices[1];
        this->v3 = vertices[2];
        this->n1 = normals[0];
        this->n2 = normals[1];
        this->n3 = normals[2];
        this->normal = cross(this->v2 - this->v1, this->v3 - this->v1).normalize();
    }

    SmoothTriangle::SmoothTriangle(const Point &v1, const Point &v2, const Point &v3, const Vector &n1, const Vector &n2, const Vector &n3, CoordMapper *texMapper, Material *material)
    {
        this->texMapper = texMapper;
        this->material = material;
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->n1 = n1;
        this->n2 = n2;
        this->n3 = n3;
        this->normal = cross(this->v2 - this->v1, this->v3 - this->v1).normalize();
    }

    Intersection SmoothTriangle::intersect(const Ray &ray, float tmin, float tmax) const
    {
        Vector d = this->v1 - ray.o;
        float t = dot(d, this->normal) / dot(ray.d, this->normal);

        if (t <= (fabs(tmin) - epsilon) || t > tmax)
            return Intersection::failure();

        Vector nv1v2 = cross(this->v2 - ray.o, this->v1 - ray.o);
        Vector nv2v3 = cross(this->v3 - ray.o, this->v2 - ray.o);
        Vector nv1v3 = cross(this->v1 - ray.o, this->v3 - ray.o);

        float lambda3 = dot(nv1v2, ray.d);
        float lambda2 = dot(nv1v3, ray.d);
        float lambda1 = dot(nv2v3, ray.d);

        float normFact = (lambda1 + lambda2 + lambda3);

        lambda2 /= normFact;
        lambda1 /= normFact;
        lambda3 = 1 - lambda1 - lambda2;

        if (lambda1 < 0.0f || lambda1 > 1.0f)
            return Intersection::failure();

        if (lambda2 < 0.0f || lambda2 > 1.0f)
            return Intersection::failure();

        if (lambda3 < 0.0f || lambda3 > 1.0f)
            return Intersection::failure();
        Vector newNormal = lambda1 * this->n1 + lambda2 * this->n2 + lambda3 * this->n3;
        return Intersection(t, ray, this, newNormal.normalize(), Point(lambda1, lambda2, lambda3));
    }

}