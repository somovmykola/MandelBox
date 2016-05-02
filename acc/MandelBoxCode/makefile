
CC       =  pgCC
CXX = pgCC
FLAGS    = -O3 -acc -fast -Minfo=accel -Minform=inform -ta=tesla:cc30#-fopenmp
CFLAGS   = $(FLAGS) 
CXXFLAGS = $(FLAGS) 
LDFLAGS  = -lm -acc -fast -ta=tesla:cc30 #-mp 

PROGRAM_NAME=mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)


clean:
	rm *.o $(PROGRAM_NAME) $(EXEEXT) *~
