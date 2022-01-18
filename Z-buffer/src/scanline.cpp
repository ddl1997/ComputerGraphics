#include "scanline.h"
using namespace scanline;

std::vector<triangle> scanlineRasterizer::preprocess(Model m, Camera c) // Ԥ����
{
    auto matrix = c.getTransMatrix(ROTATEANGLE);
    Eigen::Matrix4f imageRotate; // ��x,y����������ĶԳƱ任���Եõ���ȷ�������Ⱦ���
    imageRotate << -1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
    matrix = imageRotate * matrix;
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
            else // ������ηָ�Ϊ��������Σ���������͹����Σ�������ģ�͵���ʱ���Զ���mesh�е���Ƭת��Ϊ��������Ƭ��������һ���ǲ���Ҫ�ģ�
            {
                /*int splitSize = f.edgeSize - 2;
                for (int i = 0; i < splitSize; i++)
                {
                    face newFace;
                    newFace.edgeSize = 3;
                    newFace.vertices.push_back(f.vertices[0]);
                    newFace.vertices.push_back(f.vertices[i + 1]);
                    newFace.vertices.push_back(f.vertices[i + 2]);
                    temp.push_back(newFace);
                }*/
            }
            for (auto newF : temp)
            {
                triangle tri;
                auto vs = newF.vertices;
                for (int i = 0; i < 3; i++)
                {
                    // ��ÿ��������Ƭ�Ķ����������任ͶӰ��ƽ��
                    tri.vertices[i] = matrix * Eigen::Vector4f(vs[i].Position.x(), vs[i].Position.y(), vs[i].Position.z(), 1.0);
                    tri.vertices[i] /= tri.vertices[i].w();
                    // ת�����ӿ�����ϵ [(0,0) ~ (width,height)]
                    int minL = IMAGE_WIDTH > IMAGE_HEIGHT ? IMAGE_HEIGHT : IMAGE_WIDTH;
                    tri.vertices[i].x() = 0.5 * minL * tri.vertices[i].x() + 0.5 * IMAGE_WIDTH;
                    tri.vertices[i].y() = 0.5 * minL * tri.vertices[i].y() + 0.5 * IMAGE_HEIGHT;
                    tri.texs[i] = vs[i].TexCoords;
                }

                // ����ƽ�������εľ��ΰ�Χ��
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
                    edge e;
                    if (y1 == y2) // �жϸñ��Ƿ�Ϊˮƽ����Ϊˮƽ����ɨ���߹����к���
                        e.isHorizontal = true;
                    else
                    {
                        e.isHorizontal = false;
                        e.ymax = y1 > y2 ? y1 : y2;
                        e.ymin = y1 < y2 ? y1 : y2;
                        e.x = y1 < y2 ? x1 : x2;
                        e.dx = (x1 - x2) / (y1 - y2);
                    }
                    tri.edges.push_back(e);
                }
                sort(tri.edges.begin(), tri.edges.end(), edgeCmp);
                tris.push_back(tri);
            }
        }
    }
    return tris;
}

// ���������ֵ
std::tuple<float, float, float> scanlineRasterizer::computeBarycentric2D(float x, float y, Eigen::Vector4f v[])
{
    float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
    float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
    float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
    return { c1,c2,c3 };
}

void scanlineRasterizer::setColor(float x, float y, triangle t, texture tex, float sample_ratio)
{
    auto ratio = computeBarycentric2D(x, y, t.vertices);
    float a = std::get<0>(ratio), b = std::get<1>(ratio), c = std::get<2>(ratio);
    float w_reciprocal = 1.0 / (a / t.vertices[0].w() + b / t.vertices[1].w() + c / t.vertices[2].w());
    float z_interpolated = a * t.vertices[0].z() / t.vertices[0].w() + b * t.vertices[1].z() / t.vertices[1].w() + c * t.vertices[2].z() / t.vertices[2].w();
    z_interpolated *= w_reciprocal; // �����ֵ���z����
    int col = x, row = floor(y);
    if (depth.get(row, col) > -z_interpolated)
    {
        depth.set(row, col, -z_interpolated);
        auto tex_id = a * t.texs[0] + b * t.texs[1] + c * t.texs[2]; // ��ֵ��������
        auto originColor = tex.getColor(tex_id.x() * tex.width, tex_id.y() * tex.height) * sample_ratio;
        Eigen::Vector3f pixColor(originColor.x() * sample_ratio, originColor.y() * sample_ratio, originColor.z() * sample_ratio);
        if (1 - sample_ratio > 0)
        {
            auto backColor = color.get(y, x);
            pixColor += Eigen::Vector3f(backColor.x() * (1 - sample_ratio), backColor.y() * (1 - sample_ratio), backColor.z() * (1 - sample_ratio));
        }
        color.set(y, x, pixColor);
    }
}

void scanlineRasterizer::render(std::vector<triangle> faces, texture tex)
{
    for (auto f : faces) // ��ÿ����Ƭ����ɨ��
    {
        // ���������εľ��ΰ�Χ�еõ�ɨ���ߵ��ƶ���Χ��
        float start = std::floor(f.rb.y()) - 0.5;
        float end = std::ceil(f.lu.y()) + 0.5;

        for (; start <= end; start += 1)
        {
            if (start < 0) continue;
            if (start >= IMAGE_HEIGHT) break;
            std::vector<Eigen::Vector2f> intersects;
            for (int i = 0; i < 3; i++) // ���㽻��
            {
                auto& e = f.edges[i];
                if (e.isHorizontal || e.ymin > start)
                    continue;
                else if (e.ymin <= start && e.ymax >= start)
                {
                    intersects.push_back(Eigen::Vector2f(e.x + e.dx * (start - e.ymin), start));
                }
            }
            if (intersects.size() == 2) // ֻ����������ɨ��������������ʱ���н��������
            {
                float xmin = intersects[0].x() > intersects[1].x() ? intersects[1].x() : intersects[0].x();
                float xmax = intersects[0].x() > intersects[1].x() ? intersects[0].x() : intersects[1].x();
                int xstart = floor(xmin), xend = floor(xmax); // ��ȡ��䷶Χ
                for (int i = xstart; i <= xend; i++)
                {
                    if (i < 0) continue;
                    if (i >= IMAGE_WIDTH) break; // ���������ֹͣ
                    float ratio = 1.0;
                    /*if (i == xstart)
                        ratio = xstart + 1 - xmin;
                    else if (i == xend)
                        ratio = xmax - xend;*/
                    setColor(i, start, f, tex, ratio); // �Ը����ؽ�����Ȳ��Բ���ɫ
                }
            }
        }
    }
}