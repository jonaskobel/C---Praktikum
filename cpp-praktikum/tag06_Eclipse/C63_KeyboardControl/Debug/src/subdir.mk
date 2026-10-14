################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/A63_KeyboadControl_Hardware.cpp \
../src/InterfaceHW.cpp \
../src/KeyboardControl.cpp 

OBJS += \
./src/A63_KeyboadControl_Hardware.o \
./src/InterfaceHW.o \
./src/KeyboardControl.o 

CPP_DEPS += \
./src/A63_KeyboadControl_Hardware.d \
./src/InterfaceHW.d \
./src/KeyboardControl.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


