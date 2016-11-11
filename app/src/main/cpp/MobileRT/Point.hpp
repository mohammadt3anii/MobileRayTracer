//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYPOINT_HPP
#define MOBILERAYTRACER_MYPOINT_HPP

namespace MobileRT
{
    class Vect;
    
    class Point
    {
        private:
            float x_;
            float y_;
            float z_;

        public:
            Point();
            Point(const float x, const float y, const float z);
            Point(const Point& point);

            Point square() const;
            Point multiply(const Vect& vector) const;
            float sumCoordenates() const;

            Point operator+(const Vect& vector) const;
            Vect operator-(const Point& point) const;
            Point operator*(const float scale) const;
    };
}

#endif //MOBILERAYTRACER_MYPOINT_HPP