TARGET = octodyssey.app
DIRS = Main Screens Map Decors Physics Game Biomes Bubble Menu GameObjects NanoRobots Npcs Steam
CORE_DIR = ./octolib
INCLUDE_DIR = $(CORE_DIR)/includes $(DIRS)
BUILD_DIR = ./builds/game
OUTPUT_DIR = .
# libraries directories (ex: ../libft)
LIB_DIRS = $(CORE_DIR)

# libraries (ex: ft, mlx, ...)
LIBS = octo sfml-system sfml-window sfml-graphics sfml-audio

# sources
SRC = $(SRC_PHYSICS)									\
	  $(SRC_STATES)										\
	  $(SRC_GAME)										\
	  $(SRC_GAMEOBJ)									\
	  $(SRC_NPCS)										\
	  $(SRC_NAROBOT)									\
	  $(SRC_MAP)										\
	  $(SRC_OCTO)										\
	  $(SRC_BIOMES)										\
	  $(SRC_DECORS)										\
	  $(SRC_BUBBLE)										\
	  $(SRC_MENU)										\
	  $(SRC_STEAM)										\
	  Main/DefaultApplicationListener.cpp				\
	  Main/main.cpp										\

SRC_STATES =	Screens/StateTest.cpp					\
				Screens/IntroScreen.cpp					\
				Screens/GameScreen.cpp					\
				Screens/MenuScreen.cpp					\
				Screens/ResourceLoadingScreen.cpp		\
				Screens/QuitScreen.cpp					\
				Screens/TransitionLevelScreen.cpp		\
				Screens/TransitionScreen.cpp			\
				Screens/DeathScreen.cpp					\
				Screens/TransitionLevelZeroScreen.cpp	\
				Screens/LevelZeroScreen.cpp				\
				Screens/LogoScreen.cpp					\
				Screens/AnOctonautOdysseyScreen.cpp		\
				Screens/LaboratoryEndScreen.cpp			\
				Screens/LevelRocketEndScreen.cpp		\

SRC_GAME =		Game/Game.cpp							\
				Game/CharacterOcto.cpp					\
				Game/OctoSound.cpp						\
				Game/CameraMovement.cpp					\
				Game/Progress.cpp						\
				Game/MusicManager.cpp					\
				Game/InputListener.cpp					\
				Game/KonamiCode.cpp						\
				Game/UnlockEasy.cpp						\
				Game/ChallengeManager.cpp				\
				Game/Challenges.cpp						\
				Game/PostEffectLayer.cpp				\

SRC_GAMEOBJ =	GameObjects/AGameObject.cpp				\
				GameObjects/Portal.cpp					\
				GameObjects/ElevatorStream.cpp			\
				GameObjects/SpaceShip.cpp				\
				GameObjects/SimpleObject.cpp			\
				GameObjects/Bouibouik.cpp				\
				GameObjects/Tent.cpp					\
				GameObjects/Water.cpp					\
				GameObjects/InstanceDecor.cpp			\
				GameObjects/Firecamp.cpp				\
				GameObjects/Cage.cpp					\
				GameObjects/Concert.cpp					\
				GameObjects/Pyramid.cpp					\
				GameObjects/WeirdHouseSnow.cpp			\
				GameObjects/CheckPoint.cpp				\
				GameObjects/Monolith.cpp				\
				GameObjects/MonolithStep.cpp			\
				GameObjects/Door.cpp					\
				GameObjects/SmokeInstance.cpp			\
				GameObjects/FinalPortal.cpp				\
				GameObjects/RocketEnd.cpp				\
				GameObjects/Credit.cpp					\

