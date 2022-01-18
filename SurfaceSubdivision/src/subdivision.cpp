#include "subdivision.h"

void subdivision::importMesh(std::string filename, int type)
{
    switch (type)
    {
    case 0:
        if (catmull_clark != nullptr)
            delete catmull_clark;
        catmull_clark = new CatmullClark::mesh(filename); break;
    case 1:
        if (doo_sabin != nullptr)
            delete doo_sabin;
        doo_sabin = new DooSabin::mesh(filename); break;
    case 2:
        if (loop != nullptr)
            delete loop;
        loop = new Loop::mesh(filename); break;
    }
}

bool subdivision::subdivide(int iterCount, int type)
{
    switch (type)
    {
    case 0:
        if (catmull_clark == nullptr)
            return false;
        for (int i = 0; i < iterCount; i++)
        {
            catmull_clark->subdivide();
            printf("Iteration %d finished!\n", i + 1);
        }
        break;
    case 1:
        if (doo_sabin == nullptr)
            return false;
        for (int i = 0; i < iterCount; i++)
        {
            doo_sabin->subdivide();
            printf("Iteration %d finished!\n", i + 1);
        }
        break;
    case 2:
        if (loop == nullptr)
            return false;
        for (int i = 0; i < iterCount; i++)
        {
            loop->subdivide();
            printf("Iteration %d finished!\n", i + 1);
        }
        break;
    }
    return true;
}

bool subdivision::exportFile(std::string filename, int type)
{
    switch (type)
    {
    case 0:
        if (catmull_clark == nullptr)
            return false;
        catmull_clark->toFile(filename);
        break;
    case 1:
        if (doo_sabin == nullptr)
            return false;
        doo_sabin->toFile(filename);
        break;
    case 2:
        if (loop == nullptr)
            return false;
        loop->toFile(filename);
        break;
    }
}