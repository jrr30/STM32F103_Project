################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.c \
../ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.c \
../ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
../ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.o \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.o \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.d \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.d \
./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SystemView_SEGGER/SEGGER/%.o ThirdParty/SystemView_SEGGER/SEGGER/%.su ThirdParty/SystemView_SEGGER/SEGGER/%.cyclo: ../ThirdParty/SystemView_SEGGER/SEGGER/%.c ThirdParty/SystemView_SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Config" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Sample/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/SEGGER" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS/include" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS/portable/GCC/ARM_CM3" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
ThirdParty/SystemView_SEGGER/SEGGER/%.o: ../ThirdParty/SystemView_SEGGER/SEGGER/%.S ThirdParty/SystemView_SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Config" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-ThirdParty-2f-SystemView_SEGGER-2f-SEGGER

clean-ThirdParty-2f-SystemView_SEGGER-2f-SEGGER:
	-$(RM) ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.d ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.o ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT.su ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.d ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.o ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_RTT_printf.su ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.d ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.o ./ThirdParty/SystemView_SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-ThirdParty-2f-SystemView_SEGGER-2f-SEGGER

