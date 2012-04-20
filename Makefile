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
PROJ_NAME   = dlang
TEST_RUNNER = test_runner

# File and Directory Settings
#----------------------------
SRC_ROOT   = source/
RES_ROOT   = res/
TESTS_ROOT = tests/
SRC_EXT    = cpp
RES_EXT    = scm
TEST_EXT   = cpp
SRC_DIRS   = $(call dlist, $(SRC_ROOT))
SRC_FILES  = $(call flist, $(SRC_ROOT), $(SRC_EXT))
RES_FILES  = $(call flist, $(RES_ROOT), $(RES_EXT))
TEST_FILES = $(call flist, $(TESTS_ROOT), $(TEST_EXT))

# Object File List
SRC_OBJS  = $(SRC_FILES:%.$(SRC_EXT)=%.o)
RES_OBJS  = $(RES_FILES:%.$(RES_EXT)=%.o)
TEST_OBJS = $(TEST_FILES:%.$(TEST_EXT)=%.o)

# Include Directories
INC_DIRS = $(addprefix -I,$(SRC_DIRS)) \
           $(call incdirs, deps/parse-utils/source) \
           $(call incdirs, tools/UnitTest++/src)

# Libraries to Link Against
LIBS      = deps/parse-utils/libparseutils.a
TEST_LIBS = tools/UnitTest++/libUnitTest++.a \
            $(LIBS)

# Compiler and Linker Options
#----------------------------
CXXFLAGS = $(INC_DIRS) -Wall -Werror

# Build Rules
#------------
all: $(PROJ_NAME) test

test: $(TEST_RUNNER)
	$(TEST_RUNNER)

# Binaries
$(TEST_RUNNER): unit_test_pp parseutils $(SRC_OBJS) $(RES_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $(TEST_OBJS) $(filter-out source/main.o,$(SRC_OBJS)) $(RES_OBJS) $(TEST_LIBS)

$(PROJ_NAME): parseutils $(SRC_OBJS) $(RES_OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $(SRC_OBJS) $(RES_OBJS) $(LIBS)

# Libraries
cork:
	$(MAKE) -C deps/cork static

parseutils:
	$(MAKE) -C deps/parse-utils static

unit_test_pp:
	$(MAKE) -C tools/UnitTest++

# Resources (Compiled Text Files)
# Object Files
$(SRC_OBJS): %.o : %.$(SRC_EXT)
$(TEST_OBJS): %.o : %.$(TEST_EXT)
$(RES_OBJS): %.o : %.$(RES_EXT)
	objcopy -I binary -O elf32-i386 -B i386 $< $@

# Clean Task
clean:
	-@$(MAKE) -C deps/cork clean
	-@$(MAKE) -C deps/parse-utils clean
	-@$(MAKE) -C tools/UnitTest++ clean
	-@$(RM) $(SRC_OBJS)
	-@$(RM) $(RES_OBJS)
	-@$(RM) $(TEST_OBJS)
	-@$(RM) $(TEST_RUNNER)*
	-@$(RM) $(PROJ_NAME)*

