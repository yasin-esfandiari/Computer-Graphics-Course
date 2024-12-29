#include <core/homogeneouscoord.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/assert.h>
#include <core/scalar.h>

namespace rt
{

    HomogeneousCoord::HomogeneousCoord(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {}

    HomogeneousCoord::HomogeneousCoord(const Point &p)
        : x(p.x), y(p.y), z(p.z), w(1) {}

    HomogeneousCoord::HomogeneousCoord(const Vector &vec)
        : x(vec.x), y(vec.y), z(vec.z), w(0) {}

    float &HomogeneousCoord::operator[](int idx)
    {
        switch (idx)
        {
        case 0:
            return this->x;
            break;
        case 1:
            return this->y;
            break;
        case 2:
            return this->z;
            break;
        case 3:
            return this->w;
            break;
        default:
            return this->dflt; // a quick fix to warning
            break;
        }
    }

    float HomogeneousCoord::operator[](int idx) const
    {
        switch (idx)
        {
        case 0:
            return this->x;
            break;
        case 1:
            return this->y;
            break;
        case 2:
            return this->z;
            break;
        case 3:
            return this->w;
            break;
        default:
            return this->dflt; // a quick fix to warning
            break;
        }
    }

    HomogeneousCoord HomogeneousCoord::operator+(const HomogeneousCoord &b) const
    {
        return HomogeneousCoord(this->x + b.x, this->y + b.y, this->z + b.z, this->w + b.w);
    }

    HomogeneousCoord HomogeneousCoord::operator-(const HomogeneousCoord &b) const
    {
        return HomogeneousCoord(this->x - b.x, this->y - b.y, this->z - b.z, this->w - b.w);
    }

    HomogeneousCoord HomogeneousCoord::operator*(const HomogeneousCoord &b) const
    {
        return HomogeneousCoord(this->x * b.x, this->y * b.y, this->z * b.z, this->w * b.w);
    }

    HomogeneousCoord HomogeneousCoord::operator/(const HomogeneousCoord &b) const
    {
        // CHANGED
        // assert(b != HomogeneousCoord::rep(0.0)) << "can not be divided by zero!";
        return HomogeneousCoord(this->x / b.x, this->y / b.y, this->z / b.z, this->w / b.w);
    }

    HomogeneousCoord operator*(float scalar, const HomogeneousCoord &b)
    {
        return HomogeneousCoord(scalar * b.x, scalar * b.y, scalar * b.z, scalar * b.w);
    }

    HomogeneousCoord operator*(const HomogeneousCoord &a, float scalar)
    {
        return HomogeneousCoord(a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar);
    }

    HomogeneousCoord operator/(const HomogeneousCoord &a, float scalar)
    {
        return HomogeneousCoord(a.x / scalar, a.y / scalar, a.z / scalar, a.w / scalar);
    }

    float dot(const HomogeneousCoord &a, const HomogeneousCoord &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    HomogeneousCoord HomogeneousCoord::operator-() const
    {
        return HomogeneousCoord(-this->x, -this->y, -this->z, -this->w);
    }

    bool HomogeneousCoord::operator==(const HomogeneousCoord &b) const
    {
        float diff_x = fabs(this->x - b.x);
        float diff_y = fabs(this->y - b.y);
        float diff_z = fabs(this->z - b.z);
        float diff_w = fabs(this->w - b.w);

        if (diff_x < epsilon && diff_y < epsilon && diff_z < epsilon && diff_w < epsilon)
            return 1;
        else
            return 0;
    }

    bool HomogeneousCoord::operator!=(const HomogeneousCoord &b) const
    {
        float diff_x = fabs(this->x - b.x);
        float diff_y = fabs(this->y - b.y);
        float diff_z = fabs(this->z - b.z);
        float diff_w = fabs(this->w - b.w);

        if (diff_x < epsilon && diff_y < epsilon && diff_z < epsilon && diff_w < epsilon)
            return 0;
        else
            return 1;
    }

    HomogeneousCoord min(const HomogeneousCoord &a, const HomogeneousCoord &b)
    {
        return HomogeneousCoord(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
    }

    HomogeneousCoord max(const HomogeneousCoord &a, const HomogeneousCoord &b)
    {
        return HomogeneousCoord(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
    }

}