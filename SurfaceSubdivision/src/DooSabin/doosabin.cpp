#include "doosabin.h"
#include <fstream>
#include <sstream>

using namespace DooSabin;

int edgeList::insert(edge e)
{
    int v1 = e.v[0], v2 = e.v[1];
    e.v[0] = v1 < v2 ? v1 : v2;
    e.v[1] = v1 + v2 - e.v[0];
    int index = 0;
    bool insertTag = false;
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        switch (edgeCmp(e, *iter))
        {
        case 0:
            iter->f[1] = e.f[0]; 
            insertTag = true;
            break;
        /*case 1:
            edges.insert(iter, e); 
            insertTag = true;
            break;*/
        default:
            index++;
        }
        if (insertTag) break;
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
                vertices.push_back(vertex{ atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str())});
            }
            else if (parts[0] == "f")
            {
                int pSize = parts.size();
                face _f;
                double aX = 0, aY = 0, aZ = 0;
                for (int i = 1; i < pSize; i++)
                {
                    std::vector<std::string> _v;
                    split(parts[i], _v, "/");
                    int vIndex = atoi(_v[0].c_str()) - 1;
                    aX += vertices[vIndex].x;
                    aY += vertices[vIndex].y;
                    aZ += vertices[vIndex].z;
                    vertices[vIndex].f.push_back(faces.size());
                    _f.v.push_back(vIndex);
                }
                for (int i = 0; i < pSize - 1; i++)
                {
                    edge _e;
                    _e.v[0] = _f.v[i];
                    _e.v[1] = _f.v[(i + 1) % (pSize - 1)];
                    _e.f[0] = faces.size();
                    int eIndex = edges.insert(_e);
                    vertices[_e.v[0]].e.insert(eIndex);
                    vertices[_e.v[1]].e.insert(eIndex);
                }
                _f.avg = vertex{ aX / (pSize - 1), aY / (pSize - 1), aZ / (pSize - 1) };
                faces.push_back(_f);
            }
        }
    }
}

void mesh::subdivide()
{
    std::vector<vertex> newV;
    std::vector<face> newF;
    edgeList newE;

    std::vector<std::vector<int>> vfMap;
    int oldFSize = faces.size();
    for (int i = 0; i < oldFSize; i++)
    {
        int vSize = faces[i].v.size();
        std::vector<int> vMap;
        face _f;
        for (int j = 0; j < vSize; j++)
        {
            vertex v = vertices[faces[i].v[j]],
                lastV = vertices[faces[i].v[(j - 1 + vSize) % vSize]],
                nextV = vertices[faces[i].v[(j + 1 + vSize) % vSize]],
                fAvgV = faces[i].avg;
            double newX = (v.x * 2 + lastV.x * 0.5 + nextV.x * 0.5 + fAvgV.x) / 4.0,
                newY = (v.y * 2 + lastV.y * 0.5 + nextV.y * 0.5 + fAvgV.y) / 4.0,
                newZ = (v.z * 2 + lastV.z * 0.5 + nextV.z * 0.5 + fAvgV.z) / 4.0;
            vMap.push_back(newV.size());
            vertex _v;
            _v.x = newX;
            _v.y = newY;
            _v.z = newZ;
            _v.f.push_back(newF.size());
            newV.push_back(_v);

        }
        _f.v = vMap;
        vfMap.push_back(vMap);
        newF.push_back(_f);
    }

    // 边
    int oldESize = edges.edges.size();
    for (int i = 0; i < oldESize; i++)
    {
        int f0Index = edges.edges[i].f[0],
            f1Index = edges.edges[i].f[1];
        face f0 = faces[f0Index],
            f1 = faces[f1Index];
        int f0VSize = f0.v.size(), f1VSize = f1.v.size();
        int v0 = edges.edges[i].v[0],
            v1 = edges.edges[i].v[1];
        int index = 0;
        face _f;

        // v0在f0中的新顶点
        while (f0.v[index] != v0) index++;
        _f.v.push_back(vfMap[f0Index][index]);

        // v1在f0中的新顶点
        int _lastVIndex = (index - 1 + f0VSize) % f0VSize,
            _nextVIndex = (index + 1 + f0VSize) % f0VSize;
        if (f0.v[_lastVIndex] == v1)
            _f.v.push_back(vfMap[f0Index][_lastVIndex]);
        else
            _f.v.push_back(vfMap[f0Index][_nextVIndex]);

        // v1在f1中的新顶点
        index = 0;
        while (f1.v[index] != v1) index++;
        _f.v.push_back(vfMap[f1Index][index]);

        // v0在f1中的新顶点
        _lastVIndex = (index - 1 + f1VSize) % f1VSize;
        _nextVIndex = (index + 1 + f1VSize) % f1VSize;
        if (f1.v[_lastVIndex] == v0)
            _f.v.push_back(vfMap[f1Index][_lastVIndex]);
        else
            _f.v.push_back(vfMap[f1Index][_nextVIndex]);

        newF.push_back(_f);
    }

    // 点
    int oldVSize = vertices.size();
    for (int i = 0; i < oldVSize; i++)
    {
        int startF = vertices[i].f[0],
            fIndex = startF,
            fSize = vertices[i].f.size();
        face _f;
        std::set<int> visitedF;
        for (int j = 0; j < fSize && fIndex >= 0; j++)
        {
            int index = 0;
            while (faces[fIndex].v[index] != i) index++;
            _f.v.push_back(vfMap[fIndex][index]);
            visitedF.insert(fIndex);
            bool findNext = false;
            for (auto eIter = vertices[i].e.begin(); eIter != vertices[i].e.end(); eIter++)
            {
                int eIndex = *eIter, nextFIndex = -1;
                if (edges.edges[eIndex].f[0] == fIndex)
                    nextFIndex = edges.edges[eIndex].f[1];
                else if (edges.edges[eIndex].f[1] == fIndex)
                    nextFIndex = edges.edges[eIndex].f[0];
                if (nextFIndex != -1 && visitedF.find(nextFIndex) == visitedF.end())
                {
                    fIndex = nextFIndex;
                    findNext = true;
                    break;
                }
            }
            if (!findNext) fIndex = -1;
        }
        newF.push_back(_f);
    }

    vertices = newV;
    faces = newF;
    construct();
}

