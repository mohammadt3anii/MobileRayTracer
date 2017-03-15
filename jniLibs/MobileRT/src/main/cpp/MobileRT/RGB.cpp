//
// Created by Tiago on 16-10-2016.
//
#include "RGB.h"

using namespace MobileRT;

static unsigned int counter = 0;

RGB::RGB(void) :
        R_(0.0f),
        G_(0.0f),
        B_(0.0f),
        samples_(0) {
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

void RGB::add(const RGB &rgb1, const RGB &rgb2, const float value) {
    this->R_ += rgb1.R_ * rgb2.R_ * value;
    this->G_ += rgb1.G_ * rgb2.G_ * value;
    this->B_ += rgb1.B_ * rgb2.B_ * value;
}

void RGB::add(const RGB &rgb, const float value) {
    this->R_ += rgb.R_ * value;
    this->G_ += rgb.G_ * value;
    this->B_ += rgb.B_ * value;
}

void RGB::operator*=(const RGB &rgb) {
    this->R_ *= rgb.R_;
    this->G_ *= rgb.G_;
    this->B_ *= rgb.B_;
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
    this->samples_ = 0;
}

void RGB::reset(const float color) {
    this->R_ = color;
    this->G_ = color;
    this->B_ = color;
}

void RGB::reset(const float r, const float g, const float b) {
    this->R_ = r;
    this->G_ = g;
    this->B_ = b;
}

unsigned int RGB::RGB2Color(void) {
    unsigned int cr(static_cast<unsigned int> (this->R_ * 255u));
    const unsigned int maskR(0u - (cr > 255u));
    cr = (255u & maskR) | (cr & ~maskR);
    //unsigned int hiR (255u - cr);
    //cr = cr + (hiR & (hiR <= 255u) - 1);

    unsigned int cg(static_cast<unsigned int> (this->G_ * 255u));
    const unsigned int maskG(0u - (cg > 255u));
    cg = (255u & maskG) | (cg & ~maskG);
    //unsigned int hiG (255u - cg);
    //cg = cg + (hiG & (hiG <= 255u) - 1);

    unsigned int cb(static_cast<unsigned int> (this->B_ * 255u));
    const unsigned int maskB(0u - (cb > 255u));
    cb = (255u & maskB) | (cb & ~maskB);
    //unsigned int hiB (255u - cb);
    //cb = cb + (hiB & (hiB <= 255u) - 1);

    return (0xFF000000 | (cb << 16) | (cg << 8) | cr);
}

unsigned int RGB::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}
