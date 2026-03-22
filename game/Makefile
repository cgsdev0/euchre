CXX=g++
CXXFLAGS=-std=c++20 -W -IuWebSockets/src -IuWebSockets/uSockets/src -Iincludes
LDFLAGS=-lz -lpthread -lssl -lcrypto
TARGET=GameServer

SRC=$(shell find src/ -type f -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
DEP=$(OBJ:.o=.d)

$(TARGET): $(OBJ) $(UWS)
	cd uWebSockets/uSockets && WITH_SSL=0 make CC=$(CC)
	$(CXX) -flto $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS) uWebSockets/uSockets/*.o;

%.d: %.cpp
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

src/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

-include $(DEP)

.PHONY: clean
clean:
	rm -f **/*.o
	rm -f GameServer
	rm -f **/*.d
	cd uWebSockets/uSockets && make clean

test: CXXFLAGS=-std=c++20 -W -Iincludes
test: CXXFLAGS += -Igoogletest/googletest/include
test: CXXFLAGS += -Igoogletest/googlemock/include
test: CXXFLAGS += -Isrc -Isrc/api
test: LDFLAGS=-lz -Lgoogletest/lib -lgtest -lgmock 
test: src/Game.o src/api/API.o tests/*.cpp
	$(CXX) $(CXXFLAGS) tests/*.cpp -o test src/Game.o src/api/API.o $(LDFLAGS) 
	./test

debug: clean
debug: CXXFLAGS += -g
debug: $(TARGET)

release: clean
release: CXXFLAGS += -O2
release: $(TARGET)

all: $(TARGET)
