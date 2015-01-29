bin_PROGRAMS = butterfly

NUL =

butterfly_SOURCES = \
	cloud.c \
	cloud.h \
	config.h \
	display.c \
	display.h \
	display_init.c \
	display_init.h \
	ebullet.c \
	ebullet.h \
	eclass.c \
	enemy.c \
	enemy.h \
	game.c \
	game.h \
	globvars.h \
	input.c \
	input.h \
	level.c \
	level.h \
	main.c \
	menu.c \
	menu.h \
	palette.c \
	palette.h \
	pbullet.c \
	pbullet.h \
	sound.c \
	sound.h \
	stuff.c \
	stuff.h \
	$(NUL)
#
# This is telling the program where to find datafiles during compilation. Define the datafile directory name in configure.ac if you don't like it
if WINDOWS_VERSION
#no datadir #defined
butterfly_SOURCES += butterfly.rc
butterfly_LDFLAGS = -static
else #unix version :-)
AM_CPPFLAGS = -D'DATADIR="@DATADIR_NAME@"'
endif

#how to build resources?
.rc.o:
	$(RC) $(RCFLAGS) $< -o $@ 
