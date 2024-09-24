#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "imu_tools.h"
#include "servo_tools.h"

#define INIT_RETRIES 3

// IMU
#define ADDR 0x68	
#define SDA_PIN GPIO_NUM_21 
#define SCL_PIN GPIO_NUM_22 

// Servos
#define SERVO_ROLL_GPIO GPIO_NUM_13
#define SERVO_PITCH_GPIO GPIO_NUM_12
#define LEDC_RESOLUTION LEDC_TIMER_14_BIT // Resolução de 14 bits
#define SERVO_ROLL_FREQ 50
#define SERVO_PITCH_FREQ 50
#define SERVO_ROLL_MIN_PULSE (int)(0.023 * (1 << LEDC_RESOLUTION))
#define SERVO_ROLL_MAX_PULSE (int)(0.124 * (1 << LEDC_RESOLUTION))
#define SERVO_PITCH_MIN_PULSE (int)(0.023 * (1 << LEDC_RESOLUTION))
#define SERVO_PITCH_MAX_PULSE (int)(0.124 * (1 << LEDC_RESOLUTION))

#define DELAY_MS 1000

static const char *TAG = "IMU_SERVO_CONTROL";

float rad_to_angle(float rad) {

    rad = fmodf(rad + M_PI, 2 * M_PI) - M_PI;
    float angle_deg = (rad + M_PI) * (180.0f / (2 * M_PI));

    return fmaxf(0.0f, fminf(180.0f, angle_deg));
}

typedef enum {
    INIT,
    IMU_READ_DATA,
    IMU_CALCULATE_QUATERNION,
    IMU_CALCULATE_EULER,
    SET_SERVO_ROLL,
    SET_SERVO_PITCH,
    IDLE,
    ERROR_IMU,
    ERROR_SERVO
} State;

void app_main(void)
{
    State current_state = INIT;
    IMUData imu_data;
    Quaternion quaternion;
    EulerAngle euler;
    int init_retries = 0;
    ServoConfig servo_roll, servo_pitch;
    ServoAngle angle_roll, angle_pitch;

    while (1) {
        switch (current_state) {
            case INIT:
                ESP_LOGI(TAG, "Initializing IMU...");
                ESP_LOGI(TAG, "Initializing Servo...");

                ServoConfig servo_roll = {
                    .gpio_num = SERVO_ROLL_GPIO,
                    .freq_hz = SERVO_ROLL_FREQ,
                    .min_pulse_us = SERVO_ROLL_MIN_PULSE,
                    .max_pulse_us = SERVO_ROLL_MAX_PULSE
                };

                ServoConfig servo_pitch = {
                    .gpio_num = SERVO_PITCH_GPIO,
                    .freq_hz = SERVO_PITCH_FREQ,
                    .min_pulse_us = SERVO_PITCH_MIN_PULSE,
                    .max_pulse_us = SERVO_PITCH_MAX_PULSE
                };

                if (imu_init(ADDR, SDA_PIN, SCL_PIN) == ESP_OK &&
                    servo_init(&servo_roll) == ESP_OK &&
                    servo_init(&servo_pitch) == ESP_OK) {
                    current_state = IMU_READ_DATA;
                    ESP_LOGI(TAG, "IMU initialized successfully");
                    ESP_LOGI(TAG, "Servos initialized successfully");
                } else {
                    init_retries++;
                    if (init_retries >= INIT_RETRIES) {
                        current_state = ERROR_IMU;
                        ESP_LOGE(TAG, "Failed to initialize after %d attempts", INIT_RETRIES);
                    }
                }
                break;

            case IMU_READ_DATA:
                if (imu_read_data(&imu_data) == ESP_OK) {
                    current_state = IMU_CALCULATE_QUATERNION;
                } else {
                    current_state = ERROR_IMU;
                    ESP_LOGE(TAG, "Failed to read IMU data");
                }
                break;

            case IMU_CALCULATE_QUATERNION:
                if (imu_calculate_quaternion(&imu_data, &quaternion) == ESP_OK) {
                    current_state = IMU_CALCULATE_EULER;
                } else {
                    current_state = ERROR_IMU;
                    ESP_LOGE(TAG, "Failed to calculate quaternion");
                }
                break;

            case IMU_CALCULATE_EULER:
                if (imu_calculate_euler_angles(&quaternion, &euler) == ESP_OK) {
                    current_state = SET_SERVO_ROLL;
                } else {
                    current_state = ERROR_IMU;
                    ESP_LOGE(TAG, "Failed to calculate Euler angles");
                }
                break;

            case SET_SERVO_ROLL:
                {
                    angle_roll = rad_to_angle(euler.roll);
                    if (servo_set_angle(&servo_roll, angle_roll) == ESP_OK) {
                        current_state = SET_SERVO_PITCH;
                    } else {
                        current_state = ERROR_SERVO;
                        ESP_LOGE(TAG, "Failed to set servo roll");
                    }
                }
                break;

            case SET_SERVO_PITCH:
                {
                    angle_pitch = rad_to_angle(euler.pitch);
                    if (servo_set_angle(&servo_pitch, angle_pitch) == ESP_OK) {
                        current_state = IDLE;
                    } else {
                        current_state = ERROR_SERVO;
                        ESP_LOGE(TAG, "Failed to set servo pitch");
                    }
                }
                break;

            case IDLE:
                ESP_LOGI(TAG, "Roll: %.3f, Pitch: %.3f, Yaw: %.3f", euler.roll, euler.pitch, euler.yaw);
                vTaskDelay(pdMS_TO_TICKS(DELAY_MS));  // Agurda 1000ms antes da proxima leitura
                current_state = IMU_READ_DATA;
                break;

            case ERROR_IMU:
                ESP_LOGE(TAG, "IMU error occurred. Attempting to reinitialize...");
                vTaskDelay(pdMS_TO_TICKS(DELAY_MS));  // Agurda 1000ms (1s) antes de tentar novamente
                current_state = INIT;
                init_retries = 0;
                break;

            case ERROR_SERVO:
                ESP_LOGE(TAG, "Servo error occurred. Continuing with next cycle...");
                vTaskDelay(pdMS_TO_TICKS(DELAY_MS));  // Agurda 1000ms (1s) antes de continuar
                current_state = INIT;
                break;
        }
    }
}

