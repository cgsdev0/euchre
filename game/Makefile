CXX=g++
CPPFLAGS=-IuWebSockets/src -IuWebSockets/uSockets/src -Iincludes
CXXFLAGS=-g3 -std=c++20 -W
LDFLAGS=-lz -lpthread -lssl -lcrypto
TARGET=GameServer

SRC=$(shell find src/ -type f -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
DEP=$(OBJ:.o=.d)

$(TARGET): $(OBJ) $(UWS)
	cd uWebSockets/uSockets && WITH_SSL=0 make CC=$(CC)
	$(CXX) -flto $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS) uWebSockets/uSockets/*.o;

src/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

DEP := $(OBJ:.o=.d)
-include $(DEP)

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEP) GameServer
	cd uWebSockets/uSockets && make clean

test_build: CXXFLAGS=-std=c++20 -Iincludes -g
test_build: CXXFLAGS += -Igoogletest/googletest/include
test_build: CXXFLAGS += -Igoogletest/googlemock/include
test_build: CXXFLAGS += -Isrc -Isrc/api
test_build: LDFLAGS=-lz -Lgoogletest/lib -lgtest -lgmock -lpthread
test_build: src/Game.o src/Bot.o src/api/API.o tests/*.cpp
	$(CXX) $(CXXFLAGS) tests/*.cpp -o test src/Game.o src/Bot.o src/api/API.o $(LDFLAGS)

.PHONY: test
test: test_build
	./$@

debug: CXXFLAGS += -g
debug: $(TARGET)

release: CXXFLAGS += -O2
release: $(TARGET)

all: $(TARGET)
