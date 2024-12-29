#include <rt/textures/imagetex.h>
#include <core/point.h>
#include <core/interpolate.h>

namespace rt {

ImageTexture::ImageTexture():interpolation_typ(NEAREST), border_typ(CLAMP){}

ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i):interpolation_typ(i), border_typ(bh)
{
    this->image_tex.readPNG(filename);
}

ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i):interpolation_typ(i), border_typ(bh), image_tex(image){}

float ImageTexture::border(float border, float point)
{
    if (this->border_typ == CLAMP)
    {
        if (point < 0)
            return 0;
        else if (point > border)
            return border;
        else
        {
            return point;
        }
    }
    else if (this->border_typ == MIRROR)
    {
        if (point < 0)
            point = -point;
        float frac_coord = fmod(point, border);
        if(fmod(point - frac_coord, 2 * border) == border)
        {    
            return border - frac_coord;
        }
        else
        {
            return frac_coord;   
        }
    }
    else
    {
        if (point < 0)
        {
            return border - fmod(-point, border);
        }
        return fmod(point, border); 
    }
    
}


RGBColor ImageTexture::getColor(const Point& coord) {
    float x = coord.x;
    float y = coord.y;

    if(this->border_typ == REPEAT)
    {
        x = (x > 0) ? x : x+1;
        y = (y > 0) ? y : y+1;
    }
    
    float scaled_x = (this->image_tex.width() - 1) * x;
    float scaled_y = (this->image_tex.height() - 1) * y;

    float Corner_x0 = this->border(this->image_tex.width() - 1, floor(scaled_x));
    float Corner_x1 = this->border(this->image_tex.width() - 1, ceil(scaled_x));
    float Corner_y0 = this->border(this->image_tex.height() - 1, floor(scaled_y));
    float Corner_y1 = this->border(this->image_tex.height() - 1, ceil(scaled_y));

    if (this->border_typ == MIRROR)
    {
        float temp;
        if (Corner_x1 < Corner_x0)
        {
            temp = Corner_x1;
            Corner_x1 = Corner_x0;
            Corner_x0 = temp;
        }
        if (Corner_y1 < Corner_y0)
        {
            temp = Corner_y1;
            Corner_y1 = Corner_y0;
            Corner_y0 = temp;
        }
    }

    float point_x = this->border(this->image_tex.width() - 1, scaled_x);
    float point_y = this->border(this->image_tex.height() - 1, scaled_y);

    RGBColor rgb_corner1 = this->image_tex(Corner_x0, Corner_y0);
    RGBColor rgb_corner2 = this->image_tex(Corner_x1, Corner_y0);
    RGBColor rgb_corner3 = this->image_tex(Corner_x1, Corner_y1);
    RGBColor rgb_corner4 = this->image_tex(Corner_x0, Corner_y1);

    float frac_x = point_x - Corner_x0;
    float frac_y = point_y - Corner_y0;

    if (this->interpolation_typ == NEAREST)
    {
        if (frac_x <= 0.5 && frac_y <= 0.5)
            return rgb_corner1;
        else if(frac_x > 0.5 && frac_y <= 0.5)
            return rgb_corner2;
        else if(frac_x > 0.5 && frac_y > 0.5)
            return rgb_corner3;
        else
            return rgb_corner4;    
    }
    else
    {
        return lerp2d(rgb_corner1, rgb_corner2, rgb_corner4, rgb_corner3, frac_x, frac_y);
    }   
}

RGBColor ImageTexture::getColorDX(const Point& coord) {
    Point new_coord = Point(coord.x + 0.0015, coord.y, coord.z);
    RGBColor x = this->getColor(coord);
    RGBColor x_delta = this->getColor(new_coord);

    return (x_delta - x);
}

RGBColor ImageTexture::getColorDY(const Point& coord) {
    Point new_coord = Point(coord.x, coord.y + 0.0015, coord.z);
    RGBColor y = this->getColor(coord);
    RGBColor y_delta = this->getColor(new_coord);

    return (y_delta - y);
}

}