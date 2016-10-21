//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYPOINT_H
#define MOBILERAYTRACER_MYPOINT_H

namespace MobileRT {
    class myPoint {
    private:

    public:
        float x, y, z;

        myPoint();
        myPoint(const float px, const float py, const float pz);
        myPoint(const myPoint &p);
    };
}

#endif //MOBILERAYTRACER_MYPOINT_H