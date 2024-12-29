#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>
#include <core/random.h>

namespace rt
{

    class Triangle : public Solid
    {
    public:
        Triangle() {}
        Triangle(Point vertices[3], CoordMapper *texMapper, Material *material);
        Triangle(const Point &v1, const Point &v2, const Point &v3, CoordMapper *texMapper, Material *material);

        virtual BBox getBounds() const;
        virtual Intersection intersect(const Ray &ray, float tmin = 0, float tmax = FLT_MAX) const;
        virtual Sample sample() const;
        virtual float getArea() const;

    private:
        Point v1;
        Point v2;
        Point v3;

        Vector normal_vector;
    };

}

#endif