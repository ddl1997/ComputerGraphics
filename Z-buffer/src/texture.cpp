#include "utils/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool texture::loadFile(std::string filename, TextureType type)
{
    data = stbi_load((RESOURCEDIR + filename).c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr) return false;
    this->type = type;
    return true;
}

Eigen::Vector3f texture::getColor(int col, int row)
{
    if (col >= width) col = width - 1;
    if (col < 0) col = 0;
    if (row >= height) row = height - 1;
    if (row < 0) row = 0;
    int index = (row * width + col) * nrChannels;
    float ratio = 1.0;
    float r = data[index] * ratio,
        g = data[index + 1] * ratio,
        b = data[index + 2] * ratio;
    return Eigen::Vector3f(r, g, b);
}