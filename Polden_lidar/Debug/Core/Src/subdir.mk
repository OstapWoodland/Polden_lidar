################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Net.c \
../Core/Src/Polden_lidar_main.c \
../Core/Src/delay_mks.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/message_processing.c \
../Core/Src/ring_buff.c \
../Core/Src/sensor.c \
../Core/Src/socket.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/w5500.c \
../Core/Src/wizchip_conf.c 

OBJS += \
./Core/Src/Net.o \
./Core/Src/Polden_lidar_main.o \
./Core/Src/delay_mks.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/message_processing.o \
./Core/Src/ring_buff.o \
./Core/Src/sensor.o \
./Core/Src/socket.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/w5500.o \
./Core/Src/wizchip_conf.o 

C_DEPS += \
./Core/Src/Net.d \
./Core/Src/Polden_lidar_main.d \
./Core/Src/delay_mks.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/message_processing.d \
./Core/Src/ring_buff.d \
./Core/Src/sensor.d \
./Core/Src/socket.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/w5500.d \
./Core/Src/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Net.cyclo ./Core/Src/Net.d ./Core/Src/Net.o ./Core/Src/Net.su ./Core/Src/Polden_lidar_main.cyclo ./Core/Src/Polden_lidar_main.d ./Core/Src/Polden_lidar_main.o ./Core/Src/Polden_lidar_main.su ./Core/Src/delay_mks.cyclo ./Core/Src/delay_mks.d ./Core/Src/delay_mks.o ./Core/Src/delay_mks.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/message_processing.cyclo ./Core/Src/message_processing.d ./Core/Src/message_processing.o ./Core/Src/message_processing.su ./Core/Src/ring_buff.cyclo ./Core/Src/ring_buff.d ./Core/Src/ring_buff.o ./Core/Src/ring_buff.su ./Core/Src/sensor.cyclo ./Core/Src/sensor.d ./Core/Src/sensor.o ./Core/Src/sensor.su ./Core/Src/socket.cyclo ./Core/Src/socket.d ./Core/Src/socket.o ./Core/Src/socket.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/w5500.cyclo ./Core/Src/w5500.d ./Core/Src/w5500.o ./Core/Src/w5500.su ./Core/Src/wizchip_conf.cyclo ./Core/Src/wizchip_conf.d ./Core/Src/wizchip_conf.o ./Core/Src/wizchip_conf.su

.PHONY: clean-Core-2f-Src

