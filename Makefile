########################################################################
# Makefile for NAL
########################################################################

# Libraries to link with
LIBS = -lenet

# Path to NAL Sources
NALPATH=SRC/NAL/

# The NAL Sources
NALSOURCES=$(NALPATH)md5.cpp $(NALPATH)nal_base.cpp $(NALPATH)nal_enet.cpp $(NALPATH)nal_host.cpp $(NALPATH)nal_packet.cpp $(NALPATH)nal_packetQueue.cpp $(NALPATH)nal_system.cpp $(NALPATH)nal_timer.cpp

# Sources
SOURCES=$(NALSOURCES)

# Compiler flags
CFLAGS=-c -Wall 

# Linker flags
LDFLAGS=

# Executable
EXECUTABLE=NAL

# Library 
LIBRARY=libnal.a

########################################################################
# It should not be needed to change anything below this line
########################################################################

# Objects
OBJECTS=$(SOURCES:.cpp=.o)

# Compiler
CC=g++

#AR
AR=ar

########################################################################
		
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

library: $(SOURCES)
	$(CC) -c $(SOURCES)
	$(AR) rvu $(LIBRARY) $(OBJECTS)
	ranlib $(LIBRARY)

all: $(SOURCES) $(EXECUTABLE)
	
clean:
	rm -rf *o $(EXECUTABLE)
	
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
