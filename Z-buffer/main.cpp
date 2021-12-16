#include <iostream>
#include <utils/buffer.h>
#include <utils/render.h>

using namespace std;

int main()
{
    int width = 800, height = 600;
    buffer2D<Eigen::Vector3f> buf(width, height);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            float color = (x * y * 255.0) / (width * height);
            buf.set(y, x, Eigen::Vector3f{color, color, color});
        }
            
    renderToImage("test.jpg", buf);


    return 0;
}