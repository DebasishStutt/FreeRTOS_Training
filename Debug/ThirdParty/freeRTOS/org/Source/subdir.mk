################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/freeRTOS/org/Source/croutine.c \
../ThirdParty/freeRTOS/org/Source/event_groups.c \
../ThirdParty/freeRTOS/org/Source/list.c \
../ThirdParty/freeRTOS/org/Source/queue.c \
../ThirdParty/freeRTOS/org/Source/stream_buffer.c \
../ThirdParty/freeRTOS/org/Source/tasks.c \
../ThirdParty/freeRTOS/org/Source/timers.c 

OBJS += \
./ThirdParty/freeRTOS/org/Source/croutine.o \
./ThirdParty/freeRTOS/org/Source/event_groups.o \
./ThirdParty/freeRTOS/org/Source/list.o \
./ThirdParty/freeRTOS/org/Source/queue.o \
./ThirdParty/freeRTOS/org/Source/stream_buffer.o \
./ThirdParty/freeRTOS/org/Source/tasks.o \
./ThirdParty/freeRTOS/org/Source/timers.o 

C_DEPS += \
./ThirdParty/freeRTOS/org/Source/croutine.d \
./ThirdParty/freeRTOS/org/Source/event_groups.d \
./ThirdParty/freeRTOS/org/Source/list.d \
./ThirdParty/freeRTOS/org/Source/queue.d \
./ThirdParty/freeRTOS/org/Source/stream_buffer.d \
./ThirdParty/freeRTOS/org/Source/tasks.d \
./ThirdParty/freeRTOS/org/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/freeRTOS/org/Source/%.o: ../ThirdParty/freeRTOS/org/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/StdPeriph_Driver/inc" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/Config" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/ThirdParty/freeRTOS/org/Source/include" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/inc" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/CMSIS/device" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


