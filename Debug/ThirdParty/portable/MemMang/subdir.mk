################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/portable/MemMang/heap_4.c 

OBJS += \
./ThirdParty/portable/MemMang/heap_4.o 

C_DEPS += \
./ThirdParty/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/portable/MemMang/%.o ThirdParty/portable/MemMang/%.su ThirdParty/portable/MemMang/%.cyclo: ../ThirdParty/portable/MemMang/%.c ThirdParty/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/include" -I"C:/Users/Jesus/STM32CubeIDE/STMF103C8_Clock/ThirdParty/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-portable-2f-MemMang

clean-ThirdParty-2f-portable-2f-MemMang:
	-$(RM) ./ThirdParty/portable/MemMang/heap_4.cyclo ./ThirdParty/portable/MemMang/heap_4.d ./ThirdParty/portable/MemMang/heap_4.o ./ThirdParty/portable/MemMang/heap_4.su

.PHONY: clean-ThirdParty-2f-portable-2f-MemMang

