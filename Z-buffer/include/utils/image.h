#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <vector>

using namespace cv;

struct Vec3 {
    float r;
    float g;
    float b;
};

void imageDecode(std::string filename, float up, float down)
{
    int width, height, nrChannels;
    std::string filePath = "resource/input/image/" + filename;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    float range = up - down;
    std::vector<Vec3> buf;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r = data[i * width + j];
            int g = data[i * width + j + 1];
            int b = data[i * width + j + 2];
            int a = data[i * width + j + 3];
            //printf("%d %d %d %d\n", (int)pixel[0], (int)pixel[1], (int)pixel[2], (int)pixel[3]);
            union
            {
                int i;
                float f;
            } u;
            u.i = (r << 24) + (g << 16) + (b << 8) + (a);
            float gray = (u.f - down) * 255.0 / range;
            if (gray > 255)
                gray = 255;
            if (gray < 0)
                gray = 0;
            Vec3 color = { gray, gray, gray };
            buf.push_back(color);
        }
    }
    Mat output(height, width, CV_32FC3, buf.data());
    output.convertTo(output, CV_8UC3, 1.0f);
    cv::imwrite("resource/output/image/" + filename, output);
}

#endif // !IMAGE_H
