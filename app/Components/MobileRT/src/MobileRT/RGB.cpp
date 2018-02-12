//
// Created by Tiago on 16-10-2016.
//
#include "RGB.hpp"

using ::MobileRT::RGB;
static unsigned counter{0};

RGB::RGB(const float r, const float g, const float b) noexcept :
        R_{r},
        G_{g},
        B_{b} {
    counter++;
}

RGB::RGB(const RGB &rgb) noexcept :
        R_{rgb.R_},
        G_{rgb.G_},
        B_{rgb.B_},
        samples_{rgb.samples_} {
    counter++;
}

RGB::RGB(RGB &&rgb) noexcept :
        R_{rgb.R_},
        G_{rgb.G_},
        B_{rgb.B_},
        samples_{rgb.samples_} {
    counter++;
}

bool RGB::hasColor() const noexcept {
    return ((this->R_ > 0.0f) || (this->G_ > 0.0f) || (this->B_ > 0.0f));
}

void RGB::operator+=(const RGB rgb) noexcept {
    this->R_ += rgb.R_;
    this->G_ += rgb.G_;
    this->B_ += rgb.B_;
}

void RGB::addMult(const ::std::initializer_list<const RGB> rgbs, const float value) noexcept {
    float red{value};
    float green{value};
    float blue{value};
    for (auto &elem : rgbs) {
        red *= elem.R_;
        green *= elem.G_;
        blue *= elem.B_;
    }
    this->R_ += red;
    this->G_ += green;
    this->B_ += blue;
}

void RGB::operator*=(const RGB rgb) noexcept {
    this->R_ *= rgb.R_;
    this->G_ *= rgb.G_;
    this->B_ *= rgb.B_;
}

RGB RGB::operator*(const float value) const noexcept {
    return RGB(this->R_ * value, this->G_ * value, this->B_ * value);
}

void RGB::operator*=(const float value) noexcept {
    this->R_ *= value;
    this->G_ *= value;
    this->B_ *= value;
}

void RGB::operator+=(const float value) noexcept {
    this->R_ += value;
    this->G_ += value;
    this->B_ += value;
}

void RGB::operator/=(const float value) noexcept {
    this->R_ /= value;
    this->G_ /= value;
    this->B_ /= value;
}

void RGB::addSampleAndCalcAvg(RGB *const sample) noexcept {
    this->mutex_.lock();
    this->R_ += sample->R_;
    this->G_ += sample->G_;
    this->B_ += sample->B_;
    sample->samples_ = ++this->samples_;
    sample->R_ = this->R_;
    sample->G_ = this->G_;
    sample->B_ = this->B_;
    this->mutex_.unlock();
    sample->R_ /= sample->samples_;
    sample->G_ /= sample->samples_;
    sample->B_ /= sample->samples_;
}

void RGB::reset(const float r, const float g, const float b, const unsigned samples) noexcept {
    this->R_ = r;
    this->G_ = g;
    this->B_ = b;
    this->samples_ = samples;
}

unsigned RGB::getColor() const noexcept {
    unsigned r{this->R_ >= 1.0f ? 255u : static_cast<unsigned> (this->R_ * 255u)};
    unsigned g{this->G_ >= 1.0f ? 255u : static_cast<unsigned> (this->G_ * 255u)};
    unsigned b{this->B_ >= 1.0f ? 255u : static_cast<unsigned> (this->B_ * 255u)};
    return ((r * 1000000) + (g * 1000) + b);
}

void RGB::toneMap() noexcept {
    const float value{1.0f};
    this->R_ = (this->R_ / (this->R_ + value));
    this->G_ = (this->G_ / (this->G_ + value));
    this->B_ = (this->B_ / (this->B_ + value));
}

unsigned RGB::RGB2Color() noexcept {
    //toneMap();
    unsigned r{this->R_ >= 1.0f ? 255u : static_cast<unsigned> (this->R_ * 255u)};
    unsigned g{this->G_ >= 1.0f ? 255u : static_cast<unsigned> (this->G_ * 255u)};
    unsigned b{this->B_ >= 1.0f ? 255u : static_cast<unsigned> (this->B_ * 255u)};
    return (0xFF000000u | (b << 16u) | (g << 8u) | r);
}

unsigned RGB::getInstances() noexcept {
    unsigned res{counter};
    counter = 0;
    return res;
}

float RGB::getMax() const noexcept {
    return this->R_ >= this->G_ && this->R_ >= this->B_ ? this->R_ / samples_ :
           this->G_ >= this->R_ && this->G_ >= this->B_ ? this->G_ / samples_ :
           this->B_ / samples_;
}

RGB &RGB::operator=(const RGB &rgb) noexcept {
    this->R_ = rgb.R_;
    this->G_ = rgb.G_;
    this->B_ = rgb.B_;
    this->samples_ = rgb.samples_;
    return *this;
}

RGB &RGB::operator=(RGB &&rgb) noexcept {
    this->R_ = rgb.R_;
    this->G_ = rgb.G_;
    this->B_ = rgb.B_;
    this->samples_ = rgb.samples_;
    return *this;
}

//newAvg = ((size - 1) * oldAvg + newNum) / size;
unsigned
RGB::incrementalAvg(const RGB &sample, const unsigned avg, const unsigned numSample) noexcept {
    //toneMap();

    const unsigned redB4{avg & 0xFF};
    const unsigned greenB4{(avg >> 8) & 0xFF};
    const unsigned blueB4{(avg >> 16) & 0xFF};

    const unsigned redSamp{static_cast<unsigned> (sample.R_ * 255u)};
    const unsigned greenSamp{static_cast<unsigned> (sample.G_ * 255u)};
    const unsigned blueSamp{static_cast<unsigned> (sample.B_ * 255u)};

    const unsigned redAft{((numSample - 1) * redB4 + redSamp) / numSample};
    const unsigned greenAft{((numSample - 1) * greenB4 + greenSamp) / numSample};
    const unsigned blueAft{((numSample - 1) * blueB4 + blueSamp) / numSample};

    unsigned retR{redAft > 255u ? 255u : redAft};
    unsigned retG{greenAft > 255u ? 255u : greenAft};
    unsigned retB{blueAft > 255u ? 255u : blueAft};

    return (0xFF000000u | (retB << 16u) | (retG << 8u) | retR);
}
