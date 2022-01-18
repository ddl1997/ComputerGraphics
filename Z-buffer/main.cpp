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
    Model m(MODELFILE); // ����ģ���ļ�
    scanline::scanlineRasterizer sr; // ����ɨ������ɫ��
    Camera c; // �������
    texture t;
    if (t.loadFile(TEXTUREFILE, TextureType::diffuse)) // ���������ļ�
    {
        auto buf = sr.rasterize(m, c, t);
        renderToImage(OUTPUTFILE, buf);
    }
    else
    {
        cout << "Texture file not found!" << endl;
    }


    return 0;
}