all: video.obj audio.obj pad.obj cd.obj sprite.obj model.obj
	ccpsx -Xo$80010000 *.obj lib/mmgmnew.obj main.c -o main.cpe
	cpe2x main.cpe
	mkpsxiso -y test.xml

video.obj:
	ccpsx -c video.c

audio.obj:
	ccpsx -c audio.c

pad.obj:
	ccpsx -c pad.c

cd.obj:
	ccpsx -c cd.c

sprite.obj:
	ccpsx -c sprite.c

model.obj:
	ccpsx -c model.c

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
