#ifndef SCANLINE_H
#define SCANLINE_H

#include <utils/model.h>
#include <utils/camera.h>
#include <utils/buffer.h>
#include <Eigen/Dense>
#include <vector>
#include <algorithm>
#include <math.h>
#include "utils/global.h"

namespace scanline {

    struct edge {
        float ymax;
        float ymin;
        float x;
        float dx;
        bool isHorizontal;
    };

    struct triFace {
        edge edges[3];
        vertex vertices[3];
        Eigen::Vector2f lu; // left up
        Eigen::Vector2f rb; // right bottom
    };

    struct triangle {
        std::vector<edge> edges;
        Eigen::Vector4f vertices[3];
        Eigen::Vector2f texs[3];
        Eigen::Vector2f lu; // left up
        Eigen::Vector2f rb; // right bottom
    };

    class scanlineRasterizer {
    public:
        scanlineRasterizer()
        {
            color.init(IMAGE_WIDTH, IMAGE_HEIGHT, Eigen::Vector3f(0, 0, 0));
            depth.init(IMAGE_WIDTH, IMAGE_HEIGHT, std::numeric_limits<float>::infinity());
        }

        // 对外提供的渲染接口
        buffer2D<Eigen::Vector3f> rasterize(Model m, Camera c, texture tex)
        {
            auto faces = preprocess(m, c);
            render(faces, tex);
            return color;
        }

    private:
        buffer2D<Eigen::Vector3f> color;
        buffer2D<float> depth;

        static bool edgeCmp(edge a, edge b)
        {
            return (a.ymin < b.ymin) || (a.ymin == b.ymin && a.dx < b.dx);
        }

        // 获取三者之中最大
        float getMaxOfThree(float a, float b, float c)
        {
            return a > b ? (a > c ? a : c) : (b > c ? b : c);
        }

        // 获取三者之中最小
        float getMinOfThree(float a, float b, float c)
        {
            return a < b ? (a < c ? a : c) : (b < c ? b : c);
        }

        // 对面片进行预处理（坐标变换等）
        std::vector<triangle> preprocess(Model m, Camera c);

        // 着色函数
        void render(std::vector<triangle> faces, texture tex);

        // 重心坐标插值
        std::tuple<float, float, float> computeBarycentric2D(float x, float y, Eigen::Vector4f v[]);

        // 设置像素颜色
        void setColor(float x, float y, triangle t, texture tex, float sample_ratio);
    };
}

#endif // !SCANLINE_H

