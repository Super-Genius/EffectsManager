BIN = testEffectManager

CC ?= gcc
ifdef DEBUG
COPTS := -O0 -ggdb -Wall -fpermissive -D_DEBUG
CONFIG_DIR	:= Debug
else
COPTS := -O2 -s -Wall -fpermissive -DNDEBUG
CONFIG_DIR	:= Release
endif

LIBS = -L/usr/X11R6/lib -L../libs/Linux/$(CONFIG_DIR) -lGL -lGLU -lglut -lXi -lXmu -lXext -lX11 -lm -lEffectsManager
INCLUDES = -I../include -I../../SOIL/include

COPTS	+= $(INCLUDES)

all : $(BIN) textures shaders

clean : 
	rm -f $(BIN)

run : $(BIN)
	./$(BIN)

$(BIN) : $(BIN).cpp
	$(CC) -o $@ $(INCLUDES) $(COPTS) $< $(LIBS)
	@echo 'Done...'

textures: $(wildcard ../assets/textures/*.*)
	@mkdir -p textures
	cp $< .
	
shaders: $(wildcard ../assets/shaders/*.*)
	@mkdir -p shaders
	cp $< .