SRC_NPCS =		Npcs/ANpc.cpp							\
				Npcs/ASpecialNpc.cpp					\
				Npcs/AIdleNpc.cpp						\
				Npcs/AWalkNpc.cpp						\
				Npcs/ASinkNpc.cpp						\
				Npcs/ASwimNpc.cpp						\
				Npcs/AFishNpc.cpp						\
				Npcs/AUniqueNpc.cpp						\
				Npcs/CedricStartNpc.cpp					\
				Npcs/FranfranNpc.cpp					\
				Npcs/JuNpc.cpp							\
				Npcs/FannyNpc.cpp						\
				Npcs/TurbanNpc.cpp						\
				Npcs/GuiNpc.cpp							\
				Npcs/PunkNpc.cpp						\
				Npcs/FatNpc.cpp							\
				Npcs/LucienNpc.cpp						\
				Npcs/IohannNpc.cpp						\
				Npcs/OldDesertStaticNpc.cpp				\
				Npcs/VinceNpc.cpp						\
				Npcs/ConstanceNpc.cpp					\
				Npcs/AmandineNpc.cpp					\
				Npcs/ClementineNpc.cpp					\
				Npcs/JeffMouffyNpc.cpp					\
				Npcs/FaustNpc.cpp						\
				Npcs/BrayouNpc.cpp						\
				Npcs/EvaNpc.cpp							\
				Npcs/PierreNpc.cpp						\
				Npcs/CanouilleNpc.cpp					\
				Npcs/PeaNpc.cpp							\
				Npcs/WolfNpc.cpp						\
				Npcs/WellKeeperNpc.cpp					\
				Npcs/JellyfishNpc.cpp					\
				Npcs/AFlyNpc.cpp						\
				Npcs/BirdRedNpc.cpp						\
				Npcs/Snowman2Npc.cpp					\
				Npcs/Snowman1Npc.cpp					\
				Npcs/Snowman3Npc.cpp					\
				Npcs/SnowGirl1Npc.cpp					\
				Npcs/SnowGirl2Npc.cpp					\
				Npcs/StrangerGirlSnowNpc.cpp			\
				Npcs/StrangerSnowNpc.cpp				\
				Npcs/BirdBlueNpc.cpp					\
				Npcs/ADisappearNpc.cpp					\
				Npcs/ForestSpirit1Npc.cpp				\
				Npcs/ForestSpirit2Npc.cpp				\
				Npcs/TVScreen.cpp						\
				Npcs/Pedestal.cpp						\
				Npcs/OverCoolNpc.cpp					\
				Npcs/FabienNpc.cpp						\
				Npcs/ScientistJu.cpp					\
				Npcs/ScientistFran.cpp					\
				Npcs/ScientistLu.cpp					\
				Npcs/ScientistCedric.cpp				\
				Npcs/ScientistNpc.cpp					\
				Npcs/ScientistOcto.cpp					\
				Npcs/CedricEndNpc.cpp					\
				Npcs/OctoDeathNpc.cpp					\
				Npcs/Rocket.cpp							\
				Npcs/LongChairNpc.cpp					\
				Npcs/LuGlitchNpc.cpp					\
				Npcs/JuGlitchNpc.cpp					\
				Npcs/FranGlitchNpc.cpp					\
				Npcs/WindowGlitchNpc.cpp				\
				Npcs/CavemanNpc.cpp						\
				Npcs/CavemanClimbingNpc.cpp				\
				Npcs/CavemanSinkNpc.cpp					\
				Npcs/ElliotNpc.cpp						\
				Npcs/ChamanMonsterNpc.cpp				\
				Npcs/BeachGuyNpc.cpp					\
				Npcs/LucieNpc.cpp						\
				Npcs/SylvieNpc.cpp						\
				Npcs/AymericNpc.cpp						\
				Npcs/MysticanouilleNpc.cpp				\
				Npcs/AnthemJungle.cpp					\
				Npcs/FlorentNpc.cpp						\
				Npcs/EngineSnow.cpp						\
				Npcs/UlaNpc.cpp							\
				Npcs/AntoineNpc.cpp						\
				Npcs/BeachBoySubNpc.cpp					\
				Npcs/BeachBoyFlyNpc.cpp					\
				Npcs/SebNpc.cpp							\
				Npcs/TiboNpc.cpp						\
				Npcs/ColumnNpc.cpp						\
				Npcs/WaterHouseBroken.cpp				\
				Npcs/JihemNpc.cpp						\
				Npcs/MariaNpc.cpp						\
				Npcs/FishRedNpc.cpp						\
				Npcs/FishBlueNpc.cpp					\
				Npcs/FishBlackNpc.cpp					\
				Npcs/FishPinkNpc.cpp					\
				Npcs/MaryvonneNpc.cpp					\
				Npcs/ClaireNpc.cpp						\
				Npcs/DesertEngine.cpp					\
				Npcs/DeepoNpc.cpp						\
				Npcs/PepetteNpc.cpp						\
				Npcs/TVFanNpc.cpp						\
				Npcs/MecanouilleNpc.cpp					\
				Npcs/TheoNpc.cpp						\
				Npcs/CedricWalkNpc.cpp					\
				Npcs/ChristianNpc.cpp					\
				Npcs/SkeletonNpc.cpp					\
				Npcs/CedricIceANpc.cpp				\
				Npcs/TvLaboNpc.cpp				\
