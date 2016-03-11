MATLABDIR ?= /Applications/MATLAB_R2013a.app

CXX = g++
CFLAGS = -O3 -fPIC -D_REENTRANT -D_QT_NO_DEBUG -D_QT_CORE_LIB -fopenmp -I$(MATLABDIR)/extern/include -I/usr/include/qt5 -I/usr/include/qt5/QtCore -I./ffdbc/

LD_LIBRARY=/usr/lib:/usr/lib64:$(LD_LIBRARY_PATH)

LDFLAGS+="-fopenmp"
ifeq ($(MEX_EXT),mexmaci64)
	LDFLAGS+=" -bundle"
else
	LDFLAGS+=" -shared"
endif

ifeq ($(wildcard $(MATLABDIR)/bin/mexext),)
	MEX_EXT = 
else
	MEX_EXT = $(shell $(MATLABDIR)/bin/mexext)
endif

MEX = $(MATLABDIR)/bin/mex
MEX_OPTION = CC\=$(CXX) CXX\=$(CXX) CFLAGS\="$(CFLAGS) -fpermissive" CXXFLAGS\="$(CFLAGS) -std=c++11" LDFLAGS\=$(LDFLAGS) LD_LIBRARY_PATH\=$(LD_LIBRARY)
# comment the following line if you use MATLAB on 32-bit computer
MEX_OPTION += -largeArrayDims

all: matlab

matlab:	ffdbcClustering.$(MEX_EXT)

ffdbcClustering.$(MEX_EXT): ffdbcClustering.cpp ffdbc/FFDBC.cpp
	$(MEX) $(MEX_OPTION) -lQt5Core -lpthread -L/usr/lib/x86_64-linux-gnu ffdbcClustering.cpp ffdbc/FFDBC.cpp

clean:
	rm -f *~ *.o *.mex* *.obj libsvm/*.o libsvm/*.mex* openni/*.mex* ffdbc/*.o ffdbc/*.mex

