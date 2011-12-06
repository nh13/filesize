CC=			gcc
CFLAGS=		-g -std=c99 -m64 -Wall -O2
CXXFLAGS=	$(CFLAGS)
DFLAGS=		-D_FILE_OFFSET_BITS=64
OBJS=		filesize.o
PROG=		filesize
INCLUDES=	
LIBS=		#-lm -lz -lpthread
SUBDIRS=	. 

.SUFFIXES:.c .o .cc

.c.o:
		$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@
.cc.o:
		$(CXX) -c $(CXXFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)

filesize:$(OBJS) 
		$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $@ $(LIBS)

clean:
		rm -f gmon.out *.o a.out $(PROG) *~ *.a
