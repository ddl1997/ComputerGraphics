#include "catmullclark.h"
#include <fstream>

using namespace CatmullClark;

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
                quadrangle _q;
                for (int i = 0; i < 4; i++)
                {
                    std::vector<std::string> _v;
                    split(parts[i + 1], _v, "/");
                    int vIndex = atoi(_v[0].c_str()) - 1;
                    _q.v[i] = vIndex;
                }
                faces.push_back(_q);
            }
        }
    }

    construct();
}

void mesh::construct()
{
    edges.clear();

    int tSize = faces.size();
    for (int i = 0; i < tSize; i++)
    {
        auto _t = &faces[i];
        for (int j = 0; j < 4; j++)
        {
            int cur = _t->v[j], next = _t->v[(j + 1) % 4], opposite = _t->v[(j + 2) % 4];
            vertices[cur].f.push_back(i);
            edge adjE;
            adjE.v[0] = cur;
            adjE.v[1] = next;
            adjE.f[0] = i;
            int eIndex = edges.insert(adjE);
            _t->e[j] = eIndex;
            vertices[cur].e.insert(eIndex);
            vertices[next].e.insert(eIndex);
        }
    }
}

void mesh::subdivide()
{
    std::vector<vertex> newVs;
    std::vector<quadrangle> newFs;

    // 计算新增面点
    for (auto fIter = faces.begin(); fIter != faces.end(); fIter++)
    {
        double newX = (vertices[fIter->v[0]].x + vertices[fIter->v[1]].x + vertices[fIter->v[2]].x + vertices[fIter->v[3]].x) / 4.0,
            newY = (vertices[fIter->v[0]].y + vertices[fIter->v[1]].y + vertices[fIter->v[2]].y + vertices[fIter->v[3]].y) / 4.0,
            newZ = (vertices[fIter->v[0]].z + vertices[fIter->v[1]].z + vertices[fIter->v[2]].z + vertices[fIter->v[3]].z) / 4.0;
        fIter->avgV = newVs.size();
        newVs.push_back(vertex(newX, newY, newZ));
    }

    // 计算新增边点
    auto edgesP = &edges.edges;
    for (auto eIter = edgesP->begin(); eIter != edgesP->end(); eIter++)
    {
        double newX = vertices[eIter->v[0]].x + vertices[eIter->v[1]].x, 
            newY = vertices[eIter->v[0]].y + vertices[eIter->v[1]].y, 
            newZ = vertices[eIter->v[0]].z + vertices[eIter->v[1]].z;
        if (eIter->f[0] >= 0 && eIter->f[1] >= 0)
        {
            newX = (newX + newVs[faces[eIter->f[0]].avgV].x + newVs[faces[eIter->f[1]].avgV].x) / 4.0;
            newY = (newY + newVs[faces[eIter->f[0]].avgV].y + newVs[faces[eIter->f[1]].avgV].y) / 4.0;
            newZ = (newZ + newVs[faces[eIter->f[0]].avgV].z + newVs[faces[eIter->f[1]].avgV].z) / 4.0;
        }
        else
        {
            newX /= 2.0;
            newY /= 2.0;
            newZ /= 2.0;
        }
        eIter->newV = newVs.size();
        newVs.push_back(vertex(newX, newY, newZ));
    }

    // 调整原始顶点
    int vOffset = newVs.size();
    for (auto vIter = vertices.begin(); vIter != vertices.end(); vIter++)
    {
        double avgFX = 0.0, avgFY = 0.0, avgFZ = 0.0,
            avgEX = 0.0, avgEY = 0.0, avgEZ = 0.0;

        // 计算平均邻面中点
        int fSize = vIter->f.size();
        for (auto fIter = vIter->f.begin(); fIter != vIter->f.end(); fIter++)
        {
            auto avgFV = newVs[faces[*fIter].avgV];
            avgFX += avgFV.x;
            avgFY += avgFV.y;
            avgFZ += avgFV.z;
        }
        avgFX /= fSize;
        avgFY /= fSize;
        avgFZ /= fSize;

        // 计算平均邻边中点
        int eSize = vIter->e.size();
        for (auto eIter = vIter->e.begin(); eIter != vIter->e.end(); eIter++)
        {
            auto v0 = vertices[edges.edges[*eIter].v[0]], v1 = vertices[edges.edges[*eIter].v[1]];
            avgEX += v0.x + v1.x;
            avgEY += v0.y + v1.y;
            avgEZ += v0.z + v1.z;
        }
        avgEX /= eSize * 2.0;
        avgEY /= eSize * 2.0;
        avgEZ /= eSize * 2.0;

        double fRatio = alpha / fSize, eRatio = beta / fSize, vRatio = 1 - fRatio - eRatio;
        double newX = vRatio * vIter->x + fRatio * avgFX + eRatio * avgEX,
            newY = vRatio * vIter->y + fRatio * avgFY + eRatio * avgEY,
            newZ = vRatio * vIter->z + fRatio * avgFZ + eRatio * avgEZ;
        newVs.push_back(vertex{ newX, newY, newZ });
    }

    // 连接顶点生成新面
    for (auto fIter = faces.begin(); fIter != faces.end(); fIter++)
    {
        for (int i = 0; i < 4; i++)
        {
            quadrangle newF;
            newF.v[0] = fIter->v[i] + vOffset;
            newF.v[1] = edges.edges[fIter->e[i]].newV;
            newF.v[2] = fIter->avgV;
            newF.v[3] = edges.edges[fIter->e[(i + 3) % 4]].newV;
            newFs.push_back(newF);
        }
    }

    vertices = newVs;
    faces = newFs;
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
    for (auto fIter = faces.begin(); fIter != faces.end(); fIter++)
    {
        output << "f " + std::to_string((*fIter).v[0] + 1) + " " + std::to_string((*fIter).v[1] + 1) + " " + std::to_string((*fIter).v[2] + 1) + " " + std::to_string((*fIter).v[3] + 1);
        output << std::endl;
    }

    output.close();
}