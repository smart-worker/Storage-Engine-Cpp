CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Werror
LDFLAGS := 

SRCDIR := StorageEngine
SRC := repl.cpp $(SRCDIR)/bloomfilter.cpp $(SRCDIR)/lsmtree.cpp $(SRCDIR)/sstable.cpp
OBJ := $(SRC:.cpp=.o)
DEPS := $(SRCDIR)/bloomfilter.h $(SRCDIR)/lsmtree.h $(SRCDIR)/sstable.h $(SRCDIR)/config.h

TARGET := repl

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean