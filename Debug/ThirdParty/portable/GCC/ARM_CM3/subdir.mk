################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/portable/GCC/ARM_CM3/port.c 

OBJS += \
./ThirdParty/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./ThirdParty/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/portable/GCC/ARM_CM3/%.o: ../ThirdParty/portable/GCC/ARM_CM3/%.c ThirdParty/portable/GCC/ARM_CM3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"/home/jesus/STM32CubeIDE/workspace_1.7.0/STMF103C8_Clock/ThirdParty" -I"/home/jesus/STM32CubeIDE/workspace_1.7.0/STMF103C8_Clock/ThirdParty/include" -I"/home/jesus/STM32CubeIDE/workspace_1.7.0/STMF103C8_Clock/ThirdParty/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

