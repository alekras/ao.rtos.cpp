# Makefile for Application library

.EXPORT_ALL_VARIABLES:
include ../../Makefile-common
include ../Makefile-common-port

APP_OBJ_DIR = ${GCC_DIR}/Application

OBJECTS = ${APP_OBJ_DIR}/periph_interrupt.o \
	${APP_OBJ_DIR}/MyAO.o \
	${APP_OBJ_DIR}/UartAO.o \
	${APP_OBJ_DIR}/GpioAO.o \
	${APP_OBJ_DIR}/ThermometerAO.o \
	${APP_OBJ_DIR}/DebugAOScheduler.o
#	${APP_OBJ_DIR}/main.o

all : prepare ${GCC_DIR}/libapp.a libkern libaortos libport
	@echo "   application.a :"
	@${EXEC}-ar t ${GCC_DIR}/libapp.a
	@echo "   kernel.a :"
	@${EXEC}-ar t ${GCC_DIR}/libkern.a
	@echo "   library.a :"
	@${EXEC}-ar t ${GCC_DIR}/libaortos.a
	@echo "   porting.a :"
	@${EXEC}-ar t ${GCC_DIR}/libport.a

${GCC_DIR}/libapp.a : ${OBJECTS}
	@echo make application.a 
	@${EXEC}-ar ruvs $@ $^

${APP_OBJ_DIR}/periph_interrupt.o : periph_interrupt.cpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${APP_OBJ_DIR}/MyAO.o : MyAO.cpp Include/MyAO.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${APP_OBJ_DIR}/UartAO.o : UartAO.cpp Include/UartAO.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${APP_OBJ_DIR}/GpioAO.o : GpioAO.cpp Include/GpioAO.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${APP_OBJ_DIR}/ThermometerAO.o : ThermometerAO.cpp Include/ThermometerAO.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${APP_OBJ_DIR}/DebugAOScheduler.o : DebugAOScheduler.cpp Include/DebugAOScheduler.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

${APP_OBJ_DIR}/main.o : main.cpp
	@echo compiles source file $<
	@${EXEC}-gcc ${COPTS} ${INCLUDE_PATHS} -o $@ -c $<

.PHONY: libkern
libkern :
	cd ../../../Kernel && ${MAKE}

.PHONY: libaortos
libaortos :
	cd ../../../Library && ${MAKE}

.PHONY: libport
libport :
	cd ../../../Porting/ARM-Raspberry_Pi && ${MAKE}

.PHONY: clean
clean :
	cd ../../../Kernel && ${MAKE} clean

prepare :
	@mkdir -p ${TARGET_DIR}
	@mkdir -p ${GCC_DIR}
	@mkdir -p ${LISTING_DIR}
	@mkdir -p ${APP_OBJ_DIR}
	@mkdir -p ${GCC_DIR}/Library
	@mkdir -p ${GCC_DIR}/Kernel
	@mkdir -p ${GCC_DIR}/Porting

