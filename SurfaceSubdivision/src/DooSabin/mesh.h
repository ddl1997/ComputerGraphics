#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

namespace DooSabin {

    struct vertex {
        double x, y, z;
        std::vector<int> f;
        std::vector<int> e;
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
    };

    struct face {
        std::vector<int> v;
        vertex avg;
    };

    class mesh {
    public:

        mesh() : faces(std::vector<face>(0)), vertices(std::vector<vertex>(0)), edges() {}

        mesh(std::string filename);

        void subdivise();

        void restruct();

        std::vector<std::vector<double>> getFaceList();

        void toFile(std::string filename);

    private:

        std::vector<face> faces;
        std::vector<vertex> vertices;
        edgeList edges;
    };

}


#endif // !MESH_H
