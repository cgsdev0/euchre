CXX=g++
CXXFLAGS=-std=c++20 -W -Iincludes
LDFLAGS=-lz -lpthread
TARGET=genetic

SRC=$(shell find src/ -type f -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
DEP=$(OBJ:.o=.d)

$(TARGET): $(OBJ)
	$(CXX) -flto $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS);

%.d: %.cpp
	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

src/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

-include $(DEP)

.PHONY: clean
clean:
	rm -f **/*.o
	rm -f genetic
	rm -f **/*.d

debug: clean
debug: CXXFLAGS += -g
debug: $(TARGET)

release: clean
release: CXXFLAGS += -O2
release: $(TARGET)

all: $(TARGET)
