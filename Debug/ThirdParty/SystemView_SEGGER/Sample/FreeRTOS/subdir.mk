################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.c 

OBJS += \
./ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.o 

C_DEPS += \
./ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/%.o ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/%.su ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/%.cyclo: ../ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/%.c ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Config" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Sample/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/SEGGER" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS/include" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS/portable/GCC/ARM_CM3" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-SystemView_SEGGER-2f-Sample-2f-FreeRTOS

clean-ThirdParty-2f-SystemView_SEGGER-2f-Sample-2f-FreeRTOS:
	-$(RM) ./ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.cyclo ./ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.d ./ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.o ./ThirdParty/SystemView_SEGGER/Sample/FreeRTOS/SEGGER_SYSVIEW_FreeRTOS.su

.PHONY: clean-ThirdParty-2f-SystemView_SEGGER-2f-Sample-2f-FreeRTOS

