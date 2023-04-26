.PHONY: test

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c++17
LDFLAGS := -fvisibility=hidden -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -pthread
LDLIBS := -lm -ffast-math -lstdc++ -lboost_unit_test_framework

TESTDIR := .
SRCDIR := ../src
BUILDDIR := build 

SRC := $(wildcard $(SRC_DIR)/*.cpp)
SRCOBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

TEST := $(wildcard $(TEST_DIR)/*-test.cpp)
TESTOBJ := $(patsubst $(TEST_DIR)/*-test.cpp,$(BUILD_DIR)/%-test.o,$(TEST))

DEP := $(SRCOBJ:.o=.d) $(TESTOBJ:.o=.d)
-include $(DEP)

TARGET := test-runner

# build objects
$(TESTOBJ): $(BUILD_DIR)/%-test.o: $(TEST_DIR)/%-test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCOBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# build target executable
$(TARGET): $(SRCOBJ) $(TESTOBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(SRCOBJ) $(TESTOBJ) $^ -o $@ 

# run test
test: $(TARGET)
	./$(TARGET)

