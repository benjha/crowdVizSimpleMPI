################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/LoadShaders.cpp \
../Source/cCamera.cpp \
../Source/cFrustum.cpp \
../Source/cModel.cpp \
../Source/cProjectionManager.cpp \
../Source/framebufferObject.cpp \
../Source/glx_x11_events.cpp \
../Source/main.cpp \
../Source/vecs.cpp 

OBJS += \
./Source/LoadShaders.o \
./Source/cCamera.o \
./Source/cFrustum.o \
./Source/cModel.o \
./Source/cProjectionManager.o \
./Source/framebufferObject.o \
./Source/glx_x11_events.o \
./Source/main.o \
./Source/vecs.o 

CPP_DEPS += \
./Source/LoadShaders.d \
./Source/cCamera.d \
./Source/cFrustum.d \
./Source/cModel.d \
./Source/cProjectionManager.d \
./Source/framebufferObject.d \
./Source/glx_x11_events.d \
./Source/main.d \
./Source/vecs.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -I../../../Programs/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


