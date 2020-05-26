src = miniglut.c test.c
obj = $(src:.c=.o)
bin = test

CFLAGS = -pedantic -Wall -g -DMINIGLUT_USE_LIBC
LDFLAGS = -lX11 -lGL -lm

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
