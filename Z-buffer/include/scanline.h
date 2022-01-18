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

        // �����ṩ����Ⱦ�ӿ�
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

        // ��ȡ����֮�����
        float getMaxOfThree(float a, float b, float c)
        {
            return a > b ? (a > c ? a : c) : (b > c ? b : c);
        }

        // ��ȡ����֮����С
        float getMinOfThree(float a, float b, float c)
        {
            return a < b ? (a < c ? a : c) : (b < c ? b : c);
        }

        // ����Ƭ����Ԥ��������任�ȣ�
        std::vector<triangle> preprocess(Model m, Camera c);

        // ��ɫ����
        void render(std::vector<triangle> faces, texture tex);

        // ���������ֵ
        std::tuple<float, float, float> computeBarycentric2D(float x, float y, Eigen::Vector4f v[]);

        // ����������ɫ
        void setColor(float x, float y, triangle t, texture tex, float sample_ratio);
    };
}

#endif // !SCANLINE_H

