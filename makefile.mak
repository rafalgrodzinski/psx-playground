CFLAGS = -I./
LDFLAGS = -lLIBHMD lib/mmgmnew.obj
OBJS = core.obj video.obj audio.obj pad.obj cd.obj sprite.obj model.obj

core.obj:
	ccpsx $(CFLAGS) -c core.c

video.obj:
	ccpsx $(CFLAGS) -c video.c

audio.obj:
	ccpsx $(CFLAGS) -c audio.c

pad.obj:
	ccpsx $(CFLAGS) -c pad.c

cd.obj:
	ccpsx $(CFLAGS) -c cd.c

sprite.obj:
	ccpsx $(CFLAGS) -c sprite.c

model.obj:
	ccpsx $(CFLAGS) -c model.c

test: $(OBJS)
	ccpsx $(LDFLAGS) $(CFLAGS) -Xo$80010000 *.obj test\main.c -o main.cpe
	cpe2x main.cpe
	mkpsxiso -y test\test.xml

test_run:
	psymake test
	No$psx test.bin

clean:
	del *.obj
	del *.cpe
	del *.exe
	del *.bin
	del *.cue
	del *.map
	del *.sys
