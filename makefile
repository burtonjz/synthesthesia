.PHONY: install clean

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c++17 -fPIC -Isrc
LDFLAGS := -fvisibility=hidden -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread -shared
LDLIBS := -lm -ffast-math -lstdc++ `pkg-config --libs lv2` 

SRC_DIR := src
BUILD_DIR := build
INSTALL_DIR := $(HOME)/.lv2

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

PLUGIN_DIR := synthesthesia.lv2
PLUGIN_EXEC := synthesthesia.so

-include $(DEP)

$(BUILD_DIR):
	mkdir -p $@

# create cpp dependency files
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MM -MT '$(BUILD_DIR)/$*.o' $< > $@

# create object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/%.d | $(BUILD_DIR)
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

#install plugin to user's ~/.lv2 directory
install: $(PLUGIN_DIR)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(PLUGIN_DIR)
	mv $(PLUGIN_DIR) $(INSTALL_DIR)

clean: 
	rm -rf $(PLUGIN_DIR)
	rm -rf $(BUILD_DIR)
