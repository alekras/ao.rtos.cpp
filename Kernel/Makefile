# Makefile for Kernel library

INCLUDE_PATHS = -I ${ROOT_DIR}/Kernel/Include \
	-I ${PORT_DIR}/Include \
	-I ${ROOT_DIR}/Library/Display/Include \
	-I ${PROJECT_DIR}/Include

KERNEL_OBJ_DIR = ${GCC_DIR}/Kernel

SOURCES = memory.cpp \
	ListenerList.cpp \
	RingBuffer.cpp \
	Process.cpp \
	AObject.cpp \
	ISAObject.cpp \
	Timer.cpp \
	AOScheduler.cpp

OBJECTS = ${KERNEL_OBJ_DIR}/memory.o \
	${KERNEL_OBJ_DIR}/ListenerList.o \
	${KERNEL_OBJ_DIR}/RingBuffer.o \
	${KERNEL_OBJ_DIR}/Process.o \
	${KERNEL_OBJ_DIR}/AObject.o \
	${KERNEL_OBJ_DIR}/ISAObject.o \
	${KERNEL_OBJ_DIR}/Timer.o \
	${KERNEL_OBJ_DIR}/AOScheduler.o

${GCC_DIR}/libkern.a : ${OBJECTS}
	@${EXEC}-ar rvs $@ $^

${KERNEL_OBJ_DIR}/memory.o : memory.cpp Include/memory.hpp
	@echo compiles source file $<
	@echo include path ${INCLUDE_PATHS}
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/ListenerList.o : ListenerList.cpp Include/ListenerList.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/RingBuffer.o : RingBuffer.cpp Include/RingBuffer.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/Process.o : Process.cpp Include/Process.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/AObject.o : AObject.cpp Include/AObject.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/ISAObject.o : ISAObject.cpp Include/ISAObject.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/Timer.o : Timer.cpp Include/Timer.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${KERNEL_OBJ_DIR}/AOScheduler.o : AOScheduler.cpp Include/AOScheduler.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

clean : 
	cd ${KERNEL_OBJ_DIR} && rm -f *.o *.bin *.elf *.lst *.hex *.img
	