#ifndef DOOSABIN_MESH_H
#define DOOSABIN_MESH_H

#include <string>
#include <vector>
#include <set>

namespace DooSabin {

    struct vertex {
        double x, y, z;
        std::vector<int> f;
        std::set<int> e;
    };

    struct edge {
        int v[2];
        int f[2];
    };

    class edgeList {
    public:
        std::vector<edge> edges;

        edgeList() : edges(std::vector<edge>(0)) {};

        int insert(edge e);

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

    struct face {
        std::vector<int> v;
        vertex avg;
    };

    class mesh {
    public:

        mesh() : faces(std::vector<face>(0)), vertices(std::vector<vertex>(0)), edges() {}

        mesh(std::string filename);

        void subdivide();

        void restruct();

        std::vector<double> getVertices();

        std::vector<std::vector<int>> getFaceList();

        void toFile(std::string filename);

    private:

        std::vector<face> faces;
        std::vector<vertex> vertices;
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
