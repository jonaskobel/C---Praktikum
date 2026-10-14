################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/A64_RobotControl_Hardware.cpp \
../src/InterfaceHW.cpp \
../src/Maneuver.cpp \
../src/RobotControl.cpp 

OBJS += \
./src/A64_RobotControl_Hardware.o \
./src/InterfaceHW.o \
./src/Maneuver.o \
./src/RobotControl.o 

CPP_DEPS += \
./src/A64_RobotControl_Hardware.d \
./src/InterfaceHW.d \
./src/Maneuver.d \
./src/RobotControl.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


