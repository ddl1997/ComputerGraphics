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

    void importMesh(std::string filename, int type); // 导入网格

    bool subdivide(int iterCount, int type); // 细分曲面

    bool exportFile(std::string filename, int type); // 导出文件

private:
    CatmullClark::mesh* catmull_clark;
    DooSabin::mesh* doo_sabin;
    Loop::mesh* loop;
};
#endif // !SUBDIVISION_H
