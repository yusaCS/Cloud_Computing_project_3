# Compiler and compiler flags
CC = g++
CFLAGS = -Wall -Werror -pthread

# Source files
SRCS = main.cpp Request.cpp WebServer.cpp LoadBalancer.cpp
# Object files (generated from source files)
OBJS = main.o Request.o WebServer.o LoadBalancer.o

# Executable name
TARGET = loadbalancer_simulation

# Doxygen command
DOXYGEN = doxygen

# Default target, builds the program
all: $(TARGET)

# Rule to build the main target (executable)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rules to build each object file from the corresponding source file
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Request.o: Request.cpp Request.h
	$(CC) $(CFLAGS) -c Request.cpp

WebServer.o: WebServer.cpp WebServer.h
	$(CC) $(CFLAGS) -c WebServer.cpp

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h
	$(CC) $(CFLAGS) -c LoadBalancer.cpp

# Clean target to remove the generated files
clean:
	rm -f $(TARGET) *.o

# Doxygen target to generate documentation
docs:
	$(DOXYGEN) Doxyfile

# Phony targets to ensure make doesn't confuse them with actual files
.PHONY: all clean docs
