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
SRC = $(SRC_PHYSICS)										\
	  $(SRC_STATES)									\
	  $(SRC_FIREFLY)									\
	  $(SRC_MAP)										\
	  $(SRC_MANAGERS)									\
	  $(SRC_OCTO)										\
	  $(SRC_DECORS)										\
	  Main/DefaultApplicationListener.cpp				\
	  Main/main.cpp

SRC_STATES =	Screens/StateTest.cpp					\
				Screens/FireflyTestScreen.cpp			\
				Screens/PaletteDemoScreen.cpp			\
				Screens/SpriteSheetDemoScreen.cpp		\
				Screens/AnimatedSpriteDemoScreen.cpp	\
				Screens/AudioDemoScreen.cpp				\
				Screens/EngineScreen.cpp				\
				Screens/PhysicsMapScreen.cpp				\
				Screens/DecorManagerDemoScreen.cpp		\
				Screens/StateGame.cpp

SRC_FIREFLY =	Firefly/FireflySwarm.cpp				\
				Firefly/FireflyPopulation.cpp			\
				Firefly/FireflyPositionBehaviors.cpp

SRC_MAP =		Map/Map.cpp								\
				Map/TerrainManager.cpp			\
				Map/MapInstance.cpp

SRC_MANAGERS =	Managers/MapManager.cpp

SRC_OCTO =		Octo/OctoNoise.cpp

SRC_DECORS =	Decors/StaticTileObject.cpp				\
				Decors/GameObject.cpp					\
				Decors/DecorManager.cpp					\
				Decors/DecorBuilder.cpp					\
				Decors/ADecor.cpp						\
				Decors/ABiome.cpp						\
				Decors/TestBiome.cpp					\
				Decors/Rock.cpp

SRC_PHYSICS =	Physics/PolygonShape.cpp						\
				Physics/RectangleShape.cpp						\
				Physics/ConvexShape.cpp						\
				Physics/CircleShape.cpp						\
				Physics/Tile.cpp					\
				Physics/PhysicsEngine.cpp					\
				Physics/AShape.cpp

# package files
PACKAGE_FILE = default.pck

# compiler
CC = clang++
# packager
PACKAGER = $(CORE_DIR)/tools/packager/packager.app
# flags used in both mode
COMMON_FLAGS = -Werror -Wextra -Wall -std=c++11
# flags used in release mode
RELEASE_FLAGS = -O3 -DNDEBUG
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

dep: depend re

