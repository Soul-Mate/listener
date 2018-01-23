ROOT=..
PLATFORM=linux
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS =	test_match_h
all:	$(PROGS)

%:	%.c $(LIBS)
	$(CC) $(CFLAGS) ../libs/*.o $@.c -o $@ $(LDFLAGS) $(LDLIBS)
clean:
	rm -f $(PROGS) $(TEMPFILES) *.o
	rm -f *.txt
	(cd $(ROOT)/libs && $(MAKE) clean)

include ../Make.libs.inc