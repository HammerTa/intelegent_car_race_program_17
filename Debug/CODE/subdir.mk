################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/IMG_DEAL.c \
../CODE/Key.c \
../CODE/LCD_drive.c \
../CODE/VCAN_computer.c \
../CODE/ZZK_Threshold.c \
../CODE/inti.c \
../CODE/motor.c 

OBJS += \
./CODE/IMG_DEAL.o \
./CODE/Key.o \
./CODE/LCD_drive.o \
./CODE/VCAN_computer.o \
./CODE/ZZK_Threshold.o \
./CODE/inti.o \
./CODE/motor.o 

COMPILED_SRCS += \
./CODE/IMG_DEAL.src \
./CODE/Key.src \
./CODE/LCD_drive.src \
./CODE/VCAN_computer.src \
./CODE/ZZK_Threshold.src \
./CODE/inti.src \
./CODE/motor.src 

C_DEPS += \
./CODE/IMG_DEAL.d \
./CODE/Key.d \
./CODE/LCD_drive.d \
./CODE/VCAN_computer.d \
./CODE/ZZK_Threshold.d \
./CODE/inti.d \
./CODE/motor.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.src: ../CODE/%.c CODE/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fC:/Users/HammerTank/Desktop/IntelligentCar/NationalRace/NationalRace_Program/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

CODE/%.o: ./CODE/%.src CODE/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


