
ARM-TOOLS-PATH = /opt/gcc-arm/latest
EXEC = ${ARM-TOOLS-PATH}/bin/arm-none-eabi
CPP-LIBS = ${ARM-TOOLS-PATH}/lib/gcc/arm-none-eabi/10.3.1
COPTS = -Wno-write-strings -O2 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -nostdlib -nostartfiles -ffreestanding -fno-exceptions
SOURCES = main.cpp \
	InitialInteractiveLoaderSM.cpp \
	periph.cpp \
	../../../Library/hex2bin/Hex2BinEFSMachine.cpp \
	../../../Library/Display/formatter.cpp
# arm_cpu.o has to be first in linker list
OBJECTS = arm_cpu.o \
	main.o \
	periph.o \
	Hex2BinEFSMachine.o \
	InitialInteractiveLoaderSM.o \
	formatter.o

img : ld-image
	@echo GCC-ARM creates binary image file from elf file
	@${EXEC}-objcopy InitialInteractiveLoaderSM.elf -O binary kernel.img

hex : ld-app
	@echo GCC-ARM creates HEX file from elf file
	@${EXEC}-objcopy InitialInteractiveLoaderSM.elf -O ihex InitialInteractiveLoaderSM.hex

ld-image : ${OBJECTS}
	@echo  GCC-ARM links $^ files
	@${EXEC}-ld -L${CPP-LIBS} $^ -lgcc -Map map.lst -T loader -o InitialInteractiveLoaderSM.elf
	@${EXEC}-objdump -D InitialInteractiveLoaderSM.elf > InitialInteractiveLoaderSM.lst

ld-app : ${OBJECTS}
	@echo GCC-ARM links $^ files
	@${EXEC}-ld -L${CPP-LIBS} $^ -lgcc -Map map.lst -T loader-app -o InitialInteractiveLoaderSM.elf
	@${EXEC}-objdump -D InitialInteractiveLoaderSM.elf > InitialInteractiveLoaderSM.lst

#objects :: ${SOURCES}
#	@echo GCC-ARM compiles source files $^
#	${EXEC}-gcc ${COPTS} -DRPI2 -c ${SOURCES}

main.o : main.cpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} -DRPI2 -c $<

InitialInteractiveLoaderSM.o : InitialInteractiveLoaderSM.cpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} -DRPI2 -c $<

periph.o : periph.cpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} -DRPI2 -c $<

Hex2BinEFSMachine.o : ../../../Library/hex2bin/Hex2BinEFSMachine.cpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} -DRPI2 -c $<

formatter.o : ../../../Library/Display/formatter.cpp
	@echo GCC-ARM compiles source file $<
	@${EXEC}-gcc ${COPTS} -DRPI2 -c $<

#objects :: ../../../Porting/ARM-Raspberry_Pi/arm_cpu.s
#	@echo GCC-ARM compiles assembler files $<
#	${EXEC}-as $< -o arm_cpu.o

arm_cpu.o : ../../../Porting/ARM-Raspberry_Pi/arm_cpu.s
	@echo GCC-ARM compiles assembler file $<
	@${EXEC}-as $< -o arm_cpu.o

clean : 
	rm -f *.o *.bin *.elf *.lst *.hex *.img