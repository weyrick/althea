#
# Althea Make File
#
#
#

top_srcdir = .
top_builddir = src
program_dir = src

PROGRAM=althea

OBJECTS=support.o main.o aobject.o socket.o server.o mapengine.o sector.o\
	sninterface.o String.o dbengine.o snclient.o eventengine.o worldengine.o\
	parser.o creature.o configreader.o

####################### GENERAL SETTINGS ############################


LDLIBS = -lmysqlclient

########################### Programs #################################

# C++ compiler
CC          = c++
CCPP        = c++ -E

# C compiler
CCC         = gcc
CCCPP       = gcc -E

# Compiler for lex/yacc .c programs
CCLEX       = gcc

LEX         = flex
YACC        = bison -y
AR          = ar
AS          = @AS@
NM          = @NM@
LN_S        = ln -s
STRIP       = strip
MAKE        = make
AROPTIONS   = ruv
RANLIB      = ranlib
LD          = @LD@
MAKEINFO    = @MAKEINFO@

SHARED_LD   = gcc -shared -o

########################### Flags #################################

CFLAGS   = -Wall -g
CPPFLAGS = -Wall -g
PICFLAGS = 
LDFLAGS  = -L/usr/lib/mysql
INCLUDES = -I$(top_srcdir)/include

# Make environment for making samples on Unix

# Clears all default suffixes
.SUFFIXES:	.o .cpp .c .cxx

# The binary program extension, if any, including dots (e.g. '.exe')
PROGRAM_EXT = 
BIN_PROGRAM = $(PROGRAM)$(PROGRAM_EXT)

.c.o :
	$(CCC) $(INCLUDES) -c $(CFLAGS) -o $@ $<

.cpp.o :
	$(CC) $(INCLUDES) -c $(CPPFLAGS) -o $@ $<

.cxx.o :
	$(CC) $(INCLUDES) -c $(CPPFLAGS) -o $@ $<

VPATH = :$(top_srcdir)/$(program_dir)


################################################################################


all:    $(BIN_PROGRAM)

$(BIN_PROGRAM):	$(OBJECTS) 
	$(CC) $(LDFLAGS) -o $(BIN_PROGRAM) $(OBJECTS) $(EXTRALIBS) $(LDLIBS)

clean:
	rm -f $(OBJECTS) $(BIN_PROGRAM) core

