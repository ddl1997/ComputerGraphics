#ifndef SCANLINE_H
#define SCANLINE_H

#include <utils/model.h>
#include <utils/camera.h>
#include <utils/buffer.h>
#include <Eigen/Dense>
#include <vector>
#include <algorithm>

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

    bool edgeCmp(edge a, edge b)
    {
        return (a.ymin < b.ymin) || (a.ymin == b.ymin && a.dx < b.dx);
    }

    struct triFace {
        edge edges[3];
        vertex vertices[3];
        Eigen::Vector2f lu; // left up
        Eigen::Vector2f rb; // right bottom
    };

    struct triangle {
        edge edges[3];
        Eigen::Vector4f vertices[3];
        Eigen::Vector2f texs[3];
        Eigen::Vector2f lu; // left up
        Eigen::Vector2f rb; // right bottom
    };

    struct scanlineSample {
        std::vector<vertex> v;
    };

    std::vector<triangle> preprocess(Model m, Camera c)
    {
        auto matrix = c.getTransMatrix();
        auto meshes = m.meshes;
        std::vector<triangle> tris;
        int meshSize = meshes.size();
        for (auto mesh : meshes)
        {
            auto faces = mesh.faces;
            int faceSize = faces.size();
            for (auto f : faces)
            {
                std::vector<face> temp;
                if (f.edgeSize == 3)
                {
                    temp.push_back(f);
                }
                else
                {
                    int splitSize = f.edgeSize - 2;
                    for (int i = 0; i < splitSize; i++)
                    {
                        face newFace;
                        newFace.edgeSize = 3;
                        newFace.vertices.push_back(f.vertices[0]);
                        newFace.vertices.push_back(f.vertices[i + 1]);
                        newFace.vertices.push_back(f.vertices[i + 2]);
                        temp.push_back(newFace);
                    }
                }
                for (auto newF : temp)
                {
                    triangle tri;
                    auto vs = newF.vertices;
                    for (int i = 0; i < 3; i++)
                    {
                        tri.vertices[i] = matrix * Eigen::Vector4f(vs[i].Position, 1.0);
                        tri.vertices[i] /= tri.vertices[i].w();
                        // 转换到视口坐标系 [(0,0) ~ (width,height)]
                        tri.vertices[i].x() = 0.5 * IMAGE_WIDTH * (tri.vertices[i].x() + 1);
                        tri.vertices[i].y() = 0.5 * IMAGE_HEIGHT * (tri.vertices[i].y() + 1);
                    }
                        
                    float xmin = getMinOfThree(tri.vertices[0].x(), tri.vertices[1].x(), tri.vertices[2].x()),
                        xmax = getMaxOfThree(tri.vertices[0].x(), tri.vertices[1].x(), tri.vertices[2].x()),
                        ymin = getMinOfThree(tri.vertices[0].y(), tri.vertices[1].y(), tri.vertices[2].y()),
                        ymax = getMaxOfThree(tri.vertices[0].y(), tri.vertices[1].y(), tri.vertices[2].y());
                    tri.lu = Eigen::Vector2f(xmin, ymax);
                    tri.rb = Eigen::Vector2f(xmax, ymin);
                    std::vector<edge> temp;
                    for (int i = 0; i < 3; i++)
                    {
                        auto v1 = tri.vertices[i];
                        auto v2 = tri.vertices[(i + 1) % 3];
                        float x1 = v1.x(), y1 = v1.y(), x2 = v2.x(), y2 = v2.y();
                        edge e;
                        if (y1 == y2)
                            e.isHorizontal = true;
                        else
                        {
                            e.isHorizontal = false;
                            e.ymax = y1 > y2 ? y1 : y2;
                            e.ymin = y1 < y2 ? y1 : y2;
                            e.x = y1 < y2 ? x1 : x2;
                            e.dx = (x1 - x2) / (y1 - y2);
                        }
                        temp.push_back(e);
                    }
                    tris.push_back(tri);
                }
            }
        }
        return tris;
    }

    buffer2D<Eigen::Vector3f> render(std::vector<triangle> faces, Eigen::Vector3f background)
    {
        buffer2D<Eigen::Vector3f> color;
        color.init(background);
        buffer2D<float> depth;
        depth.init(std::numeric_limits<float>::infinity());

        for (auto f : faces)
        {
            float start = std::floor(f.rb.y()) - 0.5;
            float end = std::ceil(f.lu.y()) + 0.5;

            for (; start <= end; start += 1)
            {
                Eigen::Vector2f intersects[3] = { Eigen::Vector2f(-1, -1), Eigen::Vector2f(-1, -1), Eigen::Vector2f(-1, -1) };
                for (int i = 0; i < 3; i++)
                {
                    auto e = f.edges[i];
                    if (e.isHorizontal)
                        continue;
                    else if (e.ymin < start)
                    {
                        intersects.push_back(Eigen::Vector2f(e.x + e.dx * (start)));
                    }

                }
            }

        }

        return color;
    }
}

#endif // !SCANLINE_H

