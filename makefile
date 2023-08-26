.PHONY: install clean test

# DEFINE COMPILER AND ARGUMENTS

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c++17 -fPIC -Isrc
LDFLAGS := -fvisibility=hidden -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread
LDLIBS := -lm -ffast-math -lstdc++ `pkg-config --libs lv2` 

CXX_UI_FLAGS := -fvisibility=hidden -std=c++17 -fPIC -DPIC -DPUGL_HAVE_CAIRO -IBWidgets/include `pkg-config --cflags lv2 cairo x11`
LD_UI_FLAGS := -shared -pthread
LD_UI_LIBS := -LBWidgets/build -Wl,-Bstatic -lbwidgetscore -lcairoplus -lpugl `pkg-config --libs --static lv2` -lm -Wl,-Bdynamic `pkg-config --libs cairo x11`

CXXFLAGSTEST := -Wall -Wextra -Wpedantic -O2 -std=c++17 -fPIC -Isrc -DTEST_MODE_
LDFLAGSTEST := -fvisibility=hidden -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -pthread
LDLIBSTEST := -lm -ffast-math -lstdc++ -lboost_unit_test_framework

# DEFINE SOURCE DIRECTORIES, FILES, AND TARGETS

PLUGIN_DIR := synthesthesia.lv2
PLUGIN_EXEC := synthesthesia.so
PLUGIN_UI_EXEC := synthesthesia_ui.so

SRC_DIR := src
BUILD_DIR := build
INSTALL_DIR := $(HOME)/.lv2

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

X11_UI_SRC_DIR := $(SRC_DIR)/ui-x11
X11_UI_BUILD_DIR := $(BUILD_DIR)/ui-x11
SRC_X11_UI := $(wildcard $(X11_UI_SRC_DIR)/*.cpp)
OBJ_X11_UI := $(patsubst $(X11_UI_SRC_DIR)/%.cpp,$(X11_UI_BUILD_DIR)/%.o,$(SRC_X11_UI))
DEP_X11_UI := $(OBJ:.o=.d)

# DEFINE TEST DIRECTORIES, FILES, AND TARGETS

TEST_DIR := test
TEST_BUILD_DIR := $(TEST_DIR)/build

TEST_SRC_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(filter-out $(SRC_DIR)/main.cpp,$(SRC)))
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
$(PLUGIN_DIR): $(SRC_DIR)/manifest.ttl $(SRC_DIR)/synthesthesia.ttl $(BUILD_DIR)/$(PLUGIN_EXEC) # $(X11_UI_BUILD_DIR)/synthesthesia_ui.so assets
	rm -rf $(PLUGIN_DIR)
	mkdir -p $(PLUGIN_DIR)
	cp $(SRC_DIR)/manifest.ttl $(SRC_DIR)/synthesthesia.ttl $(PLUGIN_DIR)
	cp $(BUILD_DIR)/synthesthesia.so $(PLUGIN_DIR)
	# cp $(X11_UI_BUILD_DIR)/synthesthesia_ui.so $(PLUGIN_DIR)
	# cp assets/*.png $(PLUGIN_DIR)

#===============================================================
#================= RULES TO BUILD PLUGIN UI ====================
#===============================================================

# g++ src/ui-x11/synthesthesia-ui.cpp -fvisibility=hidden -std=c++17 -fPIC -DPIC -DPUGL_HAVE_CAIRO -IBWidgets/include `pkg-config --cflags lv2 cairo x11` -c
# g++ -shared -pthread -LBWidgets/build synthesthesia-ui.o -Wl,-Bstatic -lbwidgetscore -lcairoplus -lpugl `pkg-config --libs --static lv2` -lm -Wl,-Bdynamic `pkg-config --libs cairo x11` -o synthesthesia_ui.so

$(X11_UI_BUILD_DIR):
	mkdir -p $@

# create cpp dependency files
$(X11_UI_BUILD_DIR)/%.d: $(X11_UI_SRC_DIR)/%.cpp | $(X11_UI_BUILD_DIR)
	$(CXX) $(CXX_UI_FLAGS) -MM -MT '$(X11_UI_SRC_DIR)/$*.o' $< > $@

# create object files
$(X11_UI_BUILD_DIR)/%.o: $(X11_UI_SRC_DIR)/%.cpp $(X11_UI_BUILD_DIR)/%.d
	$(CXX) $(CXX_UI_FLAGS) -c $< -o $@

# create target executable (linking)
$(X11_UI_BUILD_DIR)/$(PLUGIN_UI_EXEC): $(OBJ_X11_UI)
	$(CXX) $(LD_UI_FLAGS) $(LD_UI_LIBS) $^ -o $@

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
$(TEST_BUILD_DIR)/main.d: $(TEST_DIR)/main.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MM -MT '$(TEST_BUILD_DIR)/$*.o' $^ > $@

# generate test file objects
$(TEST_BUILD_DIR)/main.o: $(TEST_DIR)/main.cpp $(TEST_BUILD_DIR)/main.d
	$(CXX) $(CXXFLAGSTEST) -c $< -o $@

# create test
$(TEST_BUILD_DIR)/$(TEST_EXEC): $(TEST_BUILD_DIR)/main.o $(TEST_SRC_OBJ)
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