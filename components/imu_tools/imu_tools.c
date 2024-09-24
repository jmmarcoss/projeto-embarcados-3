// imu_tools.c

#include "imu_tools.h"
#include <math.h>

esp_err_t imu_read_data(IMUData *data) {
    if (imu_get_acceleration_data(&data->accel) != ESP_OK) return ESP_FAIL;
    if (imu_get_gyroscope_data(&data->gyro) != ESP_OK) return ESP_FAIL;
    return ESP_OK;
}

esp_err_t imu_calculate_quaternion(const IMUData *data, Quaternion *quaternion) {
    // Exemplo simplificado de cálculo de quaternion
    quaternion->w = 1.0;
    quaternion->x = data->accel.x;
    quaternion->y = data->accel.y;
    quaternion->z = data->accel.z;
    return ESP_OK;
}

esp_err_t imu_calculate_euler_angles(const Quaternion *quaternion, EulerAngle *euler) {

    if (isnan(quaternion->w) || isnan(quaternion->x) || isnan(quaternion->y) || isnan(quaternion->z)) {
        return ESP_ERR_INVALID_ARG;
    }

    float sinr_cosp = 2.0f * (quaternion->w * quaternion->x + quaternion->y * quaternion->z);
    float cosr_cosp = 1.0f - 2.0f * (quaternion->x * quaternion->x + quaternion->y * quaternion->y);
    euler->roll = atan2f(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (quaternion->w * quaternion->y - quaternion->z * quaternion->x);
    if (fabsf(sinp) >= 1.0f) {
        euler->pitch = copysignf(M_PI / 2.0f, sinp);
    } else {
        euler->pitch = asinf(sinp);
    }

    float siny_cosp = 2.0f * (quaternion->w * quaternion->z + quaternion->x * quaternion->y);
    float cosy_cosp = 1.0f - 2.0f * (quaternion->y * quaternion->y + quaternion->z * quaternion->z);
    euler->yaw = atan2f(siny_cosp, cosy_cosp);

    if (isnan(euler->roll) || isnan(euler->pitch) || isnan(euler->yaw)) {
        return ESP_FAIL;
    }

    return ESP_OK;
}