#Script AddNpc



SRC_BUBBLE =	Bubble/ABubble.cpp						\
				Bubble/BubbleText.cpp					\
				Bubble/BubbleMenu.cpp					\
				Bubble/TextManager.cpp					\

SRC_MENU =		Menu/AMenu.cpp							\
				Menu/AMenuSelection.cpp					\
				Menu/MainMenu.cpp						\
				Menu/OptionMenu.cpp						\
				Menu/VideoMenu.cpp						\
				Menu/AudioMenu.cpp						\
				Menu/LanguageMenu.cpp					\
				Menu/KeyboardMenu.cpp					\
				Menu/MenuTypeMenu.cpp					\
				Menu/SoundVolumeMenu.cpp				\
				Menu/MusicVolumeMenu.cpp				\
				Menu/GlobalVolumeMenu.cpp				\
				Menu/ControlMenu.cpp					\
				Menu/CreditMenu.cpp						\
				Menu/YesNoMenu.cpp						\
				Menu/ResolutionMenu.cpp					\
				Menu/ResetMenu.cpp						\
				Menu/CheatCodeMenu.cpp					\
				Menu/LevelMenu.cpp						\
				Menu/PlayEndMenu.cpp					\
				Menu/NanoMenu.cpp						\
				Menu/RandomDiscoverMenu.cpp				\
				Menu/FakeMenu.cpp						\
				Menu/BalleMultiplierMenu.cpp			\
				Menu/DifficultyMenu.cpp					\
				Menu/SpiritCheatMenu.cpp				\
				Menu/EmptyMenu.cpp						\
				Menu/SpiritInfos.cpp					\

SRC_NAROBOT =	NanoRobots/FireflySwarm.cpp				\
				NanoRobots/FireflyPopulation.cpp		\
				NanoRobots/FireflyPositionBehaviors.cpp	\
				NanoRobots/NanoRobot.cpp				\
				NanoRobots/GroundTransformNanoRobot.cpp	\
				NanoRobots/RepairNanoRobot.cpp			\
				NanoRobots/RepairShipNanoRobot.cpp		\
				NanoRobots/JumpNanoRobot.cpp			\
				NanoRobots/DoubleJumpNanoRobot.cpp		\
				NanoRobots/SlowFallNanoRobot.cpp		\
				NanoRobots/WaterNanoRobot.cpp			\
				NanoRobots/BalleNanoRobot.cpp			\
				NanoRobots/SpiritNanoRobot.cpp			\
				NanoRobots/MenuNanoRobot.cpp			\
				NanoRobots/NanoEffect.cpp				\

SRC_MAP =		Map/Map.cpp								\
				Map/GroundManager.cpp					\
				Map/Noise.cpp							\
				Map/ParallaxScrolling.cpp				\
				Map/GenerativeLayer.cpp					\
				Map/Tile.cpp							\
				Map/MapInstance.cpp						\

