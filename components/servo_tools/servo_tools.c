#include "servo_tools.h"

// Estrutura para armazenar o estado do servomotor
typedef struct {
    ServoConfig config;
    ServoAngle current_angle;
} ServoState;

static ServoState servo_states[MAX_SERVOS];  // Defina MAX_SERVOS conforme a necessidade

esp_err_t servo_init(ServoConfig *config) {
    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Inicialize o servomotor
    esp_err_t ret = hw_servo_init(config->gpio_num);
    if (ret != ESP_OK) {
        return ret;
    }

    // Armazene o estado do servomotor
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_states[i].config.gpio_num == config->gpio_num) {
            return ESP_OK;  // Já inicializado
        }
    }

    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_states[i].config.gpio_num == 0) {
            servo_states[i].config = *config;
            servo_states[i].current_angle = 0;
            return ESP_OK;
        }
    }

    return ESP_ERR_NO_MEM;  // Sem espaço para mais servos
}

esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle) {
    if (config == NULL || angle < 0 || angle > 180) {
        return ESP_ERR_INVALID_ARG;
    }

    // Encontre o estado do servomotor
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_states[i].config.gpio_num == config->gpio_num) {
            // Converta o ângulo em largura de pulso
            uint32_t pulse_width_us = config->min_pulse_us +
                (angle * (config->max_pulse_us - config->min_pulse_us) / 180);

            esp_err_t ret = hw_servo_set_pulse_width(config->gpio_num, pulse_width_us);
            if (ret != ESP_OK) {
                return ret;
            }

            servo_states[i].current_angle = angle;
            return ESP_OK;
        }
    }

    return ESP_ERR_INVALID_ARG;  // Servomotor não encontrado
}

esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle) {
    if (config == NULL || angle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Encontre o estado do servomotor
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_states[i].config.gpio_num == config->gpio_num) {
            *angle = servo_states[i].current_angle;
            return ESP_OK;
        }
    }

    return ESP_ERR_INVALID_ARG;  // Servomotor não encontrado
}
