#include <core/color.h>
#include <core/scalar.h>
#include <core/homogeneouscoord.h>
#include <core/assert.h>

namespace rt {

RGBColor::RGBColor(const HomogeneousCoord& coord)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    return RGBColor(r+c.r, g+c.g, b+c.b);
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    return RGBColor(r-c.r, g-c.g, b-c.b);
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    return RGBColor(r*c.r, g*c.g, b*c.b);
}

bool RGBColor::operator == (const RGBColor& c) const {
    float diff_r = std::abs(r - c.r);
    float diff_g = std::abs(g - c.g);
    float diff_b = std::abs(b - c.b);

    if (diff_r<epsilon && diff_g<epsilon && diff_b<epsilon)
        return 1;
    else
        return 0;
}

bool RGBColor::operator != (const RGBColor& b) const {
    float diff_r = std::abs(this->r - b.r);
    float diff_g = std::abs(this->g - b.g);
    float diff_b = std::abs(this->b - b.b);

    if (diff_r<epsilon && diff_g<epsilon && diff_b<epsilon)
        return 0;
    else
        return 1;
}

RGBColor RGBColor::clamp() const {
    float temp_r = std::max(0.0f, std::min(r, 1.0f));
    float temp_g = std::max(0.0f, std::min(g, 1.0f));
    float temp_b = std::max(0.0f, std::min(b, 1.0f));

    return RGBColor(temp_r, temp_g, temp_b);
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
    return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);

}

RGBColor operator * (const RGBColor& c, float scalar) {
    return RGBColor(c.r*scalar, c.g*scalar, c.b*scalar);
}

RGBColor operator / (const RGBColor& c, float scalar) {
    rt_assert(scalar!=0) << "Zero division error!!!";
    return RGBColor(c.r/scalar, c.g/scalar, c.b/scalar);
}

}
