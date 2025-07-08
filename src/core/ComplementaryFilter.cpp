#include "core/ComplementaryFilter.h"
#include <math.h>

ComplementaryFilter::ComplementaryFilter() : alpha(0.98f), dt(0.001f), roll(0), pitch(0), yaw(0) {}

void ComplementaryFilter::setup(float alpha_, float dt_) {
    alpha = alpha_;
    dt = dt_;
}

void ComplementaryFilter::update(float gx, float gy, float gz, float ax, float ay, float az, float magYaw) {
    if (isnan(yaw)) yaw = 0.0f;
    if (isnan(magYaw)) magYaw = 0.0f;
    // Gyro integration (radians)
    roll  += gx * dt;
    pitch += gy * dt;

    // Accel angle (radians)
    float rollAcc  = atan2f(ay, az);
    float pitchAcc = atan2f(-ax, sqrtf(ay * ay + az * az));

    // Complementary filter
    roll  = alpha * roll  + (1.0f - alpha) * rollAcc;
    pitch = alpha * pitch + (1.0f - alpha) * pitchAcc;
    yaw   = alpha * (yaw + gz * dt) + (1.0f - alpha) * magYaw;
} 