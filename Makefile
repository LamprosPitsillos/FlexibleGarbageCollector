# CC:=clang++
CC:=g++
CFLAGS:= -g -c -Wall --std=c++20 -fno-optimize-sibling-calls -fno-omit-frame-pointer -Og  
LDFLAGS:=

SRC_DIR:=src
INCLUDE_DIR:=include
TEST_DIR:=Tests
BUILD_DIR:=Build
BENCH_DIR:=Benchmarks

SRCS:=$(wildcard $(SRC_DIR)/*.cpp)

OBJS:=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SRCS:=$(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS:=$(TEST_SRCS:.cpp=.o)

BENCH_SRCS:=$(wildcard $(BENCH_DIR)/*.cpp)
BENCH_OBJS:=$(BENCH_SRCS:.cpp=.o)

DEFINES:=-DNLOG
EXECUTABLE=garbage_collector

# Link the object files to create the executable
build: $(OBJS)

test: $(OBJS) $(TEST_OBJS) $(BENCH_OBJS)


$(OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEFINES)  $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) $(DEFINES)  $< -o $@

$(BENCH_DIR)/%.o: $(BENCH_DIR)/%.cpp
	$(CC) $(CFLAGS) $(DEFINES)  $< -o $@


clean:
	rm -f $(OBJS) $(TEST_OBJS) 