depend:
	@echo "- $(COLOR_ACTION)creating dependencies..."
	@makedepend -Y $(INCLUDE_DIRS) -p$(BUILD_DIR) -- $(CFLAGS) -- $(SRC)
	@rm -f Makefile.bak

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
	$(PACKAGER) $(PACKAGE_FILE) -h Main/ResourceDefinitions.hpp ./resources/*

complete:
	make complete -C octolib/ MODE=$(MODE)
	make re

static-checks:
	cppcheck --enable=all  -I ./includes/ -I ./octolib/includes ./includes/* ./src/
# DO NOT DELETE

./builds/game/Screens/StateTest.o: Screens/StateTest.hpp
./builds/game/Screens/StateTest.o: ./octolib/includes/AbstractState.hpp
./builds/game/Screens/FireflyTestScreen.o: Screens/FireflyTestScreen.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/AbstractState.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/GraphicsManager.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ListenerHelpers.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/NonCopyable.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/GraphicsListeners.hpp
./builds/game/Screens/FireflyTestScreen.o: Firefly/FireflySwarm.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Stack.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Stack.hxx
./builds/game/Screens/FireflyTestScreen.o: Main/BSpline.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Math.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Interpolations.hpp
./builds/game/Screens/FireflyTestScreen.o: Firefly/FireflyPopulation.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Screens/FireflyTestScreen.o: Main/PointHandle.hpp
./builds/game/Screens/FireflyTestScreen.o: Firefly/FireflyPositionBehaviors.hpp
./builds/game/Screens/FireflyTestScreen.o: ResourceDefinitions.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Application.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/StateManager.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/AbstractTransition.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/StateManager.hxx
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Camera.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/DefaultGraphicsListeners.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ResourceManager.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/PackageHeader.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Palette.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ByteArray.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ByteArray.hxx
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Hsv.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ColorWheel.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/details/ResourceManagerImp.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/PackageReader.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/IResourceListener.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Console.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/IConsoleListener.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ConsoleCore.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ConsoleInterpreter.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/details/Meta.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ConsoleInterpreter.hxx
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/details/GetTypeName.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/WPrintSFML.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ExpectChar.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ExpectChar.hxx
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ConsoleCore.hxx
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/ConsoleHistory.hpp
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/Console.hxx
./builds/game/Screens/FireflyTestScreen.o: ./octolib/includes/StringUtils.hpp
./builds/game/Screens/PaletteDemoScreen.o: Screens/PaletteDemoScreen.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/AbstractState.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Screens/PaletteDemoScreen.o: ResourceDefinitions.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/Hsv.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/Application.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/StateManager.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/NonCopyable.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/AbstractTransition.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/StateManager.hxx
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/GraphicsManager.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ListenerHelpers.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ResourceManager.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/PackageHeader.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/Palette.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ByteArray.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ByteArray.hxx
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ColorWheel.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/details/ResourceManagerImp.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/PackageReader.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/IResourceListener.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/Console.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/IConsoleListener.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/GraphicsListeners.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ConsoleCore.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ConsoleInterpreter.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/details/Meta.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ConsoleInterpreter.hxx
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/details/GetTypeName.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/WPrintSFML.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ExpectChar.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ExpectChar.hxx
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ConsoleCore.hxx
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/ConsoleHistory.hpp
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/Console.hxx
./builds/game/Screens/PaletteDemoScreen.o: ./octolib/includes/StringUtils.hpp
./builds/game/Screens/StateGame.o: Screens/StateGame.hpp
./builds/game/Screens/StateGame.o: ./octolib/includes/AbstractState.hpp
./builds/game/Screens/StateGame.o: Managers/MapManager.hpp
./builds/game/Screens/StateGame.o: Managers/TransitionManager.hpp
./builds/game/Screens/StateGame.o: Map/FactoryMap.hpp Map/Map.hpp
./builds/game/Screens/StateGame.o: ./octolib/includes/Array2D.hpp
./builds/game/Screens/StateGame.o: ./octolib/includes/Array2D.hxx
./builds/game/Screens/StateGame.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Screens/StateGame.o: Managers/DecorManager.hpp
./builds/game/Screens/StateGame.o: Managers/CollisionManager.hpp
./builds/game/Screens/StateGame.o: Physics/Player.hpp
./builds/game/Screens/StateGame.o: Physics/DynamicPolygon.hpp
./builds/game/Firefly/FireflySwarm.o: Firefly/FireflySwarm.hpp
./builds/game/Firefly/FireflySwarm.o: ./octolib/includes/Stack.hpp
./builds/game/Firefly/FireflySwarm.o: ./octolib/includes/Stack.hxx
./builds/game/Firefly/FireflySwarm.o: Main/BSpline.hpp
./builds/game/Firefly/FireflySwarm.o: ./octolib/includes/Math.hpp
./builds/game/Firefly/FireflySwarm.o: ./octolib/includes/Interpolations.hpp
./builds/game/Firefly/FireflySwarm.o: Firefly/FireflyPositionBehaviors.hpp
./builds/game/Firefly/FireflySwarm.o: Firefly/FireflyPopulation.hpp
./builds/game/Firefly/FireflySwarm.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Firefly/FireflyPopulation.o: Firefly/FireflyPopulation.hpp
./builds/game/Firefly/FireflyPopulation.o: Firefly/FireflySwarm.hpp
./builds/game/Firefly/FireflyPopulation.o: ./octolib/includes/Stack.hpp
./builds/game/Firefly/FireflyPopulation.o: ./octolib/includes/Stack.hxx
./builds/game/Firefly/FireflyPopulation.o: Main/BSpline.hpp
./builds/game/Firefly/FireflyPopulation.o: ./octolib/includes/Math.hpp
./builds/game/Firefly/FireflyPopulation.o: ./octolib/includes/Interpolations.hpp
./builds/game/Firefly/FireflyPopulation.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Firefly/FireflyPositionBehaviors.o: Firefly/FireflyPositionBehaviors.hpp
./builds/game/Firefly/FireflyPositionBehaviors.o: Firefly/FireflySwarm.hpp
./builds/game/Firefly/FireflyPositionBehaviors.o: ./octolib/includes/Stack.hpp
./builds/game/Firefly/FireflyPositionBehaviors.o: ./octolib/includes/Stack.hxx
./builds/game/Firefly/FireflyPositionBehaviors.o: Main/BSpline.hpp
./builds/game/Firefly/FireflyPositionBehaviors.o: ./octolib/includes/Math.hpp
./builds/game/Firefly/FireflyPositionBehaviors.o: ./octolib/includes/Interpolations.hpp
./builds/game/Map/FactoryMap.o: Map/FactoryMap.hpp Map/Map.hpp
./builds/game/Map/FactoryMap.o: ./octolib/includes/Array2D.hpp
./builds/game/Map/FactoryMap.o: ./octolib/includes/Array2D.hxx
./builds/game/Map/FactoryMap.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Map/FactoryMap.o: Map/MapHigh.hpp Map/MapClassic.hpp
./builds/game/Map/FactoryMap.o: Map/MapInstance.hpp
./builds/game/Map/FactoryMap.o: Decors/StaticTileObject.hpp
./builds/game/Map/FactoryMap.o: Decors/GameObject.hpp
./builds/game/Map/Map.o: Map/Map.hpp ./octolib/includes/Array2D.hpp
./builds/game/Map/Map.o: ./octolib/includes/Array2D.hxx Physics/Tile.hpp
./builds/game/Map/Map.o: Physics/Polygon.hpp
./builds/game/Map/MapClassic.o: Map/MapClassic.hpp Map/Map.hpp
./builds/game/Map/MapClassic.o: ./octolib/includes/Array2D.hpp
./builds/game/Map/MapClassic.o: ./octolib/includes/Array2D.hxx
./builds/game/Map/MapClassic.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Map/MapClassic.o: Octo/OctoNoise.hpp
./builds/game/Map/MapHigh.o: Map/MapHigh.hpp Map/Map.hpp
./builds/game/Map/MapHigh.o: ./octolib/includes/Array2D.hpp
./builds/game/Map/MapHigh.o: ./octolib/includes/Array2D.hxx Physics/Tile.hpp
./builds/game/Map/MapHigh.o: Physics/Polygon.hpp Octo/OctoNoise.hpp
./builds/game/Map/MapInstance.o: Map/MapInstance.hpp Map/Map.hpp
./builds/game/Map/MapInstance.o: ./octolib/includes/Array2D.hpp
./builds/game/Map/MapInstance.o: ./octolib/includes/Array2D.hxx
./builds/game/Map/MapInstance.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Map/MapInstance.o: Decors/StaticTileObject.hpp
./builds/game/Map/MapInstance.o: Decors/GameObject.hpp Octo/OctoNoise.hpp
./builds/game/Managers/CollisionManager.o: Managers/CollisionManager.hpp
./builds/game/Managers/CollisionManager.o: Managers/MapManager.hpp
./builds/game/Managers/CollisionManager.o: Managers/TransitionManager.hpp
./builds/game/Managers/CollisionManager.o: Map/FactoryMap.hpp Map/Map.hpp
./builds/game/Managers/CollisionManager.o: ./octolib/includes/Array2D.hpp
./builds/game/Managers/CollisionManager.o: ./octolib/includes/Array2D.hxx
./builds/game/Managers/CollisionManager.o: Physics/Tile.hpp
./builds/game/Managers/CollisionManager.o: Physics/Polygon.hpp
./builds/game/Managers/CollisionManager.o: Managers/DecorManager.hpp
./builds/game/Managers/CollisionManager.o: Physics/Player.hpp
./builds/game/Managers/CollisionManager.o: Physics/DynamicPolygon.hpp
./builds/game/Managers/CollisionManager.o: Physics/NPC.hpp
./builds/game/Managers/DecorManager.o: Managers/DecorManager.hpp
./builds/game/Managers/DecorManager.o: Managers/MapManager.hpp
./builds/game/Managers/DecorManager.o: Managers/TransitionManager.hpp
./builds/game/Managers/DecorManager.o: Map/FactoryMap.hpp Map/Map.hpp
./builds/game/Managers/DecorManager.o: ./octolib/includes/Array2D.hpp
./builds/game/Managers/DecorManager.o: ./octolib/includes/Array2D.hxx
./builds/game/Managers/DecorManager.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Managers/DecorManager.o: Managers/CollisionManager.hpp
./builds/game/Managers/DecorManager.o: Physics/Player.hpp
./builds/game/Managers/DecorManager.o: Physics/DynamicPolygon.hpp
./builds/game/Managers/DecorManager.o: Decors/Tree.hpp Decors/Decor.hpp
./builds/game/Managers/DecorManager.o: Decors/GameObject.hpp
./builds/game/Managers/DecorManager.o: Decors/Crystal.hpp Decors/Rock.hpp
./builds/game/Managers/DecorManager.o: Decors/Star.hpp
./builds/game/Managers/MapManager.o: Managers/MapManager.hpp
./builds/game/Managers/MapManager.o: Managers/TransitionManager.hpp
./builds/game/Managers/MapManager.o: Map/FactoryMap.hpp Map/Map.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/Array2D.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/Array2D.hxx
./builds/game/Managers/MapManager.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Managers/MapManager.o: Managers/DecorManager.hpp
./builds/game/Managers/MapManager.o: Managers/CollisionManager.hpp
./builds/game/Managers/MapManager.o: Physics/Player.hpp
./builds/game/Managers/MapManager.o: Physics/DynamicPolygon.hpp
./builds/game/Managers/MapManager.o: ResourceDefinitions.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/Application.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/StateManager.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/NonCopyable.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/AbstractTransition.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/AbstractState.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/StateManager.hxx
./builds/game/Managers/MapManager.o: ./octolib/includes/ResourceManager.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/PackageHeader.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/Palette.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/ByteArray.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/ByteArray.hxx
./builds/game/Managers/MapManager.o: ./octolib/includes/Hsv.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/ColorWheel.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/details/ResourceManagerImp.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/PackageReader.hpp
./builds/game/Managers/MapManager.o: ./octolib/includes/IResourceListener.hpp
./builds/game/Managers/TransitionManager.o: Managers/TransitionManager.hpp
./builds/game/Managers/TransitionManager.o: Map/FactoryMap.hpp Map/Map.hpp
./builds/game/Managers/TransitionManager.o: ./octolib/includes/Array2D.hpp
./builds/game/Managers/TransitionManager.o: ./octolib/includes/Array2D.hxx
./builds/game/Managers/TransitionManager.o: Physics/Tile.hpp
./builds/game/Managers/TransitionManager.o: Physics/Polygon.hpp
./builds/game/Managers/TransitionManager.o: Managers/MapManager.hpp
./builds/game/Managers/TransitionManager.o: Managers/DecorManager.hpp
./builds/game/Managers/TransitionManager.o: Managers/CollisionManager.hpp
./builds/game/Managers/TransitionManager.o: Physics/Player.hpp
./builds/game/Managers/TransitionManager.o: Physics/DynamicPolygon.hpp
./builds/game/Octo/OctoNoise.o: Octo/OctoNoise.hpp
./builds/game/Decors/Crystal.o: Decors/Crystal.hpp Decors/Decor.hpp
./builds/game/Decors/Crystal.o: Decors/GameObject.hpp
./builds/game/Decors/Decor.o: Decors/Decor.hpp Decors/GameObject.hpp
./builds/game/Decors/Decor.o: Map/Map.hpp ./octolib/includes/Array2D.hpp
./builds/game/Decors/Decor.o: ./octolib/includes/Array2D.hxx Physics/Tile.hpp
./builds/game/Decors/Decor.o: Physics/Polygon.hpp
./builds/game/Decors/GameObject.o: Decors/GameObject.hpp
./builds/game/Decors/Rock.o: Decors/Rock.hpp Decors/Decor.hpp
./builds/game/Decors/Rock.o: Decors/GameObject.hpp
./builds/game/Decors/Star.o: Decors/Star.hpp Decors/Decor.hpp
./builds/game/Decors/Star.o: Decors/GameObject.hpp ResourceDefinitions.hpp
./builds/game/Decors/Star.o: ./octolib/includes/ResourceManager.hpp
./builds/game/Decors/Star.o: ./octolib/includes/NonCopyable.hpp
./builds/game/Decors/Star.o: ./octolib/includes/PackageHeader.hpp
./builds/game/Decors/Star.o: ./octolib/includes/Palette.hpp
./builds/game/Decors/Star.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Decors/Star.o: ./octolib/includes/ByteArray.hpp
./builds/game/Decors/Star.o: ./octolib/includes/ByteArray.hxx
./builds/game/Decors/Star.o: ./octolib/includes/Hsv.hpp
./builds/game/Decors/Star.o: ./octolib/includes/ColorWheel.hpp
./builds/game/Decors/Star.o: ./octolib/includes/details/ResourceManagerImp.hpp
./builds/game/Decors/Star.o: ./octolib/includes/PackageReader.hpp
./builds/game/Decors/Star.o: ./octolib/includes/IResourceListener.hpp
./builds/game/Decors/Star.o: ./octolib/includes/Application.hpp
./builds/game/Decors/Star.o: ./octolib/includes/StateManager.hpp
./builds/game/Decors/Star.o: ./octolib/includes/AbstractTransition.hpp
./builds/game/Decors/Star.o: ./octolib/includes/AbstractState.hpp
./builds/game/Decors/Star.o: ./octolib/includes/StateManager.hxx
./builds/game/Decors/StaticTileObject.o: Decors/StaticTileObject.hpp
./builds/game/Decors/StaticTileObject.o: Decors/GameObject.hpp Map/Map.hpp
./builds/game/Decors/StaticTileObject.o: ./octolib/includes/Array2D.hpp
./builds/game/Decors/StaticTileObject.o: ./octolib/includes/Array2D.hxx
./builds/game/Decors/StaticTileObject.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Decors/Tree.o: Decors/Tree.hpp Decors/Decor.hpp
./builds/game/Decors/Tree.o: Decors/GameObject.hpp Map/Map.hpp
./builds/game/Decors/Tree.o: ./octolib/includes/Array2D.hpp
./builds/game/Decors/Tree.o: ./octolib/includes/Array2D.hxx Physics/Tile.hpp
./builds/game/Decors/Tree.o: Physics/Polygon.hpp
./builds/game/Physics/DynamicPolygon.o: Physics/DynamicPolygon.hpp
./builds/game/Physics/DynamicPolygon.o: Physics/Polygon.hpp
./builds/game/Physics/Player.o: Physics/Player.hpp Physics/DynamicPolygon.hpp
./builds/game/Physics/Player.o: Physics/Polygon.hpp Physics/Tile.hpp
./builds/game/Physics/Player.o: Physics/NPC.hpp
./builds/game/Physics/NPC.o: Physics/NPC.hpp Physics/DynamicPolygon.hpp
./builds/game/Physics/NPC.o: Physics/Polygon.hpp Physics/Tile.hpp
./builds/game/Physics/Polygon.o: Physics/Polygon.hpp
./builds/game/Physics/Tile.o: Physics/Tile.hpp Physics/Polygon.hpp
./builds/game/Main/DefaultApplicationListener.o: Main/DefaultApplicationListener.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/GraphicsManager.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ListenerHelpers.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/NonCopyable.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/GraphicsListeners.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/Application.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/StateManager.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/AbstractTransition.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/AbstractState.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/StateManager.hxx
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/Console.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/IConsoleListener.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ConsoleCore.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ConsoleInterpreter.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/details/Meta.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ConsoleInterpreter.hxx
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/details/GetTypeName.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/WPrintSFML.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ExpectChar.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ExpectChar.hxx
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ConsoleCore.hxx
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/ConsoleHistory.hpp
./builds/game/Main/DefaultApplicationListener.o: ./octolib/includes/Console.hxx
./builds/game/Main/main.o: ./octolib/includes/Application.hpp
./builds/game/Main/main.o: ./octolib/includes/StateManager.hpp
./builds/game/Main/main.o: ./octolib/includes/NonCopyable.hpp
./builds/game/Main/main.o: ./octolib/includes/AbstractTransition.hpp
./builds/game/Main/main.o: ./octolib/includes/AbstractState.hpp
./builds/game/Main/main.o: ./octolib/includes/StateManager.hxx
./builds/game/Main/main.o: ./octolib/includes/GraphicsManager.hpp
./builds/game/Main/main.o: ./octolib/includes/ListenerHelpers.hpp
./builds/game/Main/main.o: ./octolib/includes/Console.hpp
./builds/game/Main/main.o: ./octolib/includes/IConsoleListener.hpp
./builds/game/Main/main.o: ./octolib/includes/GraphicsListeners.hpp
./builds/game/Main/main.o: ./octolib/includes/ConsoleCore.hpp
./builds/game/Main/main.o: ./octolib/includes/ConsoleInterpreter.hpp
./builds/game/Main/main.o: ./octolib/includes/details/Meta.hpp
./builds/game/Main/main.o: ./octolib/includes/ConsoleInterpreter.hxx
./builds/game/Main/main.o: ./octolib/includes/details/GetTypeName.hpp
./builds/game/Main/main.o: ./octolib/includes/WPrintSFML.hpp
./builds/game/Main/main.o: ./octolib/includes/ExpectChar.hpp
./builds/game/Main/main.o: ./octolib/includes/ExpectChar.hxx
./builds/game/Main/main.o: ./octolib/includes/ConsoleCore.hxx
./builds/game/Main/main.o: ./octolib/includes/ConsoleHistory.hpp
./builds/game/Main/main.o: ./octolib/includes/Console.hxx
./builds/game/Main/main.o: ./octolib/includes/DefaultTransition.hpp
./builds/game/Main/main.o: Main/DefaultApplicationListener.hpp
./builds/game/Main/main.o: Screens/StateTest.hpp
./builds/game/Main/main.o: Screens/FireflyTestScreen.hpp
./builds/game/Main/main.o: Firefly/FireflySwarm.hpp
./builds/game/Main/main.o: ./octolib/includes/Stack.hpp
./builds/game/Main/main.o: ./octolib/includes/Stack.hxx Main/BSpline.hpp
./builds/game/Main/main.o: ./octolib/includes/Math.hpp
./builds/game/Main/main.o: ./octolib/includes/Interpolations.hpp
./builds/game/Main/main.o: Firefly/FireflyPopulation.hpp
./builds/game/Main/main.o: ./octolib/includes/IColorProvider.hpp
./builds/game/Main/main.o: Main/PointHandle.hpp Screens/PaletteDemoScreen.hpp
./builds/game/Main/main.o: Screens/StateGame.hpp Managers/MapManager.hpp
./builds/game/Main/main.o: Managers/TransitionManager.hpp Map/FactoryMap.hpp
./builds/game/Main/main.o: Map/Map.hpp ./octolib/includes/Array2D.hpp
./builds/game/Main/main.o: ./octolib/includes/Array2D.hxx Physics/Tile.hpp
./builds/game/Main/main.o: Physics/Polygon.hpp Managers/DecorManager.hpp
./builds/game/Main/main.o: Managers/CollisionManager.hpp Physics/Player.hpp
./builds/game/Main/main.o: Physics/DynamicPolygon.hpp
