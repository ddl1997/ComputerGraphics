#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

namespace DooSabin {

    struct vertex {
        double x, y, z;
        std::vector<int> f;
    };

    struct edge {
        int v[2];
        int f[2];
    };

    class edgeList {
    public:
        std::vector<edge> edges;

        edgeList() : edges(std::vector<edge>(0)) {};

        void insert(edge e);
    };

    struct face {
        std::vector<int> v;
    };

    class mesh {
    public:

        mesh() : faces(std::vector<face>(0)), vertices(std::vector<vertex>(0)), edges() {}

        mesh(std::string filename);

        std::vector<face> subdivise();

    private:

        std::vector<face> faces;
        std::vector<vertex> vertices;
        edgeList edges;
    };

}


#endif // !MESH_H
