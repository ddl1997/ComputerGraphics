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

    // ��ȡ�����ļ�
    bool loadFile(std::string filename, TextureType type);

    // ��ȡָ��λ����ɫ
    Eigen::Vector3f getColor(int col, int row);

private:
    unsigned char* data; // ��������
};
#endif // !TEXTURE_H
