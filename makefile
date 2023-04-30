.PHONY: install clean test

# DEFINE COMPILER AND ARGUMENTS

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c++17 -fPIC -Isrc
LDFLAGS := -fvisibility=hidden -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread
LDLIBS := -lm -ffast-math -lstdc++ `pkg-config --libs lv2` 

CXXFLAGSTEST := -Wall -Wextra -Wpedantic -O2 -std=c++17 -fPIC -Isrc -DTEST_MODE_
LDFLAGSTEST := -fvisibility=hidden -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -pthread
LDLIBSTEST := -lm -ffast-math -lstdc++ -lboost_unit_test_framework

# DEFINE SOURCE DIRECTORIES, FILES, AND TARGETS

SRC_DIR := src
BUILD_DIR := build
INSTALL_DIR := $(HOME)/.lv2

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

PLUGIN_DIR := synthesthesia.lv2
PLUGIN_EXEC := synthesthesia.so

# DEFINE TEST DIRECTORIES, FILES, AND TARGETS

TEST_DIR := test
TEST_BUILD_DIR := $(TEST_DIR)/build

TEST := $(wildcard $(TEST_DIR)/*-test.cpp)
TESTOBJ := $(patsubst $(TEST_DIR)/%-test.cpp,$(BUILD_DIR)/%-test.o,$(TEST))
TESTDEP := $(TESTOBJ:.o=.d)

TEST_SRC_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(SRC))

TEST_EXEC := test-runner

#===============================================================
#================= RULES TO BUILD PLUGIN =======================
#===============================================================

$(BUILD_DIR):
	mkdir -p $@

# create cpp dependency files
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MM -MT '$(BUILD_DIR)/$*.o' $< > $@

# create object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS) -c $< -o $@

# create target executable (linking)
$(BUILD_DIR)/$(PLUGIN_EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) $(LDLIBS) $^ -o $@

# create final plugin folder with contents
$(PLUGIN_DIR): $(SRC_DIR)/manifest.ttl $(SRC_DIR)/synthesthesia.ttl $(BUILD_DIR)/$(PLUGIN_EXEC)
	rm -rf $(PLUGIN_DIR)
	mkdir -p $(PLUGIN_DIR)
	cp $(SRC_DIR)/manifest.ttl $(SRC_DIR)/synthesthesia.ttl $(PLUGIN_DIR)
	cp $(BUILD_DIR)/synthesthesia.so $(PLUGIN_DIR)

#===============================================================
#========== RULES TO BUILD TEST SOURCES + BOOST TESTS ==========
#===============================================================

$(TEST_BUILD_DIR):
	mkdir -p $@

# create src dependency files for test build
$(TEST_BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -DTEST_MODE_ -MM -MT '$(TEST_BUILD_DIR)/$*.o' $< > $@

# create src object files for test build
$(TEST_BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(TEST_BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS) -DTEST_MODE_ -c $< -o $@

# generate test dependencies
$(TEST_BUILD_DIR)/oscillator-base-test.d: $(TEST_DIR)/oscillator-base-test.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(BUILD_DIR)/$*.o' $^ > $@

# generate test file objects
$(TEST_BUILD_DIR)/oscillator-base-test.o: $(TEST_DIR)/oscillator-base-test.cpp
	$(CXX) $(CXXFLAGSTEST) -c $< -o $@

# create test
$(TEST_BUILD_DIR)/$(TEST_EXEC): $(TEST_BUILD_DIR)/oscillator-base-test.o $(TEST_SRC_OBJ)
	$(CXX) $(LDFLAGSTEST) $^ -o $@ $(LDLIBSTEST)

#===============================================================
#================ COMMAND LINE TARGETS =========================
#===============================================================

install: $(PLUGIN_DIR)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(PLUGIN_DIR)
	mv $(PLUGIN_DIR) $(INSTALL_DIR)

clean: 
	rm -rf $(PLUGIN_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(TEST_BUILD_DIR)
	
test: $(TEST_BUILD_DIR)/$(TEST_EXEC)
	./$^