//
// Created by Tiago on 16-10-2016.
//
#include "RGB.h"

using namespace MobileRT;

static unsigned int counter(0u);

RGB::RGB(void) :
        R_(0.0f),
        G_(0.0f),
        B_(0.0f),
        samples_(0u) {
    counter++;
}

RGB::RGB(const float r, const float g, const float b) :
        R_(r),
        G_(g),
        B_(b) {
    counter++;
}

RGB::RGB(const RGB &rgb) :
        R_(rgb.R_),
        G_(rgb.G_),
        B_(rgb.B_) {
    counter++;
}

bool RGB::isNotZero(void) const {
    return ((this->R_ > 0.0f) || (this->G_ > 0.0f) || (this->B_ > 0.0f));
}

void RGB::add(const RGB &rgb) {
    this->R_ += rgb.R_;
    this->G_ += rgb.G_;
    this->B_ += rgb.B_;
}

void RGB::addMult(const RGB &rgb1, const RGB &rgb2, const float value) {
    this->R_ += rgb1.R_ * rgb2.R_ * value;
    this->G_ += rgb1.G_ * rgb2.G_ * value;
    this->B_ += rgb1.B_ * rgb2.B_ * value;
}

void RGB::addMult(const RGB &rgb1, const RGB &rgb2) {
    this->R_ += rgb1.R_ * rgb2.R_;
    this->G_ += rgb1.G_ * rgb2.G_;
    this->B_ += rgb1.B_ * rgb2.B_;
}

void RGB::addMult(const RGB &rgb, const float value) {
    this->R_ += rgb.R_ * value;
    this->G_ += rgb.G_ * value;
    this->B_ += rgb.B_ * value;
}

void RGB::operator*=(const RGB &rgb) {
    this->R_ *= rgb.R_;
    this->G_ *= rgb.G_;
    this->B_ *= rgb.B_;
}

const RGB RGB::operator*(const float value) const
{
    return RGB(this->R_ * value, this->G_ * value, this->B_ * value);
}

void RGB::operator*=(const float value) {
    this->R_ *= value;
    this->G_ *= value;
    this->B_ *= value;
}

void RGB::operator/=(const float value) {
    this->R_ /= value;
    this->G_ /= value;
    this->B_ /= value;
}

void RGB::addSampleAndCalcAvg(RGB &sample) {
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

void RGB::reset(void) {
    this->R_ = 0.0f;
    this->G_ = 0.0f;
    this->B_ = 0.0f;
    this->samples_ = 0u;
}

void RGB::reset(const float color) {
    this->R_ = color;
    this->G_ = color;
    this->B_ = color;
    this->samples_ = 0u;
}

void RGB::reset(const float r, const float g, const float b) {
    this->R_ = r;
    this->G_ = g;
    this->B_ = b;
    this->samples_ = 0u;
}

unsigned int RGB::RGB2Color(void) {
    unsigned int red(static_cast<unsigned int> (this->R_ * 255u));
    const unsigned int maskR(0u - (red > 255u));
    red = (255u & maskR) | (red & ~maskR);

    unsigned int green(static_cast<unsigned int> (this->G_ * 255u));
    const unsigned int maskG(0u - (green > 255u));
    green = (255u & maskG) | (green & ~maskG);


    unsigned int blue(static_cast<unsigned int> (this->B_ * 255u));
    const unsigned int maskB(0u - (blue > 255u));
    blue = (255u & maskB) | (blue & ~maskB);

    return (0xFF000000 | (blue << 16) | (green << 8) | red);
}

unsigned int RGB::getInstances() {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}

float RGB::getMax(void) {
    if (this->R_ >= this->G_ && this->R_ >= this->B_) return this->R_;
    if (this->G_ >= this->R_ && this->G_ >= this->B_) return this->G_;
    return this->B_;
}
