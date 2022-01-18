#include "loop.h"
#include <fstream>

using namespace Loop;

int edgeList::insert(edge e)
{
    int index = 0;
    bool insertTag = false;
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        if ((e.v[0] == iter->v[0] && e.v[1] == iter->v[1]) || (e.v[0] == iter->v[1] && e.v[1] == iter->v[0]))
        {
            iter->f[1] = e.f[0];
            insertTag = true;
            break;
        }
        else index++;
    }
    if (!insertTag)
    {
        edges.push_back(e);
    }
    return index;
}

mesh::mesh(std::string filename)
{
    int strLen = filename.length();
    if (filename.substr(strLen - 3, 3) != "obj")
        return;
    std::ifstream obj(filename);
    if (!obj.is_open())
        return;
    std::string str;
    while (std::getline(obj, str))
    {
        std::string type = str.substr(0, 2);
        if (type == "v " || type == "f ")
        {
            std::vector<std::string> parts;
            split(str, parts, " ");
            if (parts[0] == "v")
            {
                vertices.push_back(vertex{ atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str()) });
            }
            else if (parts[0] == "f")
            {
                triangle _t;
                for (int i = 0; i < 3; i++)
                {
                    std::vector<std::string> _v;
                    split(parts[i + 1], _v, "/");
                    int vIndex = atoi(_v[0].c_str()) - 1;
                    _t.v[i] = vIndex;
                }
                triangles.push_back(_t);
            }
        }
    }

    construct();
}

void mesh::construct()
{
    edges.clear();

    int tSize = triangles.size();
    for (int i = 0; i < tSize; i++)
    {
        auto _t = &triangles[i];
        for (int j = 0; j < 3; j++)
        {
            int last = _t->v[(j + 2) % 3], next = _t->v[(j + 1) % 3];
            vertices[_t->v[j]].neighbors.insert(last);
            vertices[_t->v[j]].neighbors.insert(next);
            edge opposite;
            opposite.v[0] = last;
            opposite.v[1] = next;
            opposite.f[0] = i;
            int eIndex = edges.insert(opposite);
            _t->e[j] = eIndex;
        }
    }
}

void mesh::subdivide()
{
    std::vector<vertex> newVs;
    std::vector<triangle> newTs;

    for (auto vIter = vertices.begin(); vIter != vertices.end(); vIter++)
    {
        int nSize = (*vIter).neighbors.size();
        double ratio, originRatio;
        if (nSize == 2)
            ratio = 0.125;
        else
            ratio = (5.0 / 8.0 - pow(3.0 / 8.0 + cos(2 * PI / nSize) / 4.0, 2.0)) / (double)nSize;
        originRatio = 1.0 - nSize * ratio;
        double newX = originRatio * (*vIter).x, newY = originRatio * (*vIter).y, newZ = originRatio * (*vIter).z;
        for (auto nIter = (*vIter).neighbors.begin(); nIter != (*vIter).neighbors.end(); nIter++)
        {
            auto neighbor = vertices[*nIter];
            newX += ratio * neighbor.x;
            newY += ratio * neighbor.y;
            newZ += ratio * neighbor.z;
        }
        newVs.push_back(vertex{ newX, newY, newZ });
    }

    int newVSize = newVs.size();
    int eSize = edges.edges.size();
    for (int i = 0; i < eSize; i++)
    {
        auto _e = &edges.edges[i];
        auto v0 = vertices[_e->v[0]],
            v1 = vertices[_e->v[1]];
        double newX, newY, newZ;
        if (_e->f[0] >= 0 && _e->f[1] >= 0)
        {
            int f0eIndex = 0, f1eIndex = 0;
            while (triangles[_e->f[0]].e[f0eIndex] != i) f0eIndex++;
            while (triangles[_e->f[1]].e[f1eIndex] != i) f1eIndex++;
            auto f0OpV = vertices[triangles[_e->f[0]].v[f0eIndex]],
                f1OpV = vertices[triangles[_e->f[1]].v[f1eIndex]];
            newX = (3.0 * (v0.x + v1.x) + f0OpV.x + f1OpV.x) / 8.0;
            newY = (3.0 * (v0.y + v1.y) + f0OpV.y + f1OpV.y) / 8.0;
            newZ = (3.0 * (v0.z + v1.z) + f0OpV.z + f1OpV.z) / 8.0;
        }
        else
        {
            newX = (v0.x + v1.x) / 2.0;
            newY = (v0.y + v1.y) / 2.0;
            newZ = (v0.z + v1.z) / 2.0;
        }
        
        newVs.push_back(vertex{ newX, newY, newZ });
        _e->newV = newVSize++;
    }

    for (auto tIter = triangles.begin(); tIter != triangles.end(); tIter++)
    {
        triangle newT;
        for (int i = 0; i < 3; i++)
        {
            newT.v[i] = edges.edges[tIter->e[i]].newV;
            triangle t;
            t.v[0] = tIter->v[i];
            t.v[1] = edges.edges[tIter->e[(i + 1) % 3]].newV;
            t.v[2] = edges.edges[tIter->e[(i + 2) % 3]].newV;
            newTs.push_back(t);
        }
        newTs.push_back(newT);
    }

    vertices = newVs;
    triangles = newTs;
    construct();
}

void mesh::toFile(std::string filename)
{
    std::ofstream output(filename);
    if (!output.is_open())
        return;

    output << "# Point" << std::endl;
    for (auto vIter = vertices.begin(); vIter != vertices.end(); vIter++)
    {
        output << "v " + std::to_string(vIter->x) + " " + std::to_string(vIter->y) + " " + std::to_string(vIter->z) << std::endl;
    }

    output << std::endl << "# Face" << std::endl;
    for (auto tIter = triangles.begin(); tIter != triangles.end(); tIter++)
    {
        output << "f " + std::to_string((*tIter).v[0] + 1) + " " + std::to_string((*tIter).v[1] + 1) + " " + std::to_string((*tIter).v[2] + 1);
        output << std::endl;
    }

    output.close();
}