#include <rt/bbox.h>
#include <algorithm>
#include <rt/ray.h>

namespace rt
{

    BBox BBox::empty()
    {
        return BBox();
    }

    BBox BBox::full()
    {
        return BBox(
            Point(-__FLT_MAX__, -__FLT_MAX__, -__FLT_MAX__),
            Point(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__));
    }

    void BBox::extend(const Point &point)
    {
        if (this->is_empty)
        {
            this->min = point;
            this->max = point;
            this->is_empty = false;
        }
        else
        {
            min = Point(std::min(point.x, this->min.x), std::min(point.y, this->min.y), std::min(point.z, this->min.z));
            max = Point(std::max(point.x, this->max.x), std::max(point.y, this->max.y), std::max(point.z, this->max.z));
        }
    }

    void BBox::extend(const BBox &bbox)
    {
        if (this->is_empty)
        {
            this->max = bbox.max;
            this->min = bbox.min;
            this->is_empty = false;
        }
        else
        {
            this->min = Point(std::min(bbox.min.x, this->min.x), std::min(bbox.min.y, this->min.y), std::min(bbox.min.z, this->min.z));
            this->max = Point(std::max(bbox.max.x, this->max.x), std::max(bbox.max.y, this->max.y), std::max(bbox.max.z, this->max.z));
        }
    }

    std::pair<float, float> BBox::intersect(const Ray &ray) const
    {
        std::pair<float, float> intersection;
        if (this->is_empty)
        {
            intersection.first = 1.0f;
            intersection.second = -1.0f;

            return intersection;
        }
        else
        {
            float tx_min = (min.x - ray.o.x) / ray.d.x;
            float tx_max = (max.x - ray.o.x) / ray.d.x;
            float ty_min = (min.y - ray.o.y) / ray.d.y;
            float ty_max = (max.y - ray.o.y) / ray.d.y;
            float tz_min = (min.z - ray.o.z) / ray.d.z;
            float tz_max = (max.z - ray.o.z) / ray.d.z;

            if (tx_min > tx_max)
                std::swap(tx_min, tx_max);
            if (ty_min > ty_max)
                std::swap(ty_min, ty_max);
            if (tz_min > tz_max)
                std::swap(tz_min, tz_max);

            float t_min = std::max({tx_min, ty_min, tz_min});
            float t_max = std::min({tx_max, ty_max, tz_max});

            intersection.first = t_min;
            intersection.second = t_max;

            return intersection;
        }
    }

    bool BBox::isUnbound() const
    {
        if (this->min.x <= -__FLT_MAX__ || this->min.y <= -__FLT_MAX__ || this->min.z <= -__FLT_MAX__ ||
            this->max.x >= __FLT_MAX__ || this->max.y >= __FLT_MAX__ || this->max.z >= __FLT_MAX__)
            return true;
        else
            return false;
    }

}