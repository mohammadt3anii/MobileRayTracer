//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYPOINT_H
#define MOBILERAYTRACER_MYPOINT_H

namespace MobileRT {
    class Vect;
    class Point {
    protected:
        float x_, y_, z_;

    public:
        Point();
        Point(const float& x, const float& y, const float& z);
        Point(const Point& point);

        Point square() const;
        Point multiply(const Vect& vector) const;
        float sumCoordenates() const;

        Point operator+(const Vect& vector) const;
        Vect operator-(const Point& point) const;
        Point operator*(const float& scale) const;
    };
}

#endif //MOBILERAYTRACER_MYPOINT_H