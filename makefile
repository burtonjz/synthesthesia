.PHONY: install clean

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -O2 -std=c++17 -Isrc
LDFLAGS := -fvisibility=hidden -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -pthread
LDLIBS := -lm -ffast-math -lstdc++ `pkg-config --cflags --libs lv2`

SRC_DIR := src
BUILD_DIR := build
PLUGIN := synthesthesia.lv2
INSTALL_DIR := $(HOME)/.lv2

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

TARGET := synthesthesia.so

-include $(DEP)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -fPIC -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(PLUGIN): $(SRC_DIR)/manifest.ttl $(SRC_DIR)/synthesthesia.ttl $(BUILD_DIR)/$(TARGET)
	rm -rf $(PLUGIN)
	mkdir -p $(PLUGIN)
	cp $(SRC_DIR)/manifest.ttl $(SRC_DIR)/synthesthesia.ttl $(PLUGIN)
	cp $(BUILD_DIR)/synthesthesia.so $(PLUGIN)

install: $(PLUGIN)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(PLUGIN)
	mv $(PLUGIN) $(INSTALL_DIR)

clean: 
	rm -rf $(PLUGIN)
	rm -rf $(BUILD_DIR)
