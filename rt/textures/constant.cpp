#include <rt/textures/constant.h>

namespace rt {

ConstantTexture::ConstantTexture():color(RGBColor::rep(0.0f)){}

ConstantTexture::ConstantTexture(const RGBColor& color):color(color){}

RGBColor ConstantTexture::getColor(const Point& coord) {
    return this->color;
}

RGBColor ConstantTexture::getColorDX(const Point& coord) {
    return RGBColor::rep(0.0f);
}

RGBColor ConstantTexture::getColorDY(const Point& coord) {
    return RGBColor::rep(0.0f);
}

}