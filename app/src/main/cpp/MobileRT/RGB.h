//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RGB_H
#define MOBILERAYTRACER_RGB_H

namespace MobileRT {
    class RGB {
    private:

    public:
        float R, G, B;

        RGB ();
        RGB (const float r, const float g, const float b);
        RGB (const RGB& rgb);

        bool isZero() const;
        void add (const RGB& rgb);
        void mult (const RGB& rgb);
        void mult (const float f);
    };
}

#endif //MOBILERAYTRACER_RGB_H