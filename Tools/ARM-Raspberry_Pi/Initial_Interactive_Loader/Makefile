
ARM-TOOLS-PATH = /opt/gcc-arm/latest
EXEC = ${ARM-TOOLS-PATH}/bin/arm-none-eabi
CPP-LIBS = ${ARM-TOOLS-PATH}/lib/gcc/arm-none-eabi/10.3.1
HEX2BIN_DIR = ../../../Library/hex2bin
FSM_DIR = ../../../Library/FSM
DISPLAY_DIR = ../../../Library/Display
PORTING_DIR = ../../../Porting/ARM-Raspberry_Pi
COPTS = -Wno-write-strings -O2 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -nostdlib -nostartfiles -ffreestanding -fno-exceptions

INCLUDE_PATHS = -I ${PORTING_DIR}/Include \
	-I ${FSM_DIR}/Include \
	-I ${HEX2BIN_DIR}/Include \
	-I ${DISPLAY_DIR}/Include \
	-I ./Include

SOURCES = main.cpp \
	InitialInteractiveLoaderSM.cpp \
	periph.cpp \
	${HEX2BIN_DIR}/Hex2BinEFSMachine.cpp \
	${DISPLAY_DIR}/formatter.cpp
# arm_cpu.o has to be first in linker list
OBJECTS = arm_cpu.o \
	main.o \
	periph_mini_uart.o \
	periph_timers.o \
	periph_misc.o \
	bcm_registers.o \
	arm_debug_tools.o \
	Hex2BinEFSMachine.o \
	InitialInteractiveLoaderSM.o \
	formatter.o

img : ld-image
	@echo GCC-ARM creates binary image file from elf file
	@${EXEC}-objcopy InitialInteractiveLoader.elf -O binary kernel.img

hex : ld-app
	@echo GCC-ARM creates HEX file from elf file
	@${EXEC}-objcopy IIL.elf -O ihex IIL.hex

ld-image : ${OBJECTS}
	@echo  GCC-ARM links $^ files
	@${EXEC}-ld -L${CPP-LIBS} $^ -lgcc -Map map.lst -T loader -o InitialInteractiveLoader.elf
	@${EXEC}-objdump -D InitialInteractiveLoader.elf > InitialInteractiveLoader.lst

ld-app : ${OBJECTS}
	@echo GCC-ARM links $^ files
	@${EXEC}-ld -L${CPP-LIBS} $^ -lgcc -Map map.lst -T loader-app -o IIL.elf
	@${EXEC}-objdump -D IIL.elf > InitialInteractiveLoader.lst

main.o : main.cpp Include/iil.hpp ${PORTING_DIR}/Include/bcm_registers.hpp ${PORTING_DIR}/Include/gpio.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<

InitialInteractiveLoaderSM.o : InitialInteractiveLoaderSM.cpp Include/iil.hpp ${HEX2BIN_DIR}/Include/hex2bin.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<

periph_mini_uart.o : periph_mini_uart.cpp ${PORTING_DIR}/Include/bcm_registers.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<

periph_timers.o : periph_timers.cpp ${PORTING_DIR}/Include/bcm_registers.hpp ${PORTING_DIR}/Include/gpio.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<

periph_misc.o : periph_misc.cpp ${PORTING_DIR}/Include/bcm_registers.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<
	
bcm_registers.o : ${PORTING_DIR}/bcm_registers.cpp ${PORTING_DIR}/Include/bcm_registers.hpp ${PORTING_DIR}/Include/gpio.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<

arm_debug_tools.o : ${PORTING_DIR}/arm_debug_tools.cpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -DRPI2 -c $<

Hex2BinEFSMachine.o : ${HEX2BIN_DIR}/Hex2BinEFSMachine.cpp ${HEX2BIN_DIR}/Include/hex2bin.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -c $<

formatter.o : ${DISPLAY_DIR}/formatter.cpp ${DISPLAY_DIR}/Include/formatter.hpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -c $<

arm_cpu.o : ${PORTING_DIR}/arm_cpu.s
	@echo GCC-ARM compiles assembler file $<
	@${EXEC}-as $< -o arm_cpu.o

clean : 
	rm -f *.o *.bin *.elf *.lst *.hex *.img
