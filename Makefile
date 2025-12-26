# Makefile for Card Game
# Written by:
#   Renato Fermi   <repiazza@gmail.com>
# in September 2025

CC          = gcc
GCC_TARGET  := $(shell $(CC) -dumpmachine)

ifeq ($(findstring mingw32,$(GCC_TARGET)),mingw32)
    _WIN32 = 1
endif

ifeq ($(findstring cygwin,$(GCC_TARGET)),cygwin)
    _WIN32 = 1
endif

ifndef _WIN32
    ifndef APPLE
      LINUX = 1
    endif
endif

-include build/config.mk

MSYS_PREFIX     ?= /mingw64
MSYS_WIN_PREFIX ?= D:/msys64/mingw64

#
# *** DYNAMIC VARIABLES FOR COMMAND PROMPTS ***
# *** ONLY do your personal config here     ***
#

# Set make prefix only for targeted compilation
TARGET_PREFIX =

# Set gcc prefix only for targeted compilation
CC_PREFIX =

ifneq ($(strip $(CC_PREFIX)),)
    CC = $(CC_PREFIX)gcc
endif

#
# Set prompt/terminal commands
#
RM_ALTER_CMD    =
MKDIR_ALTER_CMD =

RM_CMD    = rm -rf
MKDIR_CMD = mkdir -p

ifneq ($(strip $(RM_ALTER_CMD)),)
    RM_CMD = $(RM_ALTER_CMD)
endif

ifneq ($(strip $(MKDIR_ALTER_CMD)),)
    MKDIR_CMD = $(MKDIR_ALTER_CMD)
endif

SRC_PATH     = $(TARGET_PREFIX)src
INCLUDE_PATH = $(TARGET_PREFIX)include
OBJ_DIR      = $(TARGET_PREFIX)obj
BIN_DIR      = $(TARGET_PREFIX)bin
HTML_DIR     = $(TARGET_PREFIX)html
LATEX_DIR    = $(TARGET_PREFIX)latex
LOG_DIR      = $(TARGET_PREFIX)log

INC_DIR = -I$(INCLUDE_PATH)

SDL_ADD_LIBS =

ifdef LINUX
    INC_DIR += -I/usr/include/libxml2
endif
ifdef APPLE
    INC_DIR += -I/opt/homebrew/include -I /opt/homebrew/opt/libxml2/include/libxml2
endif
ifdef _WIN32
    INC_DIR += -I$(MSYS_PREFIX)/include/libxml2
endif

ifdef USE_SDL2
ifdef _WIN32
ifdef VCPKG_ROOT
    SDL_ADD_LIBS += -lmingw32 -L$(VCPKG_ROOT)/installed/x64-windows/lib
    INC_DIR      += -I$(VCPKG_ROOT)/installed/x64-windows/include
else
    SDL_ADD_LIBS += -lmingw32 -L$(MSYS_WIN_PREFIX)/lib
    INC_DIR      += -I$(MSYS_PREFIX)/include
endif
endif

ifdef APPLE
    SDL_ADD_LIBS += -L/opt/homebrew/lib -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
else
    SDL_ADD_LIBS += -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
endif
endif

LIBS  = -lxml2
CCOPT = -Wall -Wextra

ifdef _WIN32
    CCOPT += -D_WIN32
    LIBS  += $(SDL_ADD_LIBS) -D_WIN32
endif

ifdef LINUX
    CCOPT += -DLINUX
    LIBS  += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDL_ADD_LIBS) -DLINUX
endif

ifdef APPLE
  CCOPT += -Wno-main -DAPPLE
  LIBS +=  -L /opt/homebrew/lib -L /opt/homebrew/opt/libxml2/lib $(SDL_ADD_LIBS)
endif

ifdef USE_SDL2
    CCOPT += -DUSE_SDL2
endif

LIBS += -lm

# FAKE OPT
ifdef FAKE
    CCOPT += -DFAKE
endif

# DEBUG ADDON FLAGS
DEBUG_ADD_FLAGS =
ifdef DEBUG
    DEBUG_ADD_FLAGS = -g -ggdb -O0
else
    CCOPT += -O2
endif

SDL_OBJ =
ifdef USE_SDL2
SDL_OBJ = \
    $(OBJ_DIR)/sdl_api.o \
    $(OBJ_DIR)/sdl_animation.o \
    $(OBJ_DIR)/image.o \
    $(OBJ_DIR)/event_render.o \
    $(OBJ_DIR)/event.o
endif

CARD_GAME_EXEC = card_game

OBJS = \
    $(OBJ_DIR)/card_game.o \
    $(SDL_OBJ) \
    $(OBJ_DIR)/sys_interface.o \
    $(OBJ_DIR)/input.o \
    $(OBJ_DIR)/terminal_utils.o \
    $(OBJ_DIR)/deck.o \
    $(OBJ_DIR)/monster.o \
    $(OBJ_DIR)/player.o \
    $(OBJ_DIR)/battle.o \
    $(OBJ_DIR)/shop.o \
    $(OBJ_DIR)/dialog.o \
    $(OBJ_DIR)/console_api.o \
    $(OBJ_DIR)/trace.o \
    $(OBJ_DIR)/conf.o \
    $(OBJ_DIR)/game.o \
    $(OBJ_DIR)/xml.o \
    $(OBJ_DIR)/welcome.o \
    $(OBJ_DIR)/hud.o \
    $(OBJ_DIR)/rect.o

doc:
	@mkdir -p doc/doxygen
	@doxygen Doxyfile

all: clean directories $(BIN_DIR)/$(CARD_GAME_EXEC)

clean:
	@$(RM_CMD) $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR)

directories:
	@$(MKDIR_CMD) $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/$(CARD_GAME_EXEC): directories $(OBJS)
	$(CC) $(LDOPT) $(INC_DIR) -o $@ $(OBJS) $(LIBS) $(LDOPT)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@

distclean: clean

.PHONY: all clean distclean directories

# TEST AND COVERAGE TARGETS
TEST_CCOPT = $(CCOPT) --coverage -fprofile-arcs -ftest-coverage
TEST_LDOPT = --coverage -lgcov

test: CCOPT := $(TEST_CCOPT)
test: LDOPT := $(TEST_LDOPT)
test: clean directories $(BIN_DIR)/$(CARD_GAME_EXEC)
	@echo "Running tests..."
	@$(BIN_DIR)/$(CARD_GAME_EXEC) --test || true

coverage: test
	@echo "Generating coverage report..."
	@lcov --capture --directory $(OBJ_DIR) --output-file coverage.info
	@lcov --remove coverage.info '/usr/*' --output-file coverage.info
	@lcov --list coverage.info

coverage-html: coverage
	@echo "Generating HTML coverage report..."
	@genhtml coverage.info --output-directory coverage_html
	@echo "Coverage report generated in coverage_html/"

.PHONY: test coverage coverage-html

# DOCKER TARGETS
docker-build:
	docker build -t ccg:latest .

docker-run:
	docker run -it --rm ccg:latest

docker-run-interactive:
	docker run -it --rm --entrypoint /bin/sh ccg:latest

docker-test: docker-build docker-run

.PHONY: docker-build docker-run docker-run-interactive docker-test doc
