# Makefile
CFLAGS += -I./curl/include
LDFLAGS += -L./curl/lib -lcurl

hi: hi.o functions.o utils.o
	cc -o hi hi.o functions.o utils.o $(LDFLAGS)

hi.o: hi.c functions.h utils.h
	cc -c hi.c

functions.o: functions.c utils.h
	cc -c functions.c

utils.o: utils.c
	cc -c utils.c $(CFLAGS)

.PHONY: clean
clean: 
	rm hi hi.o functions.o utils.o
