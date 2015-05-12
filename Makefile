TARGET = octodyssey.app
CORE_DIR = ./octolib
INCLUDE_DIR = $(CORE_DIR)/includes Main Firefly Screens Map Managers Octo Decors Physics
BUILD_DIR = ./builds/game/
OUTPUT_DIR = .
# libraries directories (ex: ../libft)
LIB_DIRS = $(CORE_DIR)

# libraries (ex: ft, mlx, ...)
LIBS = octo sfml-system sfml-window sfml-graphics sfml-audio

# sources
SRC = $(SRC_STATES)										\
	  $(SRC_FIREFLY)									\
	  $(SRC_MAP)										\
	  $(SRC_MANAGERS)									\
	  $(SRC_OCTO)										\
	  $(SRC_DECORS)										\
	  $(SRC_PHYSICS)									\
	  Main/DefaultApplicationListener.cpp				\
	  Main/main.cpp

SRC_STATES =	Screens/StateTest.cpp					\
				Screens/FireflyTestScreen.cpp			\
				Screens/PaletteDemoScreen.cpp			\
				Screens/StateGame.cpp

SRC_FIREFLY =	Firefly/FireflySwarm.cpp				\
				Firefly/FireflyPopulation.cpp			\
				Firefly/FireflyPositionBehaviors.cpp

SRC_MAP =		Map/FactoryMap.cpp						\
				Map/Map.cpp								\
				Map/MapClassic.cpp						\
				Map/MapHigh.cpp							\
				Map/MapInstance.cpp

SRC_MANAGERS =	Managers/CollisionManager.cpp			\
				Managers/DecorManager.cpp				\
				Managers/MapManager.cpp					\
				Managers/TransitionManager.cpp

SRC_OCTO =		Octo/OctoNoise.cpp

SRC_DECORS =	Decors/Crystal.cpp						\
				Decors/Decor.cpp						\
				Decors/GameObject.cpp					\
				Decors/Rock.cpp							\
				Decors/Star.cpp							\
				Decors/StaticTileObject.cpp				\
				Decors/Tile.cpp							\
				Decors/Tree.cpp

SRC_PHYSICS =	Physics/DynamicPolygon.cpp				\
				Physics/Player.cpp						\
				Physics/Polygon.cpp

# package files
PACKAGE_FILE = default.pck

# compiler
CC = clang++
# packager
PACKAGER = $(CORE_DIR)/tools/packager/packager.app
# flags used in both mode
COMMON_FLAGS = -Werror -Wextra -Wall -std=c++11
# flags used in release mode
RELEASE_FLAGS = -O3
# flags used in debug mode
DEBUG_FLAGS = -g
# includes dirs paths
INCLUDE_DIRS = $(addprefix -I, $(INCLUDE_DIR))

OBJS = $(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
SRCS = $(SRC)
CFLAGS = $(COMMON_FLAGS)
CLIBS_FLAGS =  $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIBS))
COMPLETE_TARGET = $(OUTPUT_DIR)/$(TARGET)
MODE = release

ifeq ($(MODE), debug)
CFLAGS += $(DEBUG_FLAGS)
COLOR_ACTION = \033[1;32m
else
CFLAGS += $(RELEASE_FLAGS)
COLOR_ACTION = \033[0;34m
endif

COLOR_OFF = \033[0m
COLOR_OBJECT = \033[0m

all: print_summary $(COMPLETE_TARGET)

$(COMPLETE_TARGET): $(BUILD_DIR) package core_library $(OBJS)
	@echo " - $(COLOR_ACTION)building$(COLOR_OFF): $(COLOR_OBJECT)$@$(COLOR_OFF)"
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(CLIBS_FLAGS) 

$(addprefix $(BUILD_DIR)/, %.o) : $(subst $(BUILD_DIR),, %.cpp)
	@echo " - $(COLOR_ACTION)compiling$(COLOR_OFF): $(COLOR_OBJECT)$<$(COLOR_OFF)"
	@mkdir -p $(dir $@)
	@$(CC) $(INCLUDE_DIRS) $(CFLAGS) -c $< -o $@

re: print_summary fclean $(COMPLETE_TARGET)

fclean: clean
	@echo " - $(COLOR_ACTION)removing$(COLOR_OFF): $(COLOR_OBJECT)$(TARGET)$(COLOR_OFF)"
	@rm -f $(COMPLETE_TARGET)
	
clean:
	@echo " - $(COLOR_ACTION)removing$(COLOR_OFF): $(COLOR_OBJECT)$(addprefix "\\n\\t", $(notdir $(OBJS)))$(COLOR_OFF)"
	@rm -f $(OBJS)
	@echo " - $(COLOR_ACTION)removing$(COLOR_OFF): $(COLOR_OBJECT)$(BUILD_DIR)$(COLOR_OFF)"
	@rm -fr $(BUILD_DIR)

$(BUILD_DIR):
	@echo " - $(COLOR_ACTION)creating directory$(COLOR_OFF): $(COLOR_OBJECT)$(BUILD_DIR)$(COLOR_OFF)"
	@mkdir -p $(BUILD_DIR)

print_summary:
ifeq ($(MODE), debug)
	@echo " - Making $(TARGET) [DEBUG]: $(CFLAGS)"
else
	@echo " - Making $(TARGET): $(CFLAGS)"
endif

core_library:
	@make -s -C $(CORE_DIR) MODE=$(MODE)

clean_core_library:
	@make -s -C $(CORE_DIR) clean MODE=$(MODE)

fclean_core_library:
	@make -s -C $(CORE_DIR) fclean MODE=$(MODE)

package:
	$(PACKAGER) $(PACKAGE_FILE) ./resources/*

complete:
	make complete -C octolib/ MODE=$(MODE)
	make

static-checks:
	cppcheck --enable=all  -I ./includes/ -I ./octolib/includes ./includes/* ./src/
