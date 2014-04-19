

CC=g++
DEBUG = -g
CFLAGS= -c -fopenmp -Wall $(DEBUG) 
ODIR=obj
LDIR =../lib
LIBS=-lm -lgomp -lfftw3 -lfftw3_omp
LFLAGS = -Wall $(DEBUG)

DEPS = AutoCorl.h Configuration.h fft2d.h IOAgent.h PerfMon.h Reflectance.h SpatialAvg.h Supervisor.h

_OBJ = AutoCorl.o Configuration.o fft2d.o IOAgent.o PerfMon.o Reflectance.o SpatialAvg.o Supervisor.o PCAkern.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp 
	$(CC) -o $@ $< $(CFLAGS)

PCAkern: $(OBJ)
	$(CC) -o $(ODIR)/$@ $^ $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
