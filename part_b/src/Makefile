CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread -I../../part_a/src -I.

# Storage Engine path
STORAGE_ENGINE_PATH = ../../part_a/src/StorageEngine

# Server and Benchmark paths
SERVER_PATH = server
BENCHMARK_DATA_PATH = benchmarkdata

# Source files
SRC_STORAGE_ENGINE = $(STORAGE_ENGINE_PATH)/bloomfilter.cpp $(STORAGE_ENGINE_PATH)/sstable.cpp $(STORAGE_ENGINE_PATH)/lsmtree.cpp
SRC_SERVER = $(SERVER_PATH)/server.cpp $(SERVER_PATH)/resp_parser.cpp
SRC_BENCHMARK_DATA = $(BENCHMARK_DATA_PATH)/benchmarkdata.cpp
SRC_MAIN = main.cpp
SRC_BENCHMARK = $(SRC_MAIN) $(SRC_SERVER) $(SRC_BENCHMARK_DATA) $(SRC_STORAGE_ENGINE)

# Object files
OBJ_STORAGE_ENGINE = $(SRC_STORAGE_ENGINE:.cpp=.o)
OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
OBJ_BENCHMARK_DATA = $(SRC_BENCHMARK_DATA:.cpp=.o)
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)
OBJ_BENCHMARK = $(OBJ_MAIN) $(OBJ_SERVER) $(OBJ_BENCHMARK_DATA) $(OBJ_STORAGE_ENGINE)

TARGET_BENCHMARK = benchmark

all: $(TARGET_BENCHMARK)

$(TARGET_BENCHMARK): $(OBJ_BENCHMARK)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_BENCHMARK) $(TARGET_BENCHMARK)
	rm -f $(SERVER_PATH)/*.o $(BENCHMARK_DATA_PATH)/*.o *.o

# Dependency rules to ensure recompilation when headers change
$(STORAGE_ENGINE_PATH)/bloomfilter.o: $(STORAGE_ENGINE_PATH)/bloomfilter.cpp $(STORAGE_ENGINE_PATH)/bloomfilter.h $(STORAGE_ENGINE_PATH)/config.h
$(STORAGE_ENGINE_PATH)/sstable.o: $(STORAGE_ENGINE_PATH)/sstable.cpp $(STORAGE_ENGINE_PATH)/sstable.h $(STORAGE_ENGINE_PATH)/bloomfilter.h $(STORAGE_ENGINE_PATH)/config.h
$(STORAGE_ENGINE_PATH)/lsmtree.o: $(STORAGE_ENGINE_PATH)/lsmtree.cpp $(STORAGE_ENGINE_PATH)/lsmtree.h $(STORAGE_ENGINE_PATH)/sstable.h $(STORAGE_ENGINE_PATH)/config.h
$(SERVER_PATH)/resp_parser.o: $(SERVER_PATH)/resp_parser.cpp $(SERVER_PATH)/resp_parser.h
$(SERVER_PATH)/server.o: $(SERVER_PATH)/server.cpp $(SERVER_PATH)/server.h $(SERVER_PATH)/resp_parser.h
$(BENCHMARK_DATA_PATH)/benchmarkdata.o: $(BENCHMARK_DATA_PATH)/benchmarkdata.cpp $(BENCHMARK_DATA_PATH)/benchmarkdata.h
main.o: main.cpp $(SERVER_PATH)/server.h $(BENCHMARK_DATA_PATH)/benchmarkdata.h
