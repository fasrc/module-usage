CC=gcc
CFLAGS=-g -I/usr/include/mysql
LIBS=-L/usr/lib64/mysql -lmysqlclient -lm
RM=/bin/rm

EXECS = module_db
OBJS = module_db.o 

all: $(EXECS)

$(EXECS): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $(EXECS) $(OBJS) $(LIBS)

$(OBJS): Makefile

.c.o:
	$(CC) $(CFLAGS) -o $*.o -c $*.c
clean:
	$(RM) -f $(EXECS) $(OBJS) gmon.out *~ *.o *.oo

