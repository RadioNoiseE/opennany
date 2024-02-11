CC=cc
LDFLAGS=
LIBS= -lexpat
p: p.c
	$(CC) $(LDFLAGS) -o anyconnect p.c $(LIBS)
