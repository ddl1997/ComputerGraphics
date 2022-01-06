#include <iostream>
#include <utils/buffer.h>
#include <utils/render.h>
#include <utils/model.h>
#include <scanline.h>
#include <Eigen/Dense>
#include "utils/texture.h"

using namespace std;

int main()
{
    /*int width = 800, height = 600;
    buffer2D<Eigen::Vector3f> buf(width, height);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            float color = (x * y * 255.0) / (width * height);
            buf.set(y, x, Eigen::Vector3f{color, color, color});
        }*/
    Model m("Spot/spot_triangulated_good.obj");
    scanline::scanlineRasterizer sr;
    Camera c;
    texture t;
    if (t.loadFile("Spot/spot_texture.png", TextureType::diffuse))
    {
        auto buf = sr.rasterize(m, c, t);
        renderToImage("test.jpg", buf);
    }
    else
    {
        cout << "Texture file not found!" << endl;
    }


    return 0;
}