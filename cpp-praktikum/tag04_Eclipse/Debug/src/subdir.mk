################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/InterfaceSIM.cpp \
../src/PIDController.cpp \
../src/keyboard_control.cpp \
../src/main.cpp 

OBJS += \
./src/InterfaceSIM.o \
./src/PIDController.o \
./src/keyboard_control.o \
./src/main.o 

CPP_DEPS += \
./src/InterfaceSIM.d \
./src/PIDController.d \
./src/keyboard_control.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


