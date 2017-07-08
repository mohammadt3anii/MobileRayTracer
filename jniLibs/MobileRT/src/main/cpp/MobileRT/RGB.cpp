//
// Created by Tiago on 16-10-2016.
//
#include "RGB.h"

using namespace MobileRT;

static unsigned int counter(0u);

RGB::RGB(void) noexcept :
        samples_(0u),
        R_(0.0f),
        G_(0.0f),
        B_(0.0f) {
    counter++;
}

RGB::RGB(const float r, const float g, const float b) noexcept :
        R_(r),
        G_(g),
        B_(b) {
    counter++;
}

RGB::RGB(const RGB &rgb) noexcept :
        R_(rgb.R_),
        G_(rgb.G_),
        B_(rgb.B_) {
    counter++;
}

bool RGB::isNotZero(void) const noexcept {
    return ((this->R_ > 0.0f) || (this->G_ > 0.0f) || (this->B_ > 0.0f));
}

void RGB::add(const RGB &rgb) noexcept {
    this->R_ += rgb.R_;
    this->G_ += rgb.G_;
    this->B_ += rgb.B_;
}

void RGB::addMult(const RGB &rgb1, const RGB &rgb2, const float value) noexcept {
    this->R_ += rgb1.R_ * rgb2.R_ * value;
    this->G_ += rgb1.G_ * rgb2.G_ * value;
    this->B_ += rgb1.B_ * rgb2.B_ * value;
}

void RGB::addMult(const RGB &rgb1, const RGB &rgb2) noexcept {
    this->R_ += rgb1.R_ * rgb2.R_;
    this->G_ += rgb1.G_ * rgb2.G_;
    this->B_ += rgb1.B_ * rgb2.B_;
}

void RGB::addMult(const RGB &rgb, const float value) noexcept {
    this->R_ += rgb.R_ * value;
    this->G_ += rgb.G_ * value;
    this->B_ += rgb.B_ * value;
}

void RGB::operator*=(const RGB &rgb) noexcept {
    this->R_ *= rgb.R_;
    this->G_ *= rgb.G_;
    this->B_ *= rgb.B_;
}

const RGB RGB::operator*(const float value) const noexcept {
    return RGB(this->R_ * value, this->G_ * value, this->B_ * value);
}

void RGB::operator*=(const float value) noexcept {
    this->R_ *= value;
    this->G_ *= value;
    this->B_ *= value;
}

void RGB::operator/=(const float value) noexcept {
    this->R_ /= value;
    this->G_ /= value;
    this->B_ /= value;
}

void RGB::addSampleAndCalcAvg(RGB &sample) noexcept {
    this->mutex_.lock();
    this->R_ += sample.R_;
    this->G_ += sample.G_;
    this->B_ += sample.B_;
    sample.R_ = this->R_;
    sample.G_ = this->G_;
    sample.B_ = this->B_;
    sample.samples_ = ++this->samples_;
    this->mutex_.unlock();
    sample.R_ /= sample.samples_;
    sample.G_ /= sample.samples_;
    sample.B_ /= sample.samples_;
}

void RGB::reset(void) noexcept {
    this->R_ = 0.0f;
    this->G_ = 0.0f;
    this->B_ = 0.0f;
    this->samples_ = 0u;
}

unsigned int RGB::RGB2Color(void) noexcept {
    const unsigned int red(static_cast<unsigned int> (this->R_ * 255u));
    const unsigned int maskR(0u - (red > 255u));
    const unsigned int r((255u & maskR) | (red & ~maskR));

    const unsigned int green(static_cast<unsigned int> (this->G_ * 255u));
    const unsigned int maskG(0u - (green > 255u));
    const unsigned int g((255u & maskG) | (green & ~maskG));

    const unsigned int blue(static_cast<unsigned int> (this->B_ * 255u));
    const unsigned int maskB(0u - (blue > 255u));
    const unsigned int b((255u & maskB) | (blue & ~maskB));

    return (0xFF000000u | (b << 16u) | (g << 8u) | r);
}

unsigned int RGB::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}

float RGB::getMax(void) noexcept {
    return this->R_ >= this->G_ && this->R_ >= this->B_ ? this->R_ :
           this->G_ >= this->R_ && this->G_ >= this->B_ ? this->G_ :
           this->B_;
}
