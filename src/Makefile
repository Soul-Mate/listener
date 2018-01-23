ROOT=..
#PLATFORM=$(shell $(ROOT)/systype.sh)
PLATFORM=linux
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS =	main
all:	$(PROGS)

%:	%.c $(LIBS)
	$(CC) $(CFLAGS) $(ROOT)/libs/*.o $@.c -g -o $@ $(LDFLAGS) $(LDLIBS)

include $(ROOT)/Make.libs.inc

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o
	(cd $(ROOT)/libs && $(MAKE) clean)
