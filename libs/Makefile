#
# Makefile for misc library.
#
ROOT=..
PLATFORM=linux
include $(ROOT)/Make.defines.$(PLATFORM)

LIBMISC	= libs.a
OBJS = err.o  himysql.o hashmap.o inotify.o cache.o cJSON.o log.o app.o match.o league.o market.o

all:	$(LIBMISC)

$(LIBMISC):	$(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)


clean:
	rm -f *.o a.out core temp.* $(LIBMISC)