SRC_BIOMES =	Biomes/ABiome.cpp						\
				Biomes/BiomeManager.cpp					\
				Biomes/HSL.cpp							\
				Biomes/RandomBiome.cpp					\
				Biomes/RewardsBiome.cpp					\
				Biomes/RandomGameBiome.cpp				\
				Biomes/IceABiome.cpp					\
				Biomes/IceBBiome.cpp					\
				Biomes/IceCBiome.cpp					\
				Biomes/IceDBiome.cpp					\
				Biomes/DesertABiome.cpp					\
				Biomes/DesertBBiome.cpp					\
				Biomes/DesertCBiome.cpp					\
				Biomes/DesertDBiome.cpp					\
				Biomes/JungleABiome.cpp					\
				Biomes/JungleBBiome.cpp					\
				Biomes/JungleCBiome.cpp					\
				Biomes/JungleDBiome.cpp					\
				Biomes/WaterABiome.cpp					\
				Biomes/WaterBBiome.cpp					\
				Biomes/WaterCBiome.cpp					\
				Biomes/WaterDBiome.cpp					\
				Biomes/FinalBiome.cpp					\
				Biomes/RedBiome.cpp						\
				Biomes/BlueBiome.cpp					\
				Biomes/PortalBiome.cpp					\
				Biomes/EndRocketBiome.cpp				\
				Biomes/EndTimeLapseBiome.cpp			\
				Biomes/LaboBiome.cpp					\

SRC_DECORS =	Decors/DecorManager.cpp					\
				Decors/SkyManager.cpp					\
				Decors/SkyCycle.cpp						\
				Decors/ADecor.cpp						\
				Decors/RandomGenerator.cpp				\
				Decors/DecorAnimator.cpp				\
				Decors/Crystal.cpp						\
				Decors/AShineBuilder.cpp				\
				Decors/ShineEffect.cpp					\
				Decors/Star.cpp							\
				Decors/Rainbow.cpp						\
				Decors/Cloud.cpp						\
				Decors/Rock.cpp							\
				Decors/Tree.cpp							\
				Decors/Sun.cpp							\
				Decors/Moon.cpp							\
				Decors/Mushroom.cpp						\
				Decors/GroundRock.cpp					\
				Decors/Sky.cpp							\
				Decors/SunLight.cpp						\
				Decors/Grass.cpp						\
				Decors/Lightning.cpp					\
				Decors/DropSystem.cpp					\
				Decors/SmokeSystem.cpp					\
				Decors/MusicSystem.cpp					\
				Decors/SparkSystem.cpp					\
				Decors/HelmetSystem.cpp					\
				Decors/PloufSystem.cpp					\
				Decors/WaterDropSystem.cpp				\
				Decors/StarSystem.cpp					\
				Decors/WaterCascadeSystem.cpp			\
				Decors/BeamSystem.cpp					\

SRC_PHYSICS =	Physics/PolygonShape.cpp				\
				Physics/RectangleShape.cpp				\
				Physics/ConvexShape.cpp					\
				Physics/CircleShape.cpp					\
				Physics/TileShape.cpp					\
				Physics/PhysicsEngine.cpp				\
				Physics/ShapeBuilder.cpp				\
				Physics/GroupShape.cpp					\
				Physics/AShape.cpp						\

SRC_STEAM =		Steam/SteamAPI.cpp						\


