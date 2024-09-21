#ifndef SERVO_TOOLS_H
#define SERVO_TOOLS_H

#include "esp_err.h"
#include "servo_hw.h"

// Estrutura para configurar o servomotor
typedef struct {
    uint8_t gpio_num;       // Pino GPIO ao qual o servomotor está conectado
    uint32_t freq_hz;       // Frequência PWM
    uint32_t min_pulse_us;  // Largura de pulso mínima em microsegundos
    uint32_t max_pulse_us;  // Largura de pulso máxima em microsegundos
} ServoConfig;

// Tipo para representar o ângulo do servomotor
typedef int ServoAngle;

// Inicializa o servomotor com base na configuração fornecida
esp_err_t servo_init(ServoConfig *config);

// Define o ângulo do servomotor
esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle);

// Obtém o ângulo atual do servomotor
esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle);

#endif // SERVO_TOOLS_H
