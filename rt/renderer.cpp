#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <iostream>
#include <rt/cameras/camera.h>
#include <core/random.h>

namespace rt
{

    Renderer::Renderer(Camera *cam, Integrator *integrator)
        : cam(cam), integrator(integrator), samples(1)
    {
    }

    void Renderer::render(Image &img)
    {
        int img_width = img.width();
        int img_height = img.height();
        float x, y;

        if (samples == 1)
        {
            for (int i = 0; i < img_width; i++)
            {
                for (int j = 0; j < img_height; j++)
                {
                    x = (2.0f * (((float)i + 0.5f) / img_width) - 1.0f);
                    y = 2.0f * (((float)j + 0.5f) / img_height) - 1.0f;
                    y = -y;
                    Ray cam_ray = cam->getPrimaryRay(x, y);
                    img(i, j) = integrator->getRadiance(cam_ray);
                }
            }
        }
        else
        {
            for (int i = 0; i < img_width; i++)
            {
                for (int j = 0; j < img_height; j++)
                {
                    RGBColor color(0.0f, 0.0f, 0.0f);
                    for(unsigned int k = 0; k < this->samples; k++)
                    {
                        x = (2.0f * (((float)i + random()) / img_width) - 1.0f );
                        y = 2.0f * (((float)j + random()) / img_height) - 1.0f;
                        y = -y;
                        Ray cam_ray = this->cam->getPrimaryRay(x, y);
                        color = color + this->integrator->getRadiance(cam_ray);
                    }
                    color = color / this->samples;
                    img(i, j) = color;
                }
            }
        }
    }

}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt
{

    void Renderer::test_render1(Image &img)
    {
        for (unsigned int i = 0; i < img.width(); i++)
        {
            for (unsigned int j = 0; j < img.height(); j++)
            {
                img(i, j) = a1computeColor(i, j, img.width(), img.height());
            }
        }
    }
}

rt::RGBColor a2computeColor(const rt::Ray &r);

namespace rt
{

    void Renderer::test_render2(Image &img)
    {
        CG_UNUSED(img);
        float x, y;

        for (uint i = 0; i < img.width(); i++)
        {
            for (uint j = 0; j < img.height(); j++)
            {
                x = (float(i + 0.5f) / img.width() - 0.5f) * 2.0f;
                y = (float(j + 0.5f) / img.height() - 0.5f) * -2.0f;
                img(i, j) = a2computeColor(cam->getPrimaryRay(x, y));
            }
        }
    }

}
