//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINT3D_H
#define MOBILERAYTRACER_POINT3D_H

namespace MobileRT
{
    class Vector3D;
    
    class Point3D
    {
        private:


    public:
            float x_;
            float y_;
            float z_;

            Point3D();
            Point3D(const float x, const float y, const float z);
            Point3D(const Point3D& point);

            float sumCoordenates() const;

            Point3D operator+(const Vector3D& vector) const;
            Vector3D operator-(const Point3D& point) const;
    };
}

#endif //MOBILERAYTRACER_POINT3D_H