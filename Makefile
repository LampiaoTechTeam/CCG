# Makefile for Card Game
# Written by:
#   Renato Fermi   <repiazza@gmail.com>
# in September 2025

CC            = gcc
GCC_TARGET    := $(shell $(CC) -dumpmachine)

ifeq ($(findstring mingw32, $(GCC_TARGET)), mingw32)
  _WIN32 = 1
endif
ifeq ($(findstring cygwin, $(GCC_TARGET)), cygwin)
  _WIN32 = 1
endif

ifndef _WIN32
	#define LINUX
endif

#
# *** DYNAMIC VARIABLES FOR COMMAND PROMPTS ***
# *** ONLY do your personal config here     ***
#

#
# Set make prefix only 4 targeted compilation.
# If already "pathed" make, in other words, if you CAN use make from current directory:
# -- Just leave it empty!
# Dont forget the last slash or backslash 
#
# In case of using, DONT forget the last slash or backslash E.g.: ROOT_PATH/
# 
# E.g.: PREFIX = D:/cardgame/
TARGET_PREFIX = 

#
# Set gcc prefix only 4 targeted compilation.
# If already "pathed" gcc, in other words, if you CAN use gcc from current directory:
# -- Just leave it empty!
#
# In case of using, DONT forget the last slash or backslash E.g.: ROOT_PATH/
# 
# E.g.: CC_PREFIX = D:/msys64/mingw64/bin/
CC_PREFIX = ""

# Don't write slashes or backslashes here. 
ifneq (${CC_PREFIX}, "")
	CC = ${CC_PREFIX}gcc
endif

#
# Set prompt/terminal commands
# If  clean and directory phase is working well:
#
# -- Just leave it empty!
RM_ALTER_CMD    = ""
MKDIR_ALTER_CMD = ""

RM_CMD = rm -rf
MKDIR_CMD = mkdir -p

ifneq (${RM_ALTER_CMD}, "") 
	RM_CMD = ${RM_ALTER_CMD}
endif

ifneq (${RM_ALTER_CMD}, "") 
	MKDIR_CMD = ${MKDIR_ALTER_CMD}
endif

LIBS          = 
SRC_PATH      = ${TARGET_PREFIX}src
INCLUDE_PATH  = ${TARGET_PREFIX}include
OBJ_DIR       = ${TARGET_PREFIX}obj
BIN_DIR       = ${TARGET_PREFIX}bin
HTML_DIR      = ${TARGET_PREFIX}html
LATEX_DIR     = ${TARGET_PREFIX}latex
LOG_DIR       = ${TARGET_PREFIX}log
INC_DIR       = -I$(INCLUDE_PATH)
CCOPT         = -Wall -Wextra

ifdef _WIN32
  CCOPT += -D_WIN32
endif

ifdef LINUX
  CCOPT += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -DLINUX
  LIBS  += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -DLINUX 
endif

DEBUG_ADD_FLAGS = -O2
ifdef DEBUG
  DEBUG_ADD_FLAGS = -g -ggdb
endif

ifdef FAKE
  CCOPT += -DFAKE
endif

CARD_GAME_EXEC = card_game

OBJS = \
	$(OBJ_DIR)/card_game.o \
	$(OBJ_DIR)/sys_interface.o \
	$(OBJ_DIR)/input.o \
	$(OBJ_DIR)/terminal_utils.o \
	$(OBJ_DIR)/deck.o \
	$(OBJ_DIR)/monster.o \
	$(OBJ_DIR)/player.o \
	$(OBJ_DIR)/battle.o \
	$(OBJ_DIR)/shop.o \
	$(OBJ_DIR)/trace.o 

all: clean directories $(BIN_DIR)/$(CARD_GAME_EXEC)

clean:
	@${RM_CMD} $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR)

directories:
	@${MKDIR_CMD} $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/$(CARD_GAME_EXEC): directories $(OBJS)
	$(CC) $(LDOPT) $(INC_DIR) -o $@ $(OBJS) $(LIBS) $(LDOPT)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@

distclean: clean

.PHONY: all clean distclean directories 

# DOCKER TARGETS
docker-build:
	docker build -t ccg:latest .

docker-run:
	docker run -it --rm ccg:latest

docker-run-interactive:
	docker run -it --rm --entrypoint /bin/sh ccg:latest

docker-test: docker-build docker-run

.PHONY: docker-build docker-run docker-run-interactive docker-test
