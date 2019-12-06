################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/LoadShaders.cpp \
../source/cCamera.cpp \
../source/cFrustum.cpp \
../source/cModel.cpp \
../source/cProjectionManager.cpp \
../source/framebufferObject.cpp \
../source/glx_x11_events.cpp \
../source/main.cpp \
../source/vecs.cpp 

OBJS += \
./source/LoadShaders.o \
./source/cCamera.o \
./source/cFrustum.o \
./source/cModel.o \
./source/cProjectionManager.o \
./source/framebufferObject.o \
./source/glx_x11_events.o \
./source/main.o \
./source/vecs.o 

CPP_DEPS += \
./source/LoadShaders.d \
./source/cCamera.d \
./source/cFrustum.d \
./source/cModel.d \
./source/cProjectionManager.d \
./source/framebufferObject.d \
./source/glx_x11_events.d \
./source/main.d \
./source/vecs.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -I../../../Programs/include -I../header -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


