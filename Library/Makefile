# Makefile for Library library

SOURCES = HSM/Path.cpp \
	HSM/State.cpp \
	HSM/StateMachine.cpp \
	Display/display.cpp \
	Display/formatter.cpp \
	FSM/fsm.cpp

LIBRARY_OBJ_DIR = ${GCC_DIR}/Library

OBJECTS = ${LIBRARY_OBJ_DIR}/Path.o \
	${LIBRARY_OBJ_DIR}/State.o \
	${LIBRARY_OBJ_DIR}/StateMachine.o \
	${LIBRARY_OBJ_DIR}/display.o \
	${LIBRARY_OBJ_DIR}/formatter.o \
	${LIBRARY_OBJ_DIR}/fsm.o

${GCC_DIR}/libaortos.a : ${OBJECTS}
	@${EXEC}-ar rvs $@ $^

${LIBRARY_OBJ_DIR}/Path.o : HSM/Path.cpp HSM/Include/Path.h HSM/Include/State.h
	@echo compiles source file $<
	@${EXEC}-gcc ${CPPOPTS} ${INCLUDE_PATHS} -o $@ -c $<

${LIBRARY_OBJ_DIR}/State.o : HSM/State.cpp HSM/Include/State.h HSM/Include/Hsm.h
	@echo compiles source file $<
	@${EXEC}-gcc ${CPPOPTS} ${INCLUDE_PATHS} -o $@ -c $<

${LIBRARY_OBJ_DIR}/StateMachine.o : HSM/StateMachine.cpp HSM/Include/Hsm.h
	@echo compiles source file $<
	@${EXEC}-gcc ${CPPOPTS} ${INCLUDE_PATHS} -o $@ -c $<

${LIBRARY_OBJ_DIR}/display.o : Display/display.cpp Display/Include/display.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${CPPOPTS} ${INCLUDE_PATHS} -o $@ -c $<

${LIBRARY_OBJ_DIR}/formatter.o : Display/formatter.cpp Display/Include/formatter.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${CPPOPTS} ${INCLUDE_PATHS} -o $@ -c $<

${LIBRARY_OBJ_DIR}/fsm.o : FSM/fsm.cpp FSM/Include/fsm.hpp
	@echo compiles source file $<
	@${EXEC}-gcc ${CPPOPTS} ${INCLUDE_PATHS} -o $@ -c $<

clean : 
	cd ${LIBRARY_OBJ_DIR} && rm -f *.o *.bin *.elf *.lst *.hex *.img
	