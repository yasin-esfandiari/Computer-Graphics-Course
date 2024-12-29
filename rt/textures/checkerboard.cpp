#include <rt/textures/checkerboard.h>
#include <core/scalar.h>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black):white(white), black(black){}

RGBColor CheckerboardTexture::getColor(const Point& coord) {
    int x = (int)(2 * (coord.x - floor(coord.x)));
    int y = (int)(2 * (coord.y - floor(coord.y)));
    int z = (int)(2 * (coord.z - floor(coord.z)));

    if ((x + y + z) % 2 == 0)
        return this->white;
    else
        return this->black;
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

}