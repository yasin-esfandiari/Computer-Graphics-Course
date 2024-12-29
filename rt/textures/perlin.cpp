#include <rt/textures/perlin.h>
#include <core/point.h>
#include <core/scalar.h>
#include <core/interpolate.h>

namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black):white(white), black(black){}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
    float perlin_noise = 0.0f;  
    int count = 0;
    for (float frequency  : this->freq)
    {
        Point coord_scaled = coord * frequency;
        
        int x_int = floor(coord_scaled.x);
        int y_int = floor(coord_scaled.y);
        int z_int = floor(coord_scaled.z);

        float x_frac = coord_scaled.x - floor(coord_scaled.x);
        float y_frac = coord_scaled.y - floor(coord_scaled.y);
        float z_frac = coord_scaled.z - floor(coord_scaled.z);
        
        float c1 = noise(x_int, y_int, z_int);
        float c2 = noise(x_int + 1, y_int, z_int);
        float c3 = noise(x_int, y_int + 1, z_int);
        float c4 = noise(x_int + 1, y_int + 1, z_int);
        float c5 = noise(x_int, y_int, z_int + 1);
        float c6 = noise(x_int + 1, y_int, z_int + 1);
        float c7 = noise(x_int, y_int + 1, z_int + 1);
        float c8 = noise(x_int + 1, y_int + 1, z_int + 1);

        float interpolated_noise = lerp3d(c1, c2, c3, c4, c5, c6, c7, c8, x_frac, y_frac, z_frac);
        interpolated_noise = interpolated_noise * this->amp.at(count);
        perlin_noise += interpolated_noise;
        count += 1;
    }

    // RGBColor color = lerp(this->black, this->white, perlin_noise);
    RGBColor color = lerp(this->black, this->white, (perlin_noise + 1.0)/2.0);
    
    return color;
}

rt::RGBColor PerlinTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);

    /* TODO */ NOT_IMPLEMENTED;
}

rt::RGBColor PerlinTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);

    /* TODO */ NOT_IMPLEMENTED;
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    this->freq.push_back(frequency);
    this->amp.push_back(amplitude);
}

}