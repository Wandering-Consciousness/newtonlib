VERSION=0.95.5
STRUCTURE=Export Attractors Geometry Triangulation KBDTree Miniball Linux
ROOT=$(CURDIR)/..

#
# Common names & version build settings
#
.DEFAULT_GOAL:= default
CEXE=example_c
EXE=example
LIB=Attract
DLIB=lib$(LIB).so
SLIB=lib$(LIB).a
DBGDIR=debug.$(VERSION)
RELDIR=release.$(VERSION)
OBJSUFFIX=obj

# comliller and linker settings:
#
CC=gcc
CXX=g++
DCFLAGS= -m64 -Wall -Wextra -std=c11 -fPIC -MMD
DCXXFLAGS= -m64 -Wall -Wextra -std=c++11 -fPIC -MMD
LDFLAGS= -m64 

# Sources:
#
STRUCTURE= Export Attractors Geometry Triangulation KBDTree Miniball Linux 
VPATH = $(CURDIR) $(ROOT) $(addprefix $(ROOT)/, $(STRUCTURE))
INCLUDE = $(addprefix -I, $(VPATH))
SOURCES= geometry.cpp geodesic.cpp triangulation.cpp probability.cpp mathutil.cpp scp.cpp spot.cpp kdTreeSingleThread.cpp kdTree.cpp attractors.cpp export.cpp
LIB_OBJECTS= $(SOURCES:.cpp=.o) 
ALL_OBJECTS= $(LIB_OBJECTS) $(CEXE).o $(EXE).o
DEPENDENCIES= $(ALL_OBJECTS:%.o=%.d)  

# Make sure MAKECMDGOALS is defined, so it doesn't cause an error itself
ifndef MAKECMDGOALS
MAKECMDGOALS = default
endif
# Debug build settings
ifeq "$(strip $(filter debug,$(MAKECMDGOALS)))" "debug"
BUILD_DIR= $(CURDIR)/$(DBGDIR)
CFLAGS= $(DCFLAGS) -ggdb3 -O0 -DDEBUG
CXXFLAGS= $(DCXXFLAGS) -ggdb3 -O0 -DDEBUG
else
# default/release use Release build settings
BUILD_DIR= $(CURDIR)/$(RELDIR)
CFLAGS= $(DCFLAGS) -O3 -DNDEBUG
CXXFLAGS= $(DCXXFLAGS) -O3 -DNDEBUG
endif

#separate target for lib
ifeq "$(strip $(filter lib,$(MAKECMDGOALS)))" "lib"
LIBTARGET= $(BUILD_DIR)/$(SLIB) $(BUILD_DIR)/$(DLIB)
else
LIBTARGET=
endif

#common target
LIBS= -L$(BUILD_DIR) -l$(LIB)
OBJ_DIR= $(BUILD_DIR)/$(OBJSUFFIX)
LOBJ= $(addprefix $(OBJ_DIR)/, $(LIB_OBJECTS))
OBJ= $(addprefix $(OBJ_DIR)/, $(ALL_OBJECTS))
DEP = $(addprefix $(OBJ_DIR)/, $(DEPENDENCIES))

# Keywords directing variable contents
debug: default
release: default

# Keyword specifying if lib need to be built
lib: 
	@echo Making $(DLIB).$(VERSION)
lib: default
# Default target builds lib and samples
default: $(LIBTARGET) $(BUILD_DIR)/$(CEXE) $(BUILD_DIR)/$(EXE)   

$(BUILD_DIR)/$(CEXE): $(OBJ_DIR)/$(CEXE).o
	mkdir -p $(@D)	
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@
	cp $(CURDIR)/$(CEXE).sh $(BUILD_DIR)/

$(BUILD_DIR)/$(EXE): $(OBJ_DIR)/$(EXE).o
	mkdir -p $(@D)	
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@
	cp $(CURDIR)/$(EXE).sh $(BUILD_DIR)/

$(BUILD_DIR)/$(SLIB): $(LOBJ)
	mkdir -p $(@D)	
	ar ruU $@.$(VERSION) $^
	ranlib $@.$(VERSION)
	ln -fs $(SLIB).$(VERSION) $(BUILD_DIR)/$(SLIB) 

$(BUILD_DIR)/$(DLIB): $(LOBJ)
	mkdir -p $(@D)	
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$(DLIB) -o $@.$(VERSION) $(LOBJ)
	ln -fs $(DLIB).$(VERSION) $(BUILD_DIR)/$(DLIB) 


# Include all .d files
-include $(DEP)
	
# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.

#example objects
$(OBJ_DIR)/$(CEXE).o: $(CEXE).c
	mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(EXE).o: $(EXE).cpp
	mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CXXFLAGS) -c $< -o $@

#lib objects
$(OBJ_DIR)/%.o: %.cpp 
	mkdir -p $(@D)
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c $< -o $@

# This should remove all generated files.
MCT= $(DEPENDENCIES) $(ALL_OBJECTS) $(OBJSUFFIX)
CLT= $(EXE) $(CEXE) 
RCL= $(DLIB) $(SLIB) $(DLIB).$(VERSION) $(SLIB).$(VERSION)

mostlyclean:
	rm -rf $(addprefix $(CURDIR)/, $(addprefix $(DBGDIR)/, $(MCT)) $(addprefix $(RELDIR)/, $(MCT)) )

clean: mostlyclean
	rm -rf $(addprefix $(CURDIR)/, $(addprefix $(DBGDIR)/, $(CLT)) $(addprefix $(RELDIR)/, $(CLT)) )

clobber: clean
	rm -rf $(addprefix $(CURDIR)/, $(addprefix $(DBGDIR)/, $(RCL)) $(addprefix $(RELDIR)/, $(RCL)) )

.PHONY: all mostlyclean clean clobber default debug release

remake: clean all

#VERISON here

