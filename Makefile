# EffectManager makefile for linux 

# program defined
DELETER   := rm -f
CXX 	  := gcc
LD		  := gcc -shared

ifdef DEBUG
CONFIG_DIR	:= Debug
CXXFLAGS  := -O0 -ggdb -Wall -Wl,--no-undefined -fPIC -D_DEBUG
else
CONFIG_DIR	:= Release
CXXFLAGS  := -O2 -s -Wall -Wl,--no-undefined -fPIC -DNDEBUG
endif

# base source directory
SRC_DIR	  := .
# build (.obj) directory
BUILD_DIR := build
# subdirectory modules to be compiled
MODULES   := src

# add the root SRC_DIR directory prefix to the src modules (subdirs)
SRC_DIRS   := $(addprefix $(SRC_DIR)/,$(MODULES))
# build directories for Release
BUILD_DIRS := $(addprefix $(BUILD_DIR)/$(CONFIG_DIR)/,$(MODULES))

# all the source files in each modules
SRC       := $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.cpp))
# the destination build files for Release mode
OBJ       := $(patsubst $(SRC_DIR)/%.cpp,build/$(CONFIG_DIR)/%.o,$(SRC))

# any source directories are added as includes directory
INCLUDES  := -Iinclude -Iinclude/Platform/GL -I../SOIL/include -I../rapidxml/include -I../jsoncpp/include 

BIN	:= test/testEffectManager

# where the .a files get stored
LOCAL_LIBDIR := libs/Linux/$(CONFIG_DIR)

# the actual libraries to build
LIB 	  := $(LOCAL_LIBDIR)/libEffectsManager.so

LIB_STATIC			:= ../SOIL/libs/Linux/$(CONFIG_DIR)/libSOIL.a ../jsoncpp/Libs/Linux/$(CONFIG_DIR)/libjson.a

LIBS_BIN = -L/usr/X11R6/lib -Llibs/Linux/$(CONFIG_DIR) -lGL -lGLU -lglut -lXi -lXmu -lXext -lX11 -lm -lEffectsManager
INCLUDES_BIN = -I../include -I../../SOIL/include
# virtual paths for the makefile for source files
vpath %.cpp $(SRC_DIRS)

# macro to invoke compiling the source files
define make-goal
$1/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs $(BIN) textures shaders
	@echo 'Done...'

# library target Release
$(LIB): $(OBJ) $(LIB_STATIC)
	mkdir -p $(LOCAL_LIBDIR)
	$(DELETER) $@
	$(CXX) -shared -o $@ $(OBJ) $(LIB_STATIC)

checkdirs: $(BUILD_DIRS)

$(BIN):	test/testEffectManager.cpp $(LIB)
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $< $(LIBS_BIN)

# create all the build directories and the Release/Debug subdirectory
$(BUILD_DIRS):
	@mkdir -p $@

clean:
	$(DELETER) -r $(BUILD_DIR)/$(CONFIG_DIR)
	$(DELETER) -r shaders
	$(DELETER) -r textures

shaders:
	@mkdir -p test/shaders
	@cp -R assets/shaders test
	
textures:
	@mkdir -p test/textures
	@cp -R assets/textures test
	
# build each of the .o files for release
$(foreach bdir,$(BUILD_DIRS),$(eval $(call make-goal,$(bdir))))