# package files
LOADING_PCK_FILE = loading.pck
DEFAULT_PCK_FILE = default.pck
TARGET_HPP_FILE  = Main/ResourceDefinitions.hpp
LOADING_HPP_FILE = $(BUILD_DIR)/LoadingDefinitions.hpp
DEFAULT_HPP_FILE = $(BUILD_DIR)/DefaultDefinitions.hpp
# resources directory
RESOURCES_DIR = ./resources
# resources sub directory
LOADING_SRC = $(RESOURCES_DIR)/Loading/*
DEFAULT_SRC = $(RESOURCES_DIR)/Sound/*			\
			  $(RESOURCES_DIR)/Image/*			\
			  $(RESOURCES_DIR)/Color/*			\
			  $(RESOURCES_DIR)/Map/*			\
			  $(RESOURCES_DIR)/SpriteSheet/*	\
			  $(RESOURCES_DIR)/Shader/*			\
			  $(RESOURCES_DIR)/Text/*			\

# compiler
COMPILER = $(CXX)
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
RUN_DEPEND = "1"

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

$(COMPLETE_TARGET): $(BUILD_DIR) package core_library depend $(OBJS)
	@echo " - $(COLOR_ACTION)building$(COLOR_OFF): $(COLOR_OBJECT)$@$(COLOR_OFF)"
	@$(COMPILER) $(CFLAGS) $(OBJS) -o $@ $(CLIBS_FLAGS) -lsteam_api

$(addprefix $(BUILD_DIR)/, %.o) : $(subst $(BUILD_DIR),, %.cpp)
	@echo " - $(COLOR_ACTION)compiling$(COLOR_OFF): $(COLOR_OBJECT)$<$(COLOR_OFF)"
	@mkdir -p $(dir $@)
	@$(COMPILER) $(INCLUDE_DIRS) $(CFLAGS) -c $< -o $@

re: print_summary fclean $(COMPLETE_TARGET)

depend:
ifeq ($(RUN_DEPEND), "1")
	@echo " - $(COLOR_ACTION)Running hatedepend...$(COLOR_OFF)"
	@hatedepend -r -I $(DIRS) -S $(DIRS) -O $(BUILD_DIR) --signal-all
endif

fclean: clean
	@echo " - $(COLOR_ACTION)removing$(COLOR_OFF): $(COLOR_OBJECT)$(TARGET)$(COLOR_OFF)"
	@rm -f $(COMPLETE_TARGET)
	@rm -f $(LOADING_PCK_FILE) $(DEFAULT_PCK_FILE)
	@rm -f $(TARGET_HPP_FILE)
	
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
	@echo " - Making $(TARGET) [DEBUG]: $(CFLAGS) | $(COMPILER)"
else
	@echo " - Making $(TARGET): $(CFLAGS) | $(COMPILER)"
endif

core_library:
	@make -s -C $(CORE_DIR) MODE=$(MODE) RUN_DEPEND=$(RUN_DEPEND)

clean_core_library:
	@make -s -C $(CORE_DIR) clean MODE=$(MODE) RUN_DEPEND=$(RUN_DEPEND)

fclean_core_library:
	@make -s -C $(CORE_DIR) fclean MODE=$(MODE) RUN_DEPEND=$(RUN_DEPEND)


package: $(LOADING_PCK_FILE) $(DEFAULT_PCK_FILE) $(TARGET_HPP_FILE)

$(LOADING_PCK_FILE):
	@echo " - $(COLOR_ACTION)creating package $(COLOR_OFF): $(LOADING_PCK_FILE)"
	$(PACKAGER) $(LOADING_PCK_FILE) -h $(LOADING_HPP_FILE) $(LOADING_SRC)

$(DEFAULT_PCK_FILE):
	@echo " - $(COLOR_ACTION)creating package $(COLOR_OFF): $(DEFAULT_PCK_FILE)"
	$(PACKAGER) $(DEFAULT_PCK_FILE) -h $(DEFAULT_HPP_FILE) $(DEFAULT_SRC)

$(TARGET_HPP_FILE):
	@echo " - $(COLOR_ACTION)creating file $(COLOR_OFF): $(TARGET_HPP_FILE)"
	@cat $(DEFAULT_HPP_FILE) >> $(TARGET_HPP_FILE)
	@cat $(LOADING_HPP_FILE) >> $(TARGET_HPP_FILE)

complete:
	make complete -C octolib/ MODE=$(MODE) RUN_DEPEND=$(RUN_DEPEND)
	make re

static-check:
	@cppcheck -q --inline-suppr --enable=warning --error-exitcode=1 --std=c++11 --force --platform=unix64 $(addprefix -I, $(DIRS)) -I/usr/local/include/ $(DIRS)

