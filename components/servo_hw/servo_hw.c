#include "servo_hw.h"
#define LEDC_RESOLUTION LEDC_TIMER_14_BIT // Resolução de 14 bits
const int minDuty = (int)(0.023 * (1 << LEDC_RESOLUTION));
const int maxDuty = (int)(0.124 * (1 << LEDC_RESOLUTION));
typedef struct {
    uint8_t gpio_num;
    uint32_t channel;
} ServoChannel;
static ServoChannel servo_channels[MAX_SERVOS];
static void configure_ledc_channel(uint8_t gpio_num, uint32_t channel) {
    ledc_channel_config_t ledc_channel = {
        .gpio_num = gpio_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
}

esp_err_t hw_servo_init(uint8_t gpio_num) {
    // Configure o temporizador
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&ledc_timer);

    // Encontre um canal disponível para o novo servo
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_channels[i].gpio_num == 0) {
            servo_channels[i].gpio_num = gpio_num;
            servo_channels[i].channel = i;  // Use o índice como canal
            configure_ledc_channel(gpio_num, i);

            // Varre o servo motor
            for (int angle = 0; angle <= 180; angle += 10) {
                uint32_t pulse_width_us = minDuty + (angle * (maxDuty - minDuty) / 180); // Exemplo de mapeamento
                ledc_set_duty(LEDC_LOW_SPEED_MODE, i, pulse_width_us);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, i);
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            for (int angle = 180; angle >= 0; angle -= 10) {
                uint32_t pulse_width_us = minDuty + (angle * (maxDuty - minDuty) / 180); // Exemplo de mapeamento
                ledc_set_duty(LEDC_LOW_SPEED_MODE, i, pulse_width_us);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, i);
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }

            return ESP_OK;
        }
    }

    return ESP_ERR_NO_MEM;  // Sem espaço para mais servos
}

esp_err_t hw_servo_set_pulse_width(uint8_t gpio_num, uint32_t pulse_width_us) {
    // Encontre o canal correspondente ao GPIO
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_channels[i].gpio_num == gpio_num) {
            int duty_cycle = pulse_width_us; // Exemplo de mapeamento
            ledc_set_duty(LEDC_LOW_SPEED_MODE, servo_channels[i].channel, duty_cycle);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, servo_channels[i].channel);
            return ESP_OK;
        }
    }

    return ESP_ERR_INVALID_ARG;  // GPIO não encontrado
}

esp_err_t hw_servo_deinit(uint8_t gpio_num) {
    // Encontre o canal correspondente ao GPIO
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servo_channels[i].gpio_num == gpio_num) {
            ledc_stop(LEDC_LOW_SPEED_MODE, servo_channels[i].channel, 0);
            servo_channels[i].gpio_num = 0;  // Libere o GPIO
            return ESP_OK;
        }
    }

    return ESP_ERR_INVALID_ARG;  // GPIO não encontrado
}