#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <Eigen/Dense>
#include "utils/global.h"

enum TextureType{
     diffuse,
     specular
};

class texture {
public:
    int width, height, nrChannels;
    TextureType type;

    texture() 
    {
        width = height = nrChannels = 0;
        data = nullptr;
    }

    // 读取纹理文件
    bool loadFile(std::string filename, TextureType type);

    // 获取指定位置颜色
    Eigen::Vector3f getColor(int col, int row);

private:
    unsigned char* data; // 纹理数据
};
#endif // !TEXTURE_H
