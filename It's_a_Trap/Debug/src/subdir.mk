################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/25lc080.c \
../src/adc.c \
../src/cr_startup_lpc11.c \
../src/delay.c \
../src/gpio.c \
../src/main.c \
../src/pwm.c \
../src/spi.c 

OBJS += \
./src/25lc080.o \
./src/adc.o \
./src/cr_startup_lpc11.o \
./src/delay.o \
./src/gpio.o \
./src/main.o \
./src/pwm.o \
./src/spi.o 

C_DEPS += \
./src/25lc080.d \
./src/adc.d \
./src/cr_startup_lpc11.d \
./src/delay.d \
./src/gpio.d \
./src/main.d \
./src/pwm.d \
./src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_CMSIS=CMSISv1p30_LPC11xx -D_LPCXpresso_ -DDEBUG -D__CODE_RED -D__REDLIB__ -I"C:\Users\oliveruthio\Documents\LPCXpresso_workspace\Lib_CMSISv1p30_LPC11xx\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


