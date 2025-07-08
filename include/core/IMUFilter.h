#ifndef IMUFILTER_H
#define IMUFILTER_H

class BiquadLPF {
public:
    BiquadLPF();
    void setup(float filterFreq, float sampleRate);
    float update(float input);
    void update(float gx, float gy, float gz, float ax, float ay, float az, float magYaw);

private:
    float b0, b1, b2, a1, a2;
    float z1, z2;
};

#endif 