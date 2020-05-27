src = miniglut.c test.c
obj = $(src:.c=.o)
bin = test

CFLAGS = -pedantic -Wall -O3

sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = $(src:.c=.w32.o)
	bin = test.exe

	LDFLAGS = -mconsole -lopengl32 -lgdi32 -lwinmm
else
	LDFLAGS = -lX11 -lGL
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
