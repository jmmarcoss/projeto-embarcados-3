idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ""
                    REQUIRES driver
                    PRIV_REQUIRES sensor_imu
                    imu_tools
                    servo_tools)