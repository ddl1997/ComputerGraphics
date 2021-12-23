#ifndef SCANLINE_H
#define SCANLINE_H

#include <utils/model.h>
#include <utils/camera.h>
#include <utils/buffer.h>
#include <Eigen/Dense>
#include <vector>

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
                    }
                        
                    float xmin = getMinOfThree(tri.vertices[0].x(), tri.vertices[1].x(), tri.vertices[2].x()),
                        xmax = getMaxOfThree(tri.vertices[0].x(), tri.vertices[1].x(), tri.vertices[2].x()),
                        ymin = getMinOfThree(tri.vertices[0].y(), tri.vertices[1].y(), tri.vertices[2].y()),
                        ymax = getMaxOfThree(tri.vertices[0].y(), tri.vertices[1].y(), tri.vertices[2].y());
                    tri.lu = Eigen::Vector2f(xmin, ymax);
                    tri.rb = Eigen::Vector2f(xmax, ymin);
                    for (int i = 0; i < 3; i++)
                    {
                        auto v1 = tri.vertices[i];
                        auto v2 = tri.vertices[(i + 1) % 3];
                        float x1 = v1.x(), y1 = v1.y(), x2 = v2.x(), y2 = v2.y();
                        if (y1 == y2)
                            tri.edges[i].isHorizontal = true;
                        else
                        {
                            tri.edges[i].isHorizontal = false;
                            tri.edges[i].ymax = y1 > y2 ? y1 : y2;
                            tri.edges[i].ymin = y1 < y2 ? y1 : y2;
                            tri.edges[i].dx = (x1 - x2) / (y1 - y2);
                        }
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

        }

        return color;
    }
}

#endif // !SCANLINE_H

