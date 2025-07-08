#ifndef COMPLEMENTARYFILTER_H
#define COMPLEMENTARYFILTER_H

class ComplementaryFilter {
public:
    ComplementaryFilter();
    void setup(float alpha, float dt);
    void update(float gx, float gy, float gz, float ax, float ay, float az, float magYaw);
    float getRoll() const { return roll; }   // radians
    float getPitch() const { return pitch; } // radians
    float getYaw() const { return yaw; }     // radians
    void setYaw(float y) { yaw = y; }
private:
    float alpha; // blending factor
    float dt;    // sample time (s)
    float roll, pitch, yaw;
};

#endif 