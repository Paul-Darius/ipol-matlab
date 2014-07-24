OBJ	= io_tiff.o libdemosaicking.o  imgdiff.o libAuxiliary.o demosaickingIpol.o mosaic.o
BIN = demosaickingIpol imgdiff mosaic
LIBBIN=.


CXX=g++


hdrdir=-I/opt/local/include/ -I/usr/local/include/
libdir=-L/opt/local/lib/ -L/usr/local/lib/


COPT	= -O3 -funroll-loops -fomit-frame-pointer  -fno-tree-pre -falign-loops -ffast-math -ftree-vectorize
CXXFLAGS  +=  -g $(COPT)  -Weffc++  -pedantic -Wall -Wextra  -Wno-write-strings  -Wno-deprecated    $(hdrdir)
LDFLAGS +=  -g $(CXXFLAGS) $(libdir) -ltiff


LIBMX=io_tiff.o libAuxiliary.o libdemosaicking.o


default: $(OBJ)  $(BIN)


$(OBJ) : %.o : %.cpp 
	$(CXX) -c $(CXXFLAGS)   $< -o $@


$(BIN) : % : %.o  $(LIBMX)
	$(CXX) $(LDFLAGS)  -o $(LIBBIN)/$@  $^



.PHONY : clean
clean:
	$(RM) $(OBJ) ; cd $(LIBBIN); rm $(BIN)
