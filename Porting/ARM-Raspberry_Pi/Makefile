# Makefile for Porting library

SOURCES = display-start.cpp \
	rpi-mailbox.c \
	rpi-mailbox-interface.c \
	bcm_registers.cpp \
	arm_cpu.s 

PORTING_OBJ_DIR = ${GCC_DIR}/Porting

OBJECTS = ${PORTING_OBJ_DIR}/arm_debug_tools.o \
	${PORTING_OBJ_DIR}/display-start.o \
	${PORTING_OBJ_DIR}/rpi-mailbox.o \
	${PORTING_OBJ_DIR}/rpi-mailbox-interface.o \
	${PORTING_OBJ_DIR}/bcm_registers.o \
	${PORTING_OBJ_DIR}/arm_cpu.o

${GCC_DIR}/libport.a : ${OBJECTS}
	@${EXEC}-ar rvs $@ $^
	@${EXEC}-ar d $@ arm_cpu.o

${PORTING_OBJ_DIR}/arm_debug_tools.o : arm_debug_tools.cpp Include/arm_debug_tools.hpp
	@echo compiles source file $<
	@echo include path ${INCLUDE_PATHS}
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${PORTING_OBJ_DIR}/display-start.o : display-start.cpp Include/display_hw.hpp Include/MonospaceDefault.hpp Include/rpi-mailbox-interface.h
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${PORTING_OBJ_DIR}/rpi-mailbox.o : rpi-mailbox.c Include/rpi-mailbox.h
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${PORTING_OBJ_DIR}/rpi-mailbox-interface.o : rpi-mailbox-interface.c Include/rpi-mailbox.h Include/rpi-mailbox-interface.h
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${PORTING_OBJ_DIR}/bcm_registers.o : bcm_registers.cpp Include/gpio.hpp Include/bcm_registers.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} -D${RPI_VERSION} ${INCLUDE_PATHS} -o $@ -c $<

${PORTING_OBJ_DIR}/arm_cpu.o : arm_cpu.s
	@echo compiles assembler file $<
	@${EXEC}-as $< -o $@

clean : 
	cd ${PORTING_OBJ_DIR} && rm -f *.o *.bin *.elf *.lst *.hex *.img
	