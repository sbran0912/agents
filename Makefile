########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = gcc

# DEBUG or RELEASE
CXXFLAGS = -Wall -g -std=c17 -D_POSIX_C_SOURCE=199309L
# CXXFLAGS = -Wall -O2 -std=c17 -D_POSIX_C_SOURCE=199309L

LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt

# Makefile settings - Can be customized.
APPNAME = main.app
EXT = .c
SRCDIR = src
OBJDIR = obj

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm $(OBJDIR)/*.o
	rm $(APPNAME)

