#ifndef SERVO_HW_H
#define SERVO_HW_H

#include "esp_err.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
// Inicializa o GPIO para o controle do servo
#define MAX_SERVOS 10  // Defina o número máximo de servos

esp_err_t hw_servo_init(uint8_t gpio_num);

// Define a largura de pulso para controlar o ângulo do servo
esp_err_t hw_servo_set_pulse_width(uint8_t gpio_num, uint32_t pulse_width_us);

// Desabilita o PWM e libera os recursos
esp_err_t hw_servo_deinit(uint8_t gpio_num);

#endif // SERVO_HW_H
