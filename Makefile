#Compiler
CC = gcc

#Flags
CCLAGS = -Wall -std=c99 -g -Werror

LDFLAGS = -lm

#The final executable name
TARGET = myMonitoringTool

#Object files
OBJ = ${TARGET}.o parse_command_line.o utilities.o graph.o core.o memory.o cpu.o process.o signal_handler.o

HEADER = parse_command_line.h utilities.h graph.h core.h memory.h cpu.h process.h signal_handler.h

## all: Compile all files to executable myMonitoringTool
all: ${TARGET}

## myMonitoringTool: Link all relevant object files if they exist
${TARGET}: ${OBJ}
	@echo "->Linking object files ${OBJ} into $@..."
	${CC} ${CCLAGS} -o $@ $^ ${LDFLAGS}
	@echo "Executable file $@ successfully created."

#Compiles all .c files to .o files, do so even if changes are made to header files 
%.o: %.c ${HEADER}
	@echo "->Compiling $< into $@..."
	${CC} ${CCLAGS} -c $<
	@echo ""

## clean: Remove all compiled files
.PHONY: clean
clean:
	@echo "Cleaning up executable and object files..."
	rm -f ${TARGET} *.o
	@echo "Finish cleaning."

## help: Displays available targets to be used.
.PHONY: help
help: Makefile
	@echo "Commands in this Makefile:"
	@sed -n 's/^##//p' $<