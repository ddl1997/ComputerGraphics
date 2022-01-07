#include "mesh.h"
#include <fstream>
#include <sstream>

using namespace DooSabin;

int edgeCmp(edge a, edge b)
{
    if (a.v[0] == b.v[0] && a.v[1] == b.v[1])
        return 0;
    else if (a.v[0] < b.v[0] || (a.v[0] == b.v[0] && a.v[1] < b.v[1]))
        return 1;
    else
        return -1;
}

void edgeList::insert(edge e)
{
    int v1 = e.v[0], v2 = e.v[1];
    e.v[0] = v1 < v2 ? v1 : v2;
    e.v[1] = v1 + v2 - e.v[0];
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        switch (edgeCmp(e, *iter))
        {
        case 0:
            iter->f[1] = e.f[0];
        case 1:
            edges.insert(iter, e);
        default:
            continue;
        }
    }
}

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
                for (int i = 1; i < pSize; i++)
                {
                    std::vector<std::string> _v;
                    split(parts[i], _v, "/");
                    int vIndex = atoi(_v[0].c_str()) - 1;
                    vertices[vIndex].f.push_back(faces.size());
                    _f.v.push_back(vIndex);
                }
                for (int i = 0; i < pSize - 1; i++)
                {
                    edge _e;
                    _e.v[0] = _f.v[i];
                    _e.v[1] = _f.v[(i + 1) % (pSize - 1)];
                    _e.f[0] = faces.size();
                    edges.insert(_e);
                }
                faces.push_back(_f);
            }
        }
    }
}


