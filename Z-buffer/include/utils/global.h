#ifndef GLOBAL_H
#define GLOBAL_H

// 进行一些项目设置

const int IMAGE_WIDTH = 800; // 生成图像的宽度
const int IMAGE_HEIGHT = 800; // 生成图像的高度

const float PI = 3.14159265358979323846;

const std::string RESOURCEDIR = "model/"; // 资源文件位置
const std::string MODELFILE = "Spot/spot_triangulated_good.obj"; // 模型文件名
const std::string TEXTUREFILE = "Spot/spot_texture.png"; // 纹理文件名

const float ROTATEANGLE = 180.0; // 模型在场景中的水平旋转角度

const std::string OUTPUTFILE = "output.png"; // 输出文件名
#endif // !GLOBAL_H
