################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Temp/Src/TempRtos.c \
../Core/Temp/Src/control.c \
../Core/Temp/Src/dht11_driver.c \
../Core/Temp/Src/fonts.c \
../Core/Temp/Src/ssd1306.c \
../Core/Temp/Src/uart.c 

OBJS += \
./Core/Temp/Src/TempRtos.o \
./Core/Temp/Src/control.o \
./Core/Temp/Src/dht11_driver.o \
./Core/Temp/Src/fonts.o \
./Core/Temp/Src/ssd1306.o \
./Core/Temp/Src/uart.o 

C_DEPS += \
./Core/Temp/Src/TempRtos.d \
./Core/Temp/Src/control.d \
./Core/Temp/Src/dht11_driver.d \
./Core/Temp/Src/fonts.d \
./Core/Temp/Src/ssd1306.d \
./Core/Temp/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Temp/Src/%.o Core/Temp/Src/%.su Core/Temp/Src/%.cyclo: ../Core/Temp/Src/%.c Core/Temp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/Documents/stm32/Temp_linux/Core/Temp/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Temp-2f-Src

clean-Core-2f-Temp-2f-Src:
	-$(RM) ./Core/Temp/Src/TempRtos.cyclo ./Core/Temp/Src/TempRtos.d ./Core/Temp/Src/TempRtos.o ./Core/Temp/Src/TempRtos.su ./Core/Temp/Src/control.cyclo ./Core/Temp/Src/control.d ./Core/Temp/Src/control.o ./Core/Temp/Src/control.su ./Core/Temp/Src/dht11_driver.cyclo ./Core/Temp/Src/dht11_driver.d ./Core/Temp/Src/dht11_driver.o ./Core/Temp/Src/dht11_driver.su ./Core/Temp/Src/fonts.cyclo ./Core/Temp/Src/fonts.d ./Core/Temp/Src/fonts.o ./Core/Temp/Src/fonts.su ./Core/Temp/Src/ssd1306.cyclo ./Core/Temp/Src/ssd1306.d ./Core/Temp/Src/ssd1306.o ./Core/Temp/Src/ssd1306.su ./Core/Temp/Src/uart.cyclo ./Core/Temp/Src/uart.d ./Core/Temp/Src/uart.o ./Core/Temp/Src/uart.su

.PHONY: clean-Core-2f-Temp-2f-Src

