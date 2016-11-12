//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYPOINT_H
#define MOBILERAYTRACER_MYPOINT_H

namespace MobileRT
{
    class Vect;
    
    class Point
    {
        private:


    public:
            float x_;
            float y_;
            float z_;

            Point();
            Point(const float x, const float y, const float z);
            Point(const Point& point);

            Point multiply(const Vect& vector) const;
            float sumCoordenates() const;

            Point operator+(const Vect& vector) const;
            Vect operator-(const Point& point) const;
    };
}

#endif //MOBILERAYTRACER_MYPOINT_H