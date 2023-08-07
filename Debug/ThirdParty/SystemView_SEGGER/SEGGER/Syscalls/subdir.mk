################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c \
../ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.c \
../ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.c \
../ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.c 

OBJS += \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.o \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.o \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.o 

C_DEPS += \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.d \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.d \
./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/%.o ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/%.su ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/%.cyclo: ../ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/%.c ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Config" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/Sample/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/SystemView_SEGGER/SEGGER" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS/include" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/FreeRTOS/portable/GCC/ARM_CM3" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-SystemView_SEGGER-2f-SEGGER-2f-Syscalls

clean-ThirdParty-2f-SystemView_SEGGER-2f-SEGGER-2f-Syscalls:
	-$(RM) ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.su ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.d ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.o ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_IAR.su ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.d ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.o ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_KEIL.su ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.cyclo ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.d ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.o ./ThirdParty/SystemView_SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_SES.su

.PHONY: clean-ThirdParty-2f-SystemView_SEGGER-2f-SEGGER-2f-Syscalls

