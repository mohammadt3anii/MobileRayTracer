//
// Created by Tiago on 16-10-2016.
//
#include "RGB.h"

using namespace MobileRT;

RGB::RGB() :
        R_(0.0f),
        G_(0.0f),
        B_(0.0f),
        samples_(0) {
}

RGB::RGB(const float r, const float g, const float b) :
        R_(r),
        G_(g),
        B_(b) {
}

RGB::RGB(const RGB &rgb) :
        R_(rgb.R_),
        G_(rgb.G_),
        B_(rgb.B_) {
}

bool RGB::isZero() const {
    return !((this->R_ > 0.0f) || (this->G_ > 0.0f) || (this->B_ > 0.0f));
}

void RGB::add(const RGB &rgb) {
    this->R_ += rgb.R_;
    this->G_ += rgb.G_;
    this->B_ += rgb.B_;
}

void RGB::mult(const RGB &rgb) {
    this->R_ *= rgb.R_;
    this->G_ *= rgb.G_;
    this->B_ *= rgb.B_;
}

void RGB::mult(const float f) {
    this->R_ *= f;
    this->G_ *= f;
    this->B_ *= f;
}

void RGB::addSample(RGB &average, const RGB &sample) {
    this->mutex.lock();
    average.samples_ = ++this->samples_;
    this->R_ += sample.R_;
    this->G_ += sample.G_;
    this->B_ += sample.B_;
    average.R_ = this->R_;
    average.G_ = this->G_;
    average.B_ = this->B_;
    this->mutex.unlock();
}

void RGB::average() {
    this->R_ /= this->samples_;
    this->G_ /= this->samples_;
    this->B_ /= this->samples_;
}

void RGB::reset() {
    this->R_ = 0.0f;
    this->G_ = 0.0f;
    this->B_ = 0.0f;
    this->samples_ = 0;
}

void RGB::reset(const float r, const float g, const float b) {
    this->R_ = r;
    this->G_ = g;
    this->B_ = b;
}

unsigned int RGB::RGB2Color() {
    const float r((this->R_ > 1.0f) ? 1.0f : ((this->R_ < 0.0f) ? 0.0f : this->R_));
    const float g((this->G_ > 1.0f) ? 1.0f : ((this->G_ < 0.0f) ? 0.0f : this->G_));
    const float b((this->B_ > 1.0f) ? 1.0f : ((this->B_ < 0.0f) ? 0.0f : this->B_));

    const unsigned char cr(static_cast<unsigned char> (r * 255));
    const unsigned char cg(static_cast<unsigned char> (g * 255));
    const unsigned char cb(static_cast<unsigned char> (b * 255));
    return (0xFF000000 | (cb << 16) | (cg << 8) | cr);
}