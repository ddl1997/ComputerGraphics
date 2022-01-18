#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include "CatmullClark/catmullclark.h"
#include "DooSabin/doosabin.h"
#include "Loop/loop.h"

class subdivision {
public:
    subdivision()
    {
        catmull_clark = nullptr;
        doo_sabin = nullptr;
        loop = nullptr;
    }

    void importMesh(std::string filename, int type); // ��������

    bool subdivide(int iterCount, int type); // ϸ������

    bool exportFile(std::string filename, int type); // �����ļ�

private:
    CatmullClark::mesh* catmull_clark;
    DooSabin::mesh* doo_sabin;
    Loop::mesh* loop;
};
#endif // !SUBDIVISION_H
