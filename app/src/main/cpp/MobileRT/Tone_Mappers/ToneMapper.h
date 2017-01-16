//
// Created by Tiago on 16/01/2017.
//

#ifndef MOBILERAYTRACER_TONEMAPPER_H
#define MOBILERAYTRACER_TONEMAPPER_H

namespace MobileRT {
    class ToneMapper {
    private:

    public:
        ToneMapper() = default;

        virtual unsigned int RGB2Color() = 0;
    };
}

#endif //MOBILERAYTRACER_TONEMAPPER_H