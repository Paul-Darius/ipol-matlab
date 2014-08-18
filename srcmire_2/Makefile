# C source code
CSRC	= io_png/io_png.c \

# C++ source code
CXXSRC	= MIRE.cpp \
	borders.cpp \
		demo_MIRE.cpp
# all source code
SRC	= $(CSRC) $(CXXSRC)

# C objects
COBJ	= $(CSRC:.c=.o)
# C++ objects
CXXOBJ	= $(CXXSRC:.cpp=.o)
# all objects
OBJ	= $(COBJ) $(CXXOBJ)
# binary target
BIN	= demo_MIRE

default	: $(BIN)

# C optimization flags
COPT	= -O3 -ftree-vectorize -funroll-loops

# C++ optimization flags
CXXOPT	= $(COPT)

# C compilation flags
CFLAGS	= $(COPT) -Wall -Wextra \
	-Wno-write-strings -ansi -fpermissive
# C++ compilation flags
CXXFLAGS	= $(CXXOPT) -Wall -Wextra \
	-Wno-write-strings -Wno-deprecated -ansi 
# link flags
LDFLAGS	= -lpng -lm



# use openMP with `make OMP=1`
ifdef OMP
CFLAGS	+= -fopenmp
CXXFLAGS	+= -fopenmp
LDFLAGS += -lgomp
else
CFLAGS	+= -Wno-unknown-pragmas
CXXFLAGS  += -Wno-unknown-pragmas
endif


# use DEBUG
ifdef DEBUG
CFLAGS	+= -g
CXXFLAGS	+= -g
LDFLAGS += -g
endif
# build the local png library
.PHONY	: libpng
libpng	:
	$(MAKE) -C io_png/libs libpng

# partial compilation of C source code
%.o: %.c %.h
	$(CC) -c -o $@  $< $(CFLAGS)
# partial compilation of C++ source code
%.o: %.cpp %.h
	$(CXX) -c -o $@  $< $(CXXFLAGS)

# link all the opject code
$(BIN): $(OBJ) $(LIBDEPS)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# housekeeping
.PHONY	: clean distclean
clean	:
	$(RM) $(OBJ)
	$(MAKE) -C ./io_png/libs $@
distclean	: clean
	$(RM) $(BIN)
	$(MAKE) -C ./io_png/libs $@
