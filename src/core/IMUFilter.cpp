#include "core/IMUFilter.h"
#include <math.h>

BiquadLPF::BiquadLPF() : b0(1), b1(0), b2(0), a1(0), a2(0), z1(0), z2(0) {}

void BiquadLPF::setup(float filterFreq, float sampleRate) {
    float omega = 2.0f * M_PI * filterFreq / sampleRate;
    float sn = sinf(omega);
    float cs = cosf(omega);
    float alpha = sn / sqrtf(2.0f);

    float b0_ = (1.0f - cs) / 2.0f;
    float b1_ = 1.0f - cs;
    float b2_ = (1.0f - cs) / 2.0f;
    float a0_ = 1.0f + alpha;
    float a1_ = -2.0f * cs;
    float a2_ = 1.0f - alpha;

    b0 = b0_ / a0_;
    b1 = b1_ / a0_;
    b2 = b2_ / a0_;
    a1 = a1_ / a0_;
    a2 = a2_ / a0_;
    z1 = z2 = 0;
}

float BiquadLPF::update(float input) {
    float output = b0 * input + z1;
    z1 = b1 * input + z2 - a1 * output;
    z2 = b2 * input - a2 * output;
    return output;
} 