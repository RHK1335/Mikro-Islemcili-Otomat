################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SW-EK-TM4C123GXL-2.1.4.178/driverlib/tinymt32.c 

OBJS += \
./SW-EK-TM4C123GXL-2.1.4.178/driverlib/tinymt32.o 

C_DEPS += \
./SW-EK-TM4C123GXL-2.1.4.178/driverlib/tinymt32.d 


# Each subdirectory must supply rules for building sources it contributes
SW-EK-TM4C123GXL-2.1.4.178/driverlib/%.o: ../SW-EK-TM4C123GXL-2.1.4.178/driverlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DPART_TM4C123GH6PM -I"/home/student/Desktop/Embedded-System-Programming-master/SW-EK-TM4C123GXL-2.1.4.178" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

