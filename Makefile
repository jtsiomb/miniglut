src = miniglut.c test.c
obj = $(src:.c=.o)
bin = test

CFLAGS = -pedantic -Wall -g

isx86 ?= $(shell uname -m | sed 's/x86_64/x86/; s/i.86/x86/')

sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = $(src:.c=.w32.o)
	bin = test.exe

	LDFLAGS = -mconsole -lopengl32 -lgdi32 -lwinmm
else ifeq ($(sys)-$(isx86), Linux-x86)
	LDFLAGS = -lX11 -lGL
else
	# for other UNIX or non-x86 where sys_ and trig functions are not
	# implemented, just use libc
	CFLAGS += -DMINIGLUT_USE_LIBC
	LDFLAGS = -lX11 -lGL -lm
	ifeq ($(sys), IRIX)
		CC = gcc
	endif
endif

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(bin)

.PHONY: cross
cross:
	$(MAKE) CC=i686-w64-mingw32-gcc sys=mingw

.PHONY: cross-clean
cross-clean:
	$(MAKE) CC=i686-w64-mingw32-gcc sys=mingw clean
