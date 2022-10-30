###########
# configs #
###########

# target name
TARGET = engg1340-game-project

# directories
INC_DIR = include ttui/include
SRC_DIR = src
BUILD_DIR = build

# flags
CXXFLAGS = $(addprefix -I, $(INC_DIR)) -std=c++17
DBG_FLAGS = -g -Wall -pedantic-errors -Lttui/build/debug/lib/ -lttui
REL_FLAGS = -O3 -Lttui/build/release/lib/ -lttui

#########
# files #
#########

# files
SRCS = $(shell find $(SRC_DIR) -name *.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=%.o)

###################
# builds and runs #
###################

# run debug
run run_debug: debug
	$(DBG_EXE)

# run release
run_release: release
	$(REL_EXE)

#########
# debug #
#########
DBG_BUILD_DIR = $(BUILD_DIR)/debug
DBG_EXE = $(DBG_BUILD_DIR)/bin/$(TARGET)
DBG_OBJS = $(addprefix $(DBG_BUILD_DIR)/obj/, $(OBJS))

-include $(DBG_OBJS:.o=.d)

debug: CXXFLAGS += $(DBG_FLAGS)
debug: install_ttui $(DBG_EXE)
	@echo Target debug finished.
	@echo

# executable
$(DBG_EXE): $(DBG_OBJS) $(DBG_BUILD_DIR)/bin/Hangman.txt
	mkdir -p $(dir $@)
	$(CXX) -o $@ $(DBG_OBJS) $(CXXFLAGS)

# objects
$(DBG_BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXXFLAGS) -MMD -MP

###########
# release #
###########
REL_BUILD_DIR = $(BUILD_DIR)/release
REL_EXE = $(REL_BUILD_DIR)/bin/$(TARGET)
REL_OBJS = $(addprefix $(REL_BUILD_DIR)/obj/, $(OBJS))

-include $(REL_OBJS:.o=.d)

release: CXXFLAGS += $(REL_FLAGS)
release: install_ttui $(REL_EXE)
	@echo Target release finished.
	@echo

# executable
$(REL_EXE): $(REL_OBJS) $(REL_BUILD_DIR)/bin/Hangman.txt
	mkdir -p $(dir $@)
	$(CXX) -o $@ $(REL_OBJS) $(CXXFLAGS)

# objects
$(REL_BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXXFLAGS) -MMD -MP

##########
# others #
##########

# clean
.PHONY: clean
clean:
	$(RM) $(REL_EXE) $(DBG_EXE) $(REL_BUILD_DIR)/bin/Hangman.txt $(DBG_BUILD_DIR)/bin/Hangman.txt
	find . -type f \( -name *.o -o -name *.d \) -exec $(RM) {} +

.PHONY: install_ttui
install_ttui:
	sh install_ttui.sh

.PHONY: %/Hangman.txt
%/Hangman.txt:
	mkdir -p $(dir $@)
	cp -f ./data/Hangman.txt $@