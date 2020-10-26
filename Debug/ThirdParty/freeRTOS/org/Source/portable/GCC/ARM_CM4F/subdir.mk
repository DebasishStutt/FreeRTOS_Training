################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F/%.o: ../ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/StdPeriph_Driver/inc" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/Config" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/ThirdParty/freeRTOS/org/Source/include" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/inc" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/CMSIS/device" -I"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


