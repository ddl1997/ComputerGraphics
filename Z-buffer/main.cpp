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
    Model m(MODELFILE); // 导入模型文件
    scanline::scanlineRasterizer sr; // 创建扫描线着色器
    Camera c; // 创建相机
    texture t;
    if (t.loadFile(TEXTUREFILE, TextureType::diffuse)) // 导入纹理文件
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