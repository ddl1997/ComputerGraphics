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
            int partSize = parts.size() - 1;
            if (parts[0] == "v")
            {
                vertices.push_back(vertex{ atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str()) });
            }
            else if (parts[0] == "f")
            {
                quadrangle _q;
                for (int i = 0; i < partSize; i++)
                {
                    std::vector<std::string> _v;
                    split(parts[i + 1], _v, "/");
                    int vIndex = atoi(_v[0].c_str()) - 1;
                    _q.v.push_back(vIndex);
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

    int fSize = faces.size();
    for (int i = 0; i < fSize; i++)
    {
        auto _f = &faces[i];
        int vSize = _f->v.size();
        for (int j = 0; j < vSize; j++)
        {
            int cur = _f->v[j], next = _f->v[(j + 1) % vSize];
            vertices[cur].f.push_back(i);
            edge adjE;
            adjE.v[0] = cur;
            adjE.v[1] = next;
            adjE.f[0] = i;
            int eIndex = edges.insert(adjE);
            _f->e.push_back(eIndex);
            vertices[cur].e.insert(eIndex);
            vertices[next].e.insert(eIndex);
        }
    }
}

void mesh::subdivide()
{
    std::vector<vertex> newVs;
    std::vector<quadrangle> newFs;

    // �����������
    for (auto fIter = faces.begin(); fIter != faces.end(); fIter++)
    {
        double newX = 0.0, newY = 0.0, newZ = 0.0;
        int vSize = fIter->v.size();
        for (int i = 0; i < vSize; i++)
        {
            newX += vertices[fIter->v[i]].x;
            newY += vertices[fIter->v[i]].y;
            newZ += vertices[fIter->v[i]].z;
        }
        fIter->avgV = newVs.size();
        newVs.push_back(vertex(newX / 4.0, newY / 4.0, newZ / 4.0));
    }

    // ���������ߵ�
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

    // ����ԭʼ����
    int vOffset = newVs.size();
    for (auto vIter = vertices.begin(); vIter != vertices.end(); vIter++)
    {
        double avgFX = 0.0, avgFY = 0.0, avgFZ = 0.0,
            avgEX = 0.0, avgEY = 0.0, avgEZ = 0.0;

        // ����ƽ�������е�
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

        // ����ƽ���ڱ��е�
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

    // ���Ӷ�����������
    for (auto fIter = faces.begin(); fIter != faces.end(); fIter++)
    {
        int vSize = fIter->v.size();
        for (int i = 0; i < vSize; i++)
        {
            quadrangle newF;
            newF.v.push_back(fIter->v[i] + vOffset);
            newF.v.push_back(edges.edges[fIter->e[i]].newV);
            newF.v.push_back(fIter->avgV);
            newF.v.push_back(edges.edges[fIter->e[(i + vSize - 1) % vSize]].newV);
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