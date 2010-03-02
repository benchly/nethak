CC      = gcc
PROF    = 
NOCRYPT =
#Uncomment the next line if you want request support
#DBUGFLG = -DREQUESTS
C_FLAGS = -g3 -Wall $(PROF) $(NOCRYPT) $(DBUGFLG)
L_FLAGS = $(PROF)

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o boards.o \
	  build.o clans.o comm.o const.o db.o fight.o \
	  handler.o hashstr.o id.o interp.o magic.o makeobjs.o \
	  misc.o mud_comm.o mud_prog.o player.o requests.o \
	  reset.o save.o shops.o skills.o special.o tables.o track.o update.o \
	  space.o morespace.o swskills.o planets.o vote.o gboard.o webwho.o \
	  webhelp.o cyberspace.o mpxset.o newarena.o realmail.o cyberapps.o \
	  alias.o ackmapper.o

C_FILES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c boards.c \
	  build.c clans.c comm.c const.c db.c fight.c \
	  handler.c hashstr.c id.c interp.c magic.c makeobjs.c \
	  misc.c mud_comm.c mud_prog.c player.c requests.c \
	  reset.c save.c shops.c skills.c special.c tables.c track.c update.c \
	  space.c morespace.c swskills.c planets.c vote.c gboard.c webwho.c \
	  webhelp.c cyberspace.c mpxset.c newarena.c realmail.c cyberapps.c \
	  alias.c ackmapper.c

H_FILES = mud.h bet.h gboard.h alias.h ackmapper.h

all:
#	co $(H_FILES)
	make ../../neuro/bin/swr
#	rm -f $(H_FILES)

../../neuro/bin/swr: $(O_FILES)
	rm -f ../../neuro/bin/swr
	$(CC) $(L_FLAGS) -o ../../neuro/bin/swr $(O_FILES) -lm     -lcrypt
	chmod g+w ../../neuro/bin/swr
	chmod g+w $(O_FILES)

.c.o: mud.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f $(O_FILES)
