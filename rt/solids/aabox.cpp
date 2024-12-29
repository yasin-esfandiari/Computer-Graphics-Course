#include <rt/solids/aabox.h>
#include <algorithm>
#include <rt/ray.h>

using namespace std;
namespace rt
{

    AABox::AABox(const Point &corner1, const Point &corner2, CoordMapper *texMapper, Material *material)
    : Solid(texMapper, material), corner1(corner1), corner2(corner2)
    {}

    BBox AABox::getBounds() const
    {
        return BBox(min(corner1, corner2), max(corner1, corner2));
    }

    Solid::Sample AABox::sample() const
    {
        NOT_IMPLEMENTED;
    }

    float AABox::getArea() const
    {
        float width = fabs(this->corner1.y - this->corner2.y);
        float height = fabs(this->corner1.z - this->corner2.z);
        float length = fabs(this->corner1.x - this->corner2.x);

        return 2.0f * (width * height + height * length + length * width);
    }

    Intersection AABox::intersect(const Ray &ray, float tmin, float tmax) const
    {
        float tx_min = (corner1.x - ray.o.x) / ray.d.x;
        float tx_max = (corner2.x - ray.o.x) / ray.d.x;
        float ty_min = (corner1.y - ray.o.y) / ray.d.y;
        float ty_max = (corner2.y - ray.o.y) / ray.d.y;
        float tz_min = (corner1.z - ray.o.z) / ray.d.z;
        float tz_max = (corner2.z - ray.o.z) / ray.d.z;

        if (tx_min > tx_max)
            swap(tx_min, tx_max);
        if (ty_min > ty_max)
            swap(ty_min, ty_max);
        if (tz_min > tz_max)
            swap(tz_min, tz_max);

        int norm_axis = 0;
        float t_min = max({tx_min, ty_min, tz_min});
        float t_max = min({tx_max, ty_max, tz_max});

        if (t_min == ty_min)
            norm_axis = 1;
        if (t_min == tz_min)
            norm_axis = 2;

        if (t_min > t_max || t_min > tmax || t_min < tmin)
            return Intersection::failure();
        else
        {
            Vector normal = Vector(0.0f, 0.0f, 0.0f);
            if (norm_axis == 0)
            {
                if (ray.d.x > 0)
                    normal = Vector(-1.0f, 0.0f, 0.0f);
                else
                    normal = Vector(1.0f, 0.0f, 0.0f);
            }
            if (norm_axis == 1)
            {
                if (ray.d.y > 0)
                    normal = Vector(0.0f, -1.0f, 0.0f);
                else
                    normal = Vector(0.0f, 1.0f, 0.0f);
            }
            if (norm_axis == 2)
            {
                if (ray.d.z > 0)
                    normal = Vector(0.0f, 0.0f, -1.0f);
                else
                    normal = Vector(0.0f, 0.0f, 1.0f);
            }

            // Point center = Point(0.5 * (this->corner1.x + this->corner2.x), 0.5 * (this->corner1.y + this->corner2.y), 0.5 * (this->corner1.z + this->corner2.z));
            Point hitpoint = ray.getPoint(t_min);
            Point local_point = Point(hitpoint.x, hitpoint.y, hitpoint.z);

            return Intersection(t_min, ray, this, normal, local_point);
        }
    }

}
