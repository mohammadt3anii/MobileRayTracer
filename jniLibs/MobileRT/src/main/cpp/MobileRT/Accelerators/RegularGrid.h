//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_REGULARGRID_H
#define MOBILERAYTRACER_REGULARGRID_H

#include "../Primitive.h"
#include <vector>

namespace MobileRT {
    class RegularGrid {
    private:
        std::vector<Primitive *> *primitives_;// __attribute__((unused));

    public:
        explicit RegularGrid(std::vector<Primitive *> primitives);
        ~RegularGrid(void);
        void build(void);
        bool intersect(Intersection &intersection, Ray &ray);
    };
}

#endif //MOBILERAYTRACER_REGULARGRID_H
