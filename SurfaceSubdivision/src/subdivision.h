#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include "CatmullClark/catmullclark.h"
#include "DooSabin/doosabin.h"
#include "Loop/loop.h"

enum SubdivideType { CatmullClark, DooSabin, Loop };

class subdivision {
public:
    void static subdivide(std::string filename, SubdivideType type)
    {
        switch (type)
        {
        case CatmullClark:

        }
    }
};
#endif // !SUBDIVISION_H
