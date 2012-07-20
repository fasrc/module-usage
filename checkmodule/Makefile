CC=gcc
CFLAGS=-g -I/usr/include/mysql
LIBS=-L/usr/lib64/mysql -lmysqlclient -lm
RM=/bin/rm

EXECS = checkmodule
OBJS = checkmodule.o commandline.o

all: $(EXECS)

$(EXECS): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $(EXECS) $(OBJS) $(LIBS)

$(OBJS): Makefile

.c.o:
	$(CC) $(CFLAGS) -o $*.o -c $*.c
clean:
	$(RM) -f $(EXECS) $(OBJS) gmon.out *~ *.o *.oo

