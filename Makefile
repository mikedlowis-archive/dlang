###############################################################################
#
# Name:    DLang
# Type:    Application
# Author:  Mike Lowis
# License: BSD 2-Clause
#
###############################################################################

# Utility Function Definitions
#-----------------------------
# Function for generating an file list
flist = $(shell env find $(1) -name *.$(strip $(2)) -print)

# Function for generating an file list
dlist = $(shell env find $(1) -type d -print)

# Function for generating an include list
incdirs = $(addprefix -I, $(call dlist, $(1)))

# Project and Artifact Names
#---------------------------
PROJ_NAME = dlang

# File and Directory Settings
#----------------------------
SRC_ROOT  = source/
SRC_EXT   = cpp
SRC_DIRS  = $(call dlist, source/)
SRC_FILES = $(call flist, $(SRC_ROOT), $(SRC_EXT))

# Object File List
OBJ_FILES = $(SRC_FILES:%.$(SRC_EXT)=%.o)

# Include Directories
INC_DIRS = $(addprefix -I,$(SRC_DIRS)) \
           $(call incdirs, deps/parse-utils/source)

# Libraries to Link Against
LIBS = deps/parse-utils/libparseutils.a

# Compiler and Linker Options
#----------------------------
CXXFLAGS = $(INC_DIRS) -Wall -Werror

# Build Rules
#------------
all: $(PROJ_NAME)

$(PROJ_NAME): parseutils $(OBJ_FILES) res/environment.o
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJ_FILES) $(LIBS) res/environment.o

cork:
	$(MAKE) -C deps/cork static

parseutils:
	$(MAKE) -C deps/parse-utils static

res/environment.o: res/environment.scm
	objcopy -I binary -O elf32-i386 -B i386 $< $@

$(OBJ_FILES): %.o : %.$(SRC_EXT)

clean:
	$(MAKE) -C deps/cork clean
	$(MAKE) -C deps/parse-utils clean
	$(RM) $(foreach dir,$(SRC_DIRS), $(dir)/*.o)
	$(RM) $(PROJ_NAME)*

