################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/driver/common.c \
../Core/Src/driver/dio_control.c \
../Core/Src/driver/flash.c \
../Core/Src/driver/hmi_control.c \
../Core/Src/driver/modbus_rtu.c \
../Core/Src/driver/motor_control.c \
../Core/Src/driver/tick.c 

OBJS += \
./Core/Src/driver/common.o \
./Core/Src/driver/dio_control.o \
./Core/Src/driver/flash.o \
./Core/Src/driver/hmi_control.o \
./Core/Src/driver/modbus_rtu.o \
./Core/Src/driver/motor_control.o \
./Core/Src/driver/tick.o 

C_DEPS += \
./Core/Src/driver/common.d \
./Core/Src/driver/dio_control.d \
./Core/Src/driver/flash.d \
./Core/Src/driver/hmi_control.d \
./Core/Src/driver/modbus_rtu.d \
./Core/Src/driver/motor_control.d \
./Core/Src/driver/tick.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/driver/%.o Core/Src/driver/%.su Core/Src/driver/%.cyclo: ../Core/Src/driver/%.c Core/Src/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/Desktop/BMS/MC/Core/Inc/app" -I"C:/Users/Admin/Desktop/BMS/MC/Core/Inc/driver" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-driver

clean-Core-2f-Src-2f-driver:
	-$(RM) ./Core/Src/driver/common.cyclo ./Core/Src/driver/common.d ./Core/Src/driver/common.o ./Core/Src/driver/common.su ./Core/Src/driver/dio_control.cyclo ./Core/Src/driver/dio_control.d ./Core/Src/driver/dio_control.o ./Core/Src/driver/dio_control.su ./Core/Src/driver/flash.cyclo ./Core/Src/driver/flash.d ./Core/Src/driver/flash.o ./Core/Src/driver/flash.su ./Core/Src/driver/hmi_control.cyclo ./Core/Src/driver/hmi_control.d ./Core/Src/driver/hmi_control.o ./Core/Src/driver/hmi_control.su ./Core/Src/driver/modbus_rtu.cyclo ./Core/Src/driver/modbus_rtu.d ./Core/Src/driver/modbus_rtu.o ./Core/Src/driver/modbus_rtu.su ./Core/Src/driver/motor_control.cyclo ./Core/Src/driver/motor_control.d ./Core/Src/driver/motor_control.o ./Core/Src/driver/motor_control.su ./Core/Src/driver/tick.cyclo ./Core/Src/driver/tick.d ./Core/Src/driver/tick.o ./Core/Src/driver/tick.su

.PHONY: clean-Core-2f-Src-2f-driver

