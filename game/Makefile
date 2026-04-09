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

.PHONY: test_clean
test_clean:
	rm -f test

test_build:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -Igoogletest/googletest/include -Igoogletest/googlemock/include -Isrc -Isrc/api \
		tests/test_bot.cpp tests/test_game.cpp tests/test_simulation.cpp \
		src/Game.o src/Bot.o src/api/API.o \
		-o test googletest/lib/libgtest_main.a googletest/lib/libgtest.a -lpthread

.PHONY: test
test: test_build
	./test

debug: CXXFLAGS += -g
debug: $(TARGET)

release: CXXFLAGS += -O2
release: $(TARGET)

all: $(TARGET)
