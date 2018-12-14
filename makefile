CXX 	= g++

SRC 	:= $(wildcard ./src/*.cpp)
OBJ 	:= $(SRC:./src/%.cpp=./obj/%.o)
OBJDIR 	= ./obj
BIN 	= slugbox
BINDIR 	= ./bin/
DEPDIR 	= ./dep/
DEPFLAGS = -MMD -MP -MF $(DEPDIR)$*.d

CFLAGS = -g -I./inc
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: dirs $(OBJ)
	$(CXX) $(OBJ) $(LFLAGS) -o $(BINDIR)$(BIN)

./obj/%.o: ./src/%.cpp $(DEPDIR)%.d
	@echo "Compiling $(basename $(notdir $@))..."
	$(CXX) $(DEPFLAGS) -c -o $@ $< $(CFLAGS)

dirs:
	-@ mkdir -p bin dep obj

$(DEPDIR)%.d: ;

-include $(SRC:./src/%.cpp=./dep/%.d)

clean:
	-rm -rf ./dep ./obj ./bin