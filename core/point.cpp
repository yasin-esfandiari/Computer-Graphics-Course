#include <core/point.h>
#include <core/homogeneouscoord.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>
#include <cmath>

namespace rt
{

    Point::Point(float x, float y, float z)
        : x(x), y(y), z(z) {}

    Point::Point(const HomogeneousCoord &coord)
        : x(coord.x / coord.w), y(coord.y / coord.w), z(coord.z / coord.w)
    {
        rt_assert(fabs(coord.w) > epsilon) << "for points, fourth component should be non-zero";
    }

    Vector Point::operator-(const Point &b) const
    {
        return Vector(x - b.x, y - b.y, z - b.z);
    }
    Point Point::operator+(const Point &b) const
    {
        return Point(x + b.x, y + b.y, z + b.z);
    }

    float Point::operator[](const int index) const
    {
        if (index == 0)
            return x;
        else if (index == 1)
            return this->y;
        else if (index == 2)
            return this->z;
        else
            return -9999.9f; // a quick fix to warning
    }

    bool Point::operator==(const Point &b) const
    {
        float diff_x = fabs(x - b.x);
        float diff_y = fabs(y - b.y);
        float diff_z = fabs(z - b.z);

        if (diff_x < epsilon && diff_y < epsilon && diff_z < epsilon)
            return 1;
        else
            return 0;
    }

    bool Point::operator!=(const Point &b) const
    {
        float diff_x = fabs(x - b.x);
        float diff_y = fabs(y - b.y);
        float diff_z = fabs(z - b.z);

        if (diff_x < epsilon && diff_y < epsilon && diff_z < epsilon)
            return 0;
        else
            return 1;
    }

    Point operator*(float scalar, const Point &b)
    {
        return Point(scalar * b.x, scalar * b.y, scalar * b.z);
    }

    Point operator*(const Point &a, float scalar)
    {
        return Point(a.x * scalar, a.y * scalar, a.z * scalar);
    }

    Point min(const Point &a, const Point &b)
    {
        float min_x, min_y, min_z;
        if (a.x < b.x)
            min_x = a.x;
        else
            min_x = b.x;

        if (a.y < b.y)
            min_y = a.y;
        else
            min_y = b.y;

        if (a.z < b.z)
            min_z = a.z;
        else
            min_z = b.z;

        return Point(min_x, min_y, min_z);
    }

    Point max(const Point &a, const Point &b)
    {
        float max_x, max_y, max_z;
        if (a.x < b.x)
            max_x = b.x;
        else
            max_x = a.x;

        if (a.y < b.y)
            max_y = b.y;
        else
            max_y = a.y;

        if (a.z < b.z)
            max_z = b.z;
        else
            max_z = a.z;

        return Point(max_x, max_y, max_z);
    }

}
