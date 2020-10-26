################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include ThirdParty/freeRTOS/org/Source/portable/MemMang/subdir.mk
-include ThirdParty/freeRTOS/org/Source/portable/GCC/ARM_CM4F/subdir.mk
-include ThirdParty/freeRTOS/org/Source/subdir.mk
-include StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: stm32Nucelo_HelloWorld.elf

# Tool invocations
stm32Nucelo_HelloWorld.elf: $(OBJS) $(USER_OBJS) D:\All_Learnings\Udemy_embedded\Workspace\stm32Nucelo_HelloWorld/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=rdimon.specs -lc -lrdimon -T"D:/All_Learnings/Udemy_embedded/Workspace/stm32Nucelo_HelloWorld/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "stm32Nucelo_HelloWorld.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "stm32Nucelo_HelloWorld.elf" "stm32Nucelo_HelloWorld.bin"
	arm-none-eabi-size "stm32Nucelo_HelloWorld.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
