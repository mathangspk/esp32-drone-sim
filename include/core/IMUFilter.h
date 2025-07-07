#ifndef IMUFILTER_H
#define IMUFILTER_H

class BiquadLPF {
public:
    BiquadLPF();
    void setup(float filterFreq, float sampleRate);
    float update(float input);

private:
    float b0, b1, b2, a1, a2;
    float z1, z2;
};

#endif 