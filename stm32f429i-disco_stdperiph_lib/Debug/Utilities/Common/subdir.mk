################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Common/fonts.c \
../Utilities/Common/lcd_log.c 

OBJS += \
./Utilities/Common/fonts.o \
./Utilities/Common/lcd_log.o 

C_DEPS += \
./Utilities/Common/fonts.d \
./Utilities/Common/lcd_log.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Common/%.o: ../Utilities/Common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429ZITx -DSTM32F429I_DISCO -DSTM32 -DSTM32F4 -DUSE_STDPERIPH_DRIVER -DSTM32F429_439xx -I"C:/Users/Alexander Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_stdperiph_lib/CMSIS/core" -I"C:/Users/Alexander Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_stdperiph_lib/CMSIS/device" -I"C:/Users/Alexander Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_stdperiph_lib/StdPeriph_Driver/inc" -I"C:/Users/Alexander Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_stdperiph_lib/Utilities/Common" -I"C:/Users/Alexander Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_stdperiph_lib/Utilities/STM32F429I-Discovery" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


