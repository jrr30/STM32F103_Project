################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/croutine.c \
../ThirdParty/event_groups.c \
../ThirdParty/list.c \
../ThirdParty/queue.c \
../ThirdParty/stream_buffer.c \
../ThirdParty/tasks.c \
../ThirdParty/timers.c 

OBJS += \
./ThirdParty/croutine.o \
./ThirdParty/event_groups.o \
./ThirdParty/list.o \
./ThirdParty/queue.o \
./ThirdParty/stream_buffer.o \
./ThirdParty/tasks.o \
./ThirdParty/timers.o 

C_DEPS += \
./ThirdParty/croutine.d \
./ThirdParty/event_groups.d \
./ThirdParty/list.d \
./ThirdParty/queue.d \
./ThirdParty/stream_buffer.d \
./ThirdParty/tasks.d \
./ThirdParty/timers.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/%.o: ../ThirdParty/%.c ThirdParty/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"/home/jesus/STM32CubeIDE/workspace_1.7.0/STMF103C8_Clock/ThirdParty" -I"/home/jesus/STM32CubeIDE/workspace_1.7.0/STMF103C8_Clock/ThirdParty/include" -I"/home/jesus/STM32CubeIDE/workspace_1.7.0/STMF103C8_Clock/ThirdParty/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

