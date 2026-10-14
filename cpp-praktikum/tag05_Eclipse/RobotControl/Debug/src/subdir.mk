################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/InterfaceSIM.cpp \
../src/PIDController.cpp \
../src/PositionEstimation.cpp \
../src/RobotControl.cpp \
../src/main.cpp \
../src/maneuver.cpp 

OBJS += \
./src/InterfaceSIM.o \
./src/PIDController.o \
./src/PositionEstimation.o \
./src/RobotControl.o \
./src/main.o \
./src/maneuver.o 

CPP_DEPS += \
./src/InterfaceSIM.d \
./src/PIDController.d \
./src/PositionEstimation.d \
./src/RobotControl.d \
./src/main.d \
./src/maneuver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


