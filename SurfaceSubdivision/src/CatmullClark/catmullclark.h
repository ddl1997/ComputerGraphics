#ifndef CATMULL_CLARK_MESH_H
#define CATMULL_CLARK_MESH_H

#include <vector>
#include <string>
#include <set>

namespace CatmullClark {

    struct vertex {
        double x, y, z;
        std::vector<int> f;
        std::set<int> e;

        vertex() {};

        vertex(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
    };

    struct edge {
        int v[2];
        int f[2];
        int newV;
    };

    class edgeList {
    public:
        std::vector<edge> edges;

        edgeList() : edges(std::vector<edge>(0)) {};

        int insert(edge e);

        void clear() { edges.clear(); };

        int edgeCmp(edge a, edge b)
        {
            if (a.v[0] == b.v[0] && a.v[1] == b.v[1])
                return 0;
            else if (a.v[0] < b.v[0] || (a.v[0] == b.v[0] && a.v[1] < b.v[1]))
                return 1;
            else
                return -1;
        }
    };

    struct quadrangle {
        std::vector<int> v;
        std::vector<int> e;
        int avgV;
    };

    class mesh {
    public:
        mesh() : faces(std::vector<quadrangle>(0)), vertices(std::vector<vertex>(0)), edges() {}

        mesh(std::string filename);

        void subdivide();

        void construct();

        void toFile(std::string filename);

        void setAlpha(double _alpha) { alpha = _alpha; }

        void setBeta(double _beta) { beta = _beta; }

    private:
        std::vector<vertex> vertices;
        std::vector<quadrangle> faces;
        edgeList edges;

        double alpha = 1.0; // 细分调整原始顶点时平均邻边中点的权重
        double beta = 2.0; // 细分调整原始顶点时平均邻面中点的权重

        void split(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ")
        {
            std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
            std::string::size_type pos = s.find_first_of(delimiters, lastPos);
            while (std::string::npos != pos || std::string::npos != lastPos)
            {
                tokens.push_back(s.substr(lastPos, pos - lastPos));
                lastPos = s.find_first_not_of(delimiters, pos);
                pos = s.find_first_of(delimiters, lastPos);
            }
        }
    };
}
#endif // !CATMULL_CLARK_MESH_H
