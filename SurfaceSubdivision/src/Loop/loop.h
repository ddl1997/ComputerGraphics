#ifndef LOOP_MESH_H
#define LOOP_MESH_H

#include <vector>
#include <string>
#include <set>

namespace Loop {

    const double PI = 3.14159265;

    struct vertex {
        double x, y, z;
        std::set<int> neighbors;
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

    struct triangle {
        int v[3];
        int e[3];
    };

    class mesh {
    public:
        mesh() : triangles(std::vector<triangle>(0)), vertices(std::vector<vertex>(0)), edges() {}

        mesh(std::string filename);

        void subdivide();

        void restruct();

        void toFile(std::string filename);

    private:
        std::vector<vertex> vertices;
        std::vector<triangle> triangles;
        edgeList edges;

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
#endif