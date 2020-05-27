obj = test.obj miniglut.obj
bin = test.exe

CC = wcc386
LD = wlink
CFLAGS = -d3 -6r -otexan -zq -bt=nt
LDFLAGS = system nt library { opengl32 winmm }

$(bin): $(obj)
	$(LD) name $@ debug all file { $(obj) } $(LDFLAGS)

.c.obj: .autodepend
	$(CC) -fo=$@ $(CFLAGS) $[*

!ifdef __UNIX__
clean: .symbolic
	rm -f $(obj) $(bin)
!else
clean: .symbolic
	del $(obj)
	del $(bin)
!endif
