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

    /*struct vertex {
        Eigen::Vector3f v;
        Eigen::Vector2f tex;
        Eigen::Vector3f color;
    };*/
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

        buffer2D<Eigen::Vector3f> rasterize(Model m, Camera c, texture tex)
        {
            auto faces = preprocess(m, c);
            render(faces, tex);
            return color;
        }

    private:
        buffer2D<Eigen::Vector3f> color;
        buffer2D<float> depth;

        bool edgeCmp(edge a, edge b)
        {
            return (a.ymin < b.ymin) || (a.ymin == b.ymin && a.dx < b.dx);
        }

        float getMaxOfThree(float a, float b, float c)
        {
            return a > b ? (a > c ? a : c) : (b > c ? b : c);
        }

        float getMinOfThree(float a, float b, float c)
        {
            return a < b ? (a < c ? a : c) : (b < c ? b : c);
        }

        std::vector<triangle> preprocess(Model m, Camera c);

        void render(std::vector<triangle> faces, texture tex);

        // 重心坐标插值
        std::tuple<float, float, float> computeBarycentric2D(float x, float y, Eigen::Vector4f v[]);

        Eigen::Vector3f setColor(float x, float y, triangle t, texture tex);
    };
}

#endif // !SCANLINE_H

