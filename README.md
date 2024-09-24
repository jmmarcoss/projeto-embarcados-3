| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

## ▶️ Link Youtube Funcionamento 

https://youtu.be/hf9BEdIc-L4

##  ▶️  Run project
1. Clone the repository to your local machine:
``` bash
git clone https://github.com/jmmarcoss/projeto-embarcados-3.git
```

2. Navigate to the project directory:
``` bash
cd embarcados-projeto-3
```

3. Open the ESP-IDF

4. Select: **Configure ESP-IDF Extension**
   
5. Select: **Use Existing Setup**
   
6. Select: **Advanced / Add .vscode subdirectory files**
   
7. Select: **Build**

8. Close the ESP-IDF
   
9. Verify the status of build

## Funcionalidades
- Controle de servo motor.
- Abstração de hardware para controle de posição
- Implementação de ferramentas de controle customizadas para servo.

## Requisitos

- **SDK:** Necessário instalar o SDK de desenvolvimento específico para sistemas embarcados utilizado neste projeto (como o ESP-IDF, por exemplo).
- **Compilador CMake:** Certifique-se de ter o CMake instalado para compilar e gerenciar o build.
- **Plataforma:** Projetos testados em sistemas Linux ou macOS. Adaptável para outros sistemas operacionais.

## 📁 Project Structure

The project **embarcados-projeto-3** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── build/
│   # Pasta onde os arquivos compilados são armazenados
│
├── components/
│   ├── servo_tools/
│   │   ├── include/
│   │   │   └── servo_tools.h  # Arquivo de cabeçalho para as funções de controle do servo
│   │   ├── servo_tools.c      # Arquivo fonte para as funções de controle do servo
│   │   └── CMakeLists.txt     # Arquivo CMake específico para o
componente servo_tools
│   │
│   └── servo_hw/
│       ├── include/
│       │   └── servo_hw.h     # Arquivo de cabeçalho para a abstração de hardware do servo
│       ├── servo_hw.c         # Arquivo fonte para a abstração de hardware do servo
│       └── CMakeLists.txt     # Arquivo CMake específico para o componente servo_hw
│
├── docs/
│   ├── documentation.pdf      # PDF com documentação para firmware e bibliotecas
│   ├── state_machine.png      # Diagrama de máquina de estados para firmware
│   └── circuit_diagram.jpg    # Diagrama de circuito para firmware │
├── main/
│   ├── CMakeLists.txt         # Arquivo CMake para a aplicação principal
│   └── main.c                 # Arquivo fonte da aplicação principal │
├── .gitignore                 # Arquivo que especifica quais arquivos ou diretórios devem ser ignorados pelo Git
│
├── CMakeLists.txt             # Arquivo CMake principal para todo o projeto
│
└── sdkconfig                  # Arquivo de configuração (gerado/gerenciado pelo "make menuconfig")
```

# embarcados-projeto-3
