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
# Root Directories
SRC_ROOT   = source/
TESTS_ROOT = tests/

# File Extensions
SRC_EXT    = cpp
TEST_EXT   = cpp

# Libraries to Link Against
LIBS      = deps/parse-utils/libparseutils.a
TEST_LIBS = $(LIBS) \
            tools/UnitTest++/libUnitTest++.a

# Source File Lists
SRC_FILES  = $(call flist, $(SRC_ROOT), $(SRC_EXT))
TEST_FILES = $(call flist, $(TESTS_ROOT), $(TEST_EXT))

# Object File Lists
SRC_OBJS  = $(SRC_FILES:%.$(SRC_EXT)=%.o)
TEST_OBJS = $(TEST_FILES:%.$(TEST_EXT)=%.o)

# Include Directories
INC_DIRS = $(call incdirs, $(SRC_ROOT)) \
           $(call incdirs, deps/parse-utils/source) \
           $(call incdirs, tools/UnitTest++/src)

# Compiler and Linker Options
#----------------------------
CXXFLAGS      = -c $(INC_DIRS) -Wall -Werror
TEST_CXXFLAGS = -c $(INC_DIRS) -Wall

# Build Rules
#------------
all: release test

release: $(PROJ_NAME)

test: $(TEST_RUNNER)
	$(TEST_RUNNER)

# Binaries
$(PROJ_NAME): parseutils $(SRC_OBJS)
	$(CXX) -o $@ $(SRC_OBJS) $(LIBS)

$(TEST_RUNNER): parseutils unit_test_pp $(SRC_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $(filter-out source/main.o,$(SRC_OBJS)) $(TEST_OBJS) $(TEST_LIBS)

# Libraries
parseutils:
	$(MAKE) -C deps/parse-utils static

unit_test_pp:
	$(MAKE) -C tools/UnitTest++

# Object Files
$(SRC_OBJS): %.o : %.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(TEST_OBJS): %.o : %.$(TEST_EXT)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $<

# Cleanup
clean:
	$(MAKE) -C deps/parse-utils clean
	$(MAKE) -C tools/UnitTest++ clean
	$(RM) $(SRC_OBJS)
	$(RM) $(TEST_OBJS)
	$(RM) $(TEST_RUNNER)*
	$(RM) $(PROJ_NAME)*