void mesh::construct()
{
    edges.edges.clear();

    int fSize = faces.size();
    for (int i = 0; i < fSize; i++)
    {
        face* _f = &faces[i];
        double aX = 0, aY = 0, aZ = 0;
        int vSize = faces[i].v.size();
        for (int j = 0; j < vSize; j++)
        {
            int vIndex = faces[i].v[j];
            aX += vertices[vIndex].x;
            aY += vertices[vIndex].y;
            aZ += vertices[vIndex].z;
            vertices[vIndex].f.push_back(i);

            edge _e;
            _e.v[0] = _f->v[j];
            _e.v[1] = _f->v[(j + 1) % vSize];
            _e.f[0] = i;
            int eIndex = edges.insert(_e);
            vertices[_e.v[0]].e.insert(eIndex);
            vertices[_e.v[1]].e.insert(eIndex);
        }
        _f->avg = vertex{ aX / vSize, aY / vSize, aZ / vSize };
    }
}

std::vector<double> mesh::getVertices()
{
    int vSize = vertices.size();
    std::vector<double> vList;
    for (int i = 0; i < vSize; i++)
    {
        vList.push_back(vertices[i].x);
        vList.push_back(vertices[i].y);
        vList.push_back(vertices[i].z);
    }
    return vList;
}

std::vector<std::vector<int>> mesh::getFaceList()
{
    std::vector<std::vector<int>> faceList;
    auto fIter = faces.begin();
    for (; fIter != faces.end(); fIter++)
    {
        std::vector<int> vList;
        auto vIter = (*fIter).v.begin();
        for (; vIter != (*fIter).v.end(); vIter++)
        {
            vList.push_back((*vIter));
        }
        faceList.push_back(vList);
    }
    return faceList;
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
    for (auto fIter = faces.begin(); fIter != faces.end(); fIter++)
    {
        output << "f";
        for (auto vIter = (*fIter).v.begin(); vIter != (*fIter).v.end(); vIter++)
            output << " " + std::to_string((*vIter) + 1);
        output << std::endl;
    }

    output.close();
}

