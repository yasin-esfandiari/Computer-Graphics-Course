#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <cmath>

namespace rt
{

    class Ray;

    class BBox
    {
    public:
        Point min, max;

        BBox() : is_empty(true) {}
        BBox(const Point &min, const Point &max) : min(min), max(max), is_empty(false) {}

        static BBox empty();
        static BBox full();

        void extend(const Point &point);
        void extend(const BBox &bbox);

        Vector diagonal() const
        {
            return this->max - this->min;
        }

        float area() const
        {
            float width = fabs(this->max.y - this->min.y);
            float height = fabs(this->max.z - this->min.z);
            float length = fabs(this->max.x - this->min.x);

            return 2.0f * (width * height + height * length + length * width);
        }

        std::pair<float, float> intersect(const Ray &ray) const;

        bool isUnbound() const;

    private:
        bool is_empty;
    };

}

#endif