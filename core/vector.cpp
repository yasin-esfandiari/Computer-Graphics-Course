#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/homogeneouscoord.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt
{

    Vector::Vector(float x, float y, float z)
        : x(x), y(y), z(z) {}

    Vector::Vector(const HomogeneousCoord &coord)
        : x(coord.x), y(coord.y), z(coord.z)
    {
        rt_assert(coord.w <= epsilon) << "for vectors, fourth component should be zero";
    }

    Vector Vector::operator+(const Vector &b) const
    {
        return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
    }

    Vector Vector::operator-(const Vector &b) const
    {
        return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
    }

    Vector Vector::operator-() const
    {
        return Vector(-this->x, -this->y, -this->z);
    }

    Vector Vector::normalize() const
    {
        float length = this->length();
        return Vector(this->x / length, this->y / length, this->z / length);
    }

    Vector operator*(float scalar, const Vector &b)
    {
        return Vector(b.x * scalar, b.y * scalar, b.z * scalar);
    }

    Vector operator*(const Vector &a, float scalar)
    {
        return scalar * a;
    }

    Vector operator/(const Vector &a, float scalar)
    {
        return (1 / scalar) * a;
    }

    Vector cross(const Vector &a, const Vector &b)
    {
        return Vector(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
    }

    float dot(const Vector &a, const Vector &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float Vector::lensqr() const
    {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    float Vector::length() const
    {
        return sqrtf(this->lensqr());
    }

    bool Vector::operator==(const Vector &b) const
    {
        return (this->x == b.x && this->y == b.y && this->z == b.z) ? 1 : 0;
    }

    bool Vector::operator!=(const Vector &b) const
    {
        return (this->x != b.x || this->y != b.y || this->z != b.z) ? 1 : 0;
    }

    Vector min(const Vector &a, const Vector &b)
    {
        return Vector(
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.z ? a.z : b.z);
    }

    Vector max(const Vector &a, const Vector &b)
    {
        return Vector(
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y,
            a.z > b.z ? a.z : b.z);
    }

    Point operator+(const Point &a, const Vector &b)
    {
        return Point(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    Point operator+(const Vector &a, const Point &b)
    {
        return b + a;
    }

    Point operator-(const Point &a, const Vector &b)
    {
        return Point(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    Point operator*(const HomogeneousCoord &scale, const Point &p)
    {
        /* TODO */ NOT_IMPLEMENTED;
    }

}
