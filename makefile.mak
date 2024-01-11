CFLAGS =
LDFLAGS = -lLIBHMD lib/mmgmnew.obj
OBJS = core.obj video.obj audio.obj pad.obj cd.obj sprite.obj model.obj

all: $(OBJS)
	ccpsx $(LDFLAGS) $(CFLAGS) -Xo$80010000 *.obj main.c -o main.cpe
	cpe2x main.cpe
	mkpsxiso -y test.xml

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

run:
	psymake
	No$psx test.bin

clean:
	del *.obj
	del *.cpe
	del *.exe
	del *.bin
	del *.cue
	del *.map
	del *.sys
