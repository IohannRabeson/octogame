#include "GroundManager.hpp"

#include "ABiome.hpp"
#include "TileShape.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include "MapInstance.hpp"
#include "SkyCycle.hpp"
#include "Progress.hpp"

//Decors
#include "ADecor.hpp"
#include "Rainbow.hpp"
#include "Rock.hpp"
#include "Tree.hpp"
#include "Mushroom.hpp"
#include "Crystal.hpp"
#include "GroundRock.hpp"
#include "Grass.hpp"

//Object
#include "SpaceShip.hpp"
#include "Bouibouik.hpp"
#include "WeirdHouseSnow.hpp"
#include "Tent.hpp"
#include "Pyramid.hpp"
#include "Concert.hpp"
#include "Firecamp.hpp"
#include "Cage.hpp"
#include "Monolith.hpp"

//Npc
#include "CedricStartNpc.hpp"
//Script AddNpc Include
#include "CedricIceANpc.hpp"
#include "SkeletonNpc.hpp"
#include "ChristianNpc.hpp"
#include "CedricWalkNpc.hpp"
#include "TheoNpc.hpp"
#include "MecanouilleNpc.hpp"
#include "TVFanNpc.hpp"
#include "PepetteNpc.hpp"
#include "DeepoNpc.hpp"
#include "DesertEngine.hpp"
#include "ClaireNpc.hpp"
#include "MaryvonneNpc.hpp"
#include "FishPinkNpc.hpp"
#include "FishBlackNpc.hpp"
#include "FishBlueNpc.hpp"
#include "FishRedNpc.hpp"
#include "MariaNpc.hpp"
#include "JihemNpc.hpp"
#include "WaterHouseBroken.hpp"
#include "ColumnNpc.hpp"
#include "TiboNpc.hpp"
#include "SebNpc.hpp"
#include "BeachBoyFlyNpc.hpp"
#include "BeachBoySubNpc.hpp"
#include "AntoineNpc.hpp"
#include "UlaNpc.hpp"
#include "EngineSnow.hpp"
#include "FlorentNpc.hpp"
#include "CavemanSinkNpc.hpp"
#include "AnthemJungle.hpp"
#include "MysticanouilleNpc.hpp"
#include "AymericNpc.hpp"
#include "SylvieNpc.hpp"
#include "LucieNpc.hpp"
#include "BeachGuyNpc.hpp"
#include "ChamanMonsterNpc.hpp"
#include "ScientistCedric.hpp"
#include "ScientistLu.hpp"
#include "ScientistFran.hpp"
#include "ScientistJu.hpp"
#include "WindowGlitchNpc.hpp"
#include "FranGlitchNpc.hpp"
#include "JuGlitchNpc.hpp"
#include "LuGlitchNpc.hpp"
#include "LongChairNpc.hpp"
#include "Rocket.hpp"
#include "OctoDeathNpc.hpp"
#include "CedricEndNpc.hpp"
#include "TVScreen.hpp"
#include "FabienNpc.hpp"
#include "CheckPoint.hpp"
#include "SmokeInstance.hpp"
#include "Door.hpp"
#include "OverCoolNpc.hpp"
#include "Pedestal.hpp"
#include "ForestSpirit2Npc.hpp"
#include "ForestSpirit1Npc.hpp"
#include "BirdBlueNpc.hpp"
#include "StrangerSnowNpc.hpp"
#include "StrangerGirlSnowNpc.hpp"
#include "SnowGirl2Npc.hpp"
#include "SnowGirl1Npc.hpp"
#include "Snowman3Npc.hpp"
#include "Snowman2Npc.hpp"
#include "Snowman1Npc.hpp"
#include "FranfranNpc.hpp"
#include "JuNpc.hpp"
#include "FannyNpc.hpp"
#include "TurbanNpc.hpp"
#include "GuiNpc.hpp"
#include "PunkNpc.hpp"
#include "FatNpc.hpp"
#include "LucienNpc.hpp"
#include "BrayouNpc.hpp"
#include "EvaNpc.hpp"
#include "IohannNpc.hpp"
#include "ClementineNpc.hpp"
#include "ConstanceNpc.hpp"
#include "FaustNpc.hpp"
#include "CanouilleNpc.hpp"
#include "AmandineNpc.hpp"
#include "JeffMouffyNpc.hpp"
#include "JellyfishNpc.hpp"
#include "BirdRedNpc.hpp"
#include "OldDesertStaticNpc.hpp"
#include "WellKeeperNpc.hpp"
#include "VinceNpc.hpp"
#include "WolfNpc.hpp"
#include "PeaNpc.hpp"
#include "PierreNpc.hpp"
#include "CavemanNpc.hpp"
#include "CavemanClimbingNpc.hpp"
#include "ElliotNpc.hpp"

//NanoRobots
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
#include "RepairShipNanoRobot.hpp"
#include "JumpNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "SlowFallNanoRobot.hpp"
#include "WaterNanoRobot.hpp"
#include "BalleNanoRobot.hpp"
#include "SpiritNanoRobot.hpp"

#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>
#include <limits>

GroundManager::GroundManager(void) :
	m_tiles(nullptr),
	m_tilesPrev(nullptr),
	m_transitionTimer(0.f),
	m_transitionTimerMax(0.4f),
	m_offset(),
	m_vertices(nullptr),
	m_verticesCount(0u),
	m_oldOffset(0, 0),
	//TODO:Estimate what we need
	m_decorManagerBack(200000),
	m_decorManagerFront(50000),
	m_decorManagerGround(15000),
	m_decorManagerInstanceBack(100000),
	m_decorManagerInstanceFront(100000),
	m_decorManagerInstanceGround(50000),
	m_nextState(GenerationState::None),
	m_water(nullptr)
{}

void GroundManager::setup(ABiome & biome, SkyCycle & cycle)
{
	m_mapSize = biome.getMapSize();

	// Init maps
	m_tiles.reset(new Map());
	m_tilesPrev.reset(new Map());
	m_tiles->init(biome);
	m_tilesPrev->init(biome);

	// Set pointer to the offset of the camera
	m_tiles->setCameraView(&m_offset);
	m_tilesPrev->setCameraView(&m_offset);

	// Init vertices
	m_vertices.reset(new sf::Vertex[m_tiles->getRows() * m_tiles->getColumns() * 4u]);

	// Init physics
	ShapeBuilder & builder = PhysicsEngine::getShapeBuilder();
	m_tileShapes.resize(m_tiles->getColumns(), 2u, nullptr);
	PhysicsEngine::getInstance().setTileMapSize(sf::Vector2i(m_tiles->getColumns(), 2u/*m_tiles->getRows()*/));

	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		for (std::size_t y = 0u; y < 2u; y++)
		{
			m_tileShapes.get(x, y) = builder.createTile(x, y);
			m_tileShapes.get(x, y)->setVertex(&m_vertices[0u]);
			m_tileShapes.get(x, y)->setEndVertex(&m_vertices[0u]);
			m_tileShapes.get(x, y)->setGameObject(nullptr);
		}
	}

	m_transitionTimerMax = biome.getTransitionDuration();
	m_transitionTimer = m_transitionTimerMax;

	// Init decors
	setupDecors(biome, cycle);

	// Init game objects
	setupGameObjects(biome);

	sf::Rect<float> const & rect = octo::Application::getCamera().getRectangle();
	m_offset.x = rect.left;
	m_offset.y = rect.top;
	//m_oldOffset = sf::Vector2i(m_offset / 16.f);
	updateOffset(0.f);

	//avoid empty columns
	//TODO: Find if the problem is in swapMp
	for (std::size_t i = 0u; i <= 1u; i++)
		swapMap();
}

//TODO: Use movement mask
void GroundManager::setupGroundRock(ABiome & biome)
{
	std::vector<sf::Vector2f> positions;
	auto const & instances = biome.getInstances();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	for (auto & instance : instances)
	{
		octo::LevelMap const & levelMap = resources.getLevelMap(instance.second);
		octo::Array3D<octo::LevelMap::TileType> const & map = levelMap.getMap();
		for (std::size_t y = 0u; y < map.rows(); y++)
		{
			for (std::size_t x = 0u; x < map.columns(); x++)
			{
				if (biome.randomBool(0.01f) && map(x, y, 0) == octo::LevelMap::TileType::Square)
				{
					bool isEmpty = false;
					sf::Vector2f position(x * Tile::TileSize, y * Tile::TileSize);
					position.x += instance.first * Tile::TileSize - Map::OffsetX;
					position.y += (-levelMap.getMapSize().y + levelMap.getMapPosY()) * Tile::TileSize - Map::OffsetY;
					for (std::size_t z = 0u; z < map.depth(); z++)
					{
						if (map(x, y, z) != octo::LevelMap::TileType::Square)
							isEmpty = true;
					}
					if (isEmpty == false)
						positions.push_back(position);
				}
			}
		}
	}

	for (auto & pos : positions)
	{
		ADecor * adecor = nullptr;
		adecor = new GroundRock(true);
		adecor->setPosition(sf::Vector2f(pos.x, pos.y + Tile::TileSize));
		m_decorManagerInstanceGround.add(adecor);
	}
}

void GroundManager::setupGameObjects(ABiome & biome)
{
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	setupGroundRock(biome);

	m_npcFactory.registerCreator<FranfranNpc>(FRANFRAN_OSS);
	m_npcFactory.registerCreator<JuNpc>(JU_OSS);
	m_npcFactory.registerCreator<GuiNpc>(GUILLAUME_OSS);
	m_npcFactory.registerCreator<ConstanceNpc>(CONSTANCE_OSS);
	m_npcFactory.registerCreator<FaustNpc>(NPC_FAUST_OSS);
	m_npcFactory.registerCreator<AmandineNpc>(AMANDINE_OSS);
	m_npcFactory.registerCreator<PeaNpc>(PEA_OSS);
	m_npcFactory.registerCreator<Snowman2Npc>(SNOWMAN_2_OSS);
	m_npcFactory.registerCreator<PunkNpc>(NPC_PUNK_OSS);
	m_npcFactory.registerCreator<FatNpc>(NPC_FAT_OSS);
	m_npcFactory.registerCreator<BirdRedNpc>(BIRD_RED_OSS);
	m_npcFactory.registerCreator<LucienNpc>(LUCIEN_OSS);
	m_npcFactory.registerCreator<PierreNpc>(PIERRE_OSS);
	m_npcFactory.registerCreator<IohannNpc>(IOHANN_OSS);
	m_npcFactory.registerCreator<ClementineNpc>(CLEMENTINE_OSS);
	m_npcFactory.registerCreator<WolfNpc>(WOLF_OSS);
	m_npcFactory.registerCreator<FannyNpc>(FANNY_OSS);
//Script AddNpc Factory
	m_npcFactory.registerCreator<CedricIceANpc>(CEDRIC_ICE_A_OSS);
	m_npcFactory.registerCreator<SkeletonNpc>(SKELETON_1_OSS);
	m_npcFactory.registerCreator<ChristianNpc>(CHRISTIAN_OSS);
	m_npcFactory.registerCreator<CedricWalkNpc>(CEDRIC_WALK_OSS);
	m_npcFactory.registerCreator<TheoNpc>(THEO_OSS);
	m_npcFactory.registerCreator<MecanouilleNpc>(NPC_MECANOUILLE_OSS);
	m_npcFactory.registerCreator<TVFanNpc>(TV_FAN_OSS);
	m_npcFactory.registerCreator<DeepoNpc>(DEEPO_OSS);
	m_npcFactory.registerCreator<DesertEngine>(DESERT_ENGINE_1_OSS);
	m_npcFactory.registerCreator<ClaireNpc>(CLAIRE_OSS);
	m_npcFactory.registerCreator<MaryvonneNpc>(MARYVONNE_OSS);
	m_npcFactory.registerCreator<MariaNpc>(MARIA_OSS);
	m_npcFactory.registerCreator<JihemNpc>(JIHEM_OSS);
	m_npcFactory.registerCreator<WaterHouseBroken>(WATER_HOUSE_BROKEN_OSS);
	m_npcFactory.registerCreator<ColumnNpc>(COLUMN_1_OSS);
	m_npcFactory.registerCreator<TiboNpc>(TIBO_OSS);
	m_npcFactory.registerCreator<BeachBoyFlyNpc>(BEACHBOY_FLY_OSS);
	m_npcFactory.registerCreator<AntoineNpc>(ANTOINE_OSS);
	m_npcFactory.registerCreator<UlaNpc>(ULA_OSS);
	m_npcFactory.registerCreator<EngineSnow>(ENGINE_SNOW_OSS);
	m_npcFactory.registerCreator<FlorentNpc>(FLORENT_OSS);
	m_npcFactory.registerCreator<AnthemJungle>(ANTHEM_JUNGLE_OSS);
	m_npcFactory.registerCreator<MysticanouilleNpc>(MYSTICANOUILLE_OSS);
	m_npcFactory.registerCreator<AymericNpc>(AYMERIC_OSS);
	m_npcFactory.registerCreator<LucieNpc>(LUCIE_OSS);
	m_npcFactory.registerCreator<BeachGuyNpc>(BEACH_GUY_OSS);
	m_npcFactory.registerCreator<ChamanMonsterNpc>(CHAMAN_MONSTER_OSS);
	m_npcFactory.registerCreator<ScientistCedric>(SCIENTISTCEDRIC_OSS);
	m_npcFactory.registerCreator<ScientistLu>(SCIENTISTLU_OSS);
	m_npcFactory.registerCreator<ScientistFran>(SCIENTISTFRAN_OSS);
	m_npcFactory.registerCreator<ScientistJu>(SCIENTISTJU_OSS);
	m_npcFactory.registerCreator<WindowGlitchNpc>(WINDOW_GLITCH_OSS);
	m_npcFactory.registerCreator<FranGlitchNpc>(FRAN_GLITCH_OSS);
	m_npcFactory.registerCreator<JuGlitchNpc>(JU_GLITCH_OSS);
	m_npcFactory.registerCreator<LuGlitchNpc>(LU_GLITCH_OSS);
	m_npcFactory.registerCreator<LongChairNpc>(NPC_LONGCHAIR_OSS);
	m_npcFactory.registerCreator<Rocket>(ROCKET_OSS);
	m_npcFactory.registerCreator<FabienNpc>(FABIEN_OSS);
	m_npcFactory.registerCreator<OverCoolNpc>(OVER_COOL_NPC_OSS);
	m_npcFactory.registerCreator<Pedestal>(PEDESTAL_OSS);
	m_npcFactory.registerCreator<ForestSpirit2Npc>(FOREST_SPIRIT_2_OSS);
	m_npcFactory.registerCreator<ForestSpirit1Npc>(FOREST_SPIRIT_1_OSS);
	m_npcFactory.registerCreator<BirdBlueNpc>(BIRD_BLUE_OSS);
	m_npcFactory.registerCreator<StrangerSnowNpc>(STRANGER_SNOW_OSS);
	m_npcFactory.registerCreator<StrangerGirlSnowNpc>(STRANGER_GIRL_SNOW_OSS);
	m_npcFactory.registerCreator<SnowGirl2Npc>(SNOWGIRL_2_OSS);
	m_npcFactory.registerCreator<SnowGirl1Npc>(SNOWGIRL_1_OSS);
	m_npcFactory.registerCreator<Snowman3Npc>(SNOWMAN_3_OSS);
	m_npcFactory.registerCreator<Snowman1Npc>(SNOWMAN_1_OSS);
	m_npcFactory.registerCreator<WellKeeperNpc>(NPC_WELL_KEEPER_OSS);
	m_npcFactory.registerCreator<BrayouNpc>(BRAYOU_OSS);
	m_npcFactory.registerCreator<JeffMouffyNpc>(JEFF_MOUFFY_OSS);

	//ASwimNpc
	m_npcFactory.registerCreator(FISH_PINK_OSS, [&biome](){ return new FishPinkNpc(biome); });
	m_npcFactory.registerCreator(FISH_RED_OSS, [&biome](){ return new FishRedNpc(biome); });
	m_npcFactory.registerCreator(FISH_BLACK_OSS, [&biome](){ return new FishBlackNpc(biome); });
	m_npcFactory.registerCreator(FISH_BLUE_OSS, [&biome](){ return new FishBlueNpc(biome); });
	m_npcFactory.registerCreator(BEACHBOY_SUB_OSS, [&biome](){ return new BeachBoySubNpc(biome); });
	m_npcFactory.registerCreator(SYLVIE_OSS, [&biome](){ return new SylvieNpc(biome); });
	m_npcFactory.registerCreator(PEPETTE_OSS, [&biome](){ return new PepetteNpc(biome); });
	m_npcFactory.registerCreator(SEB_OSS, [&biome](){ return new SebNpc(biome); });
	m_npcFactory.registerCreator(JELLYFISH_OSS, [&biome](){ return new JellyfishNpc(biome); });

	//AUniqueNpc
	m_npcFactory.registerCreator(OCTO_DEATH_HELMET_OSS, [&biome](){ return new OctoDeathNpc(biome.getWaterLevel(), biome.getWaterColor()); });
	m_npcFactory.registerCreator(EVA_OSS, [&biome](){ return new EvaNpc(biome.getWaterColor()); });
	m_npcFactory.registerCreator(CEDRIC_START_OSS, [&biome](){ return new CedricStartNpc(biome.getType()); });
	m_npcFactory.registerCreator(CEDRIC_END_OSS, [&biome](){ return new CedricEndNpc(biome.getType()); });
	m_npcFactory.registerCreator(TV_BLACK_OSS, [](){ return new TVScreen("render_black_kernel"); });
	m_npcFactory.registerCreator(TV_WHITE_OSS, [](){ return new TVScreen("render_white_kernel"); });
	m_npcFactory.registerCreator(CAVEMAN_OSS, [](){ return new CavemanNpc(); });
	m_npcFactory.registerCreator(CAVEMAN_SINKING_OSS, [&biome](){ return new CavemanSinkNpc(biome.getWaterLevel(), biome.getWaterColor()); });
	m_npcFactory.registerCreator(ELLIOT_OSS, [](){ return new ElliotNpc(); });

	octo::GenericFactory<std::string, InstanceDecor, sf::Vector2f const &, sf::Vector2f const &>	m_decorFactory;
	m_decorFactory.registerCreator(DOOR_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new Door(scale, position);
			});
	m_decorFactory.registerCreator(CHECKPOINT_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new CheckPoint(scale, position);
			});
	m_decorFactory.registerCreator(SMOKE_INSTANCE_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new SmokeInstance(scale, position);
			});
	m_decorFactory.registerCreator(HOUSE_ORANGE_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_ORANGE_OSS, scale, position, 3u);
			});
	m_decorFactory.registerCreator(HOUSE_GREEN_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_GREEN_OSS, scale, position, 5u);
			});
	m_decorFactory.registerCreator(HOUSE_RANDOM_2_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_RANDOM_2_OSS, scale, position, 2u);
			});
	m_decorFactory.registerCreator(HOUSE_RANDOM_1_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_RANDOM_1_OSS, scale, position, 2u);
			});
	m_decorFactory.registerCreator(HOUSE_RANDOM_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_RANDOM_OSS, scale, position, 3u);
			});
	m_decorFactory.registerCreator(HOUSE_LIGHT_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_LIGHT_OSS, scale, position, 3u);
			});
	m_decorFactory.registerCreator(HOUSE_PUSSY_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_PUSSY_OSS, scale, position, 4u);
			});
	m_decorFactory.registerCreator(FALL_SIGN_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(FALL_SIGN_OSS, scale, position, 4u, 0.4f);
			});
	m_decorFactory.registerCreator(PLANT_JUNGLE_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(PLANT_JUNGLE_OSS, scale, position, 3u, 0.4f);
			});
	m_decorFactory.registerCreator(FIRECAMP_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new Firecamp(scale, position);
			});
	m_decorFactory.registerCreator(CAGE_FRONT_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new Cage(scale, position);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_1_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_1_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_2_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_2_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_3_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_3_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_4_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_4_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_5_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_5_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_6_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_6_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_7_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_7_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_8_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_8_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_9_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_9_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TRAIL_SIGN_10_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TRAIL_SIGN_10_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(SIGN_DESERT_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(SIGN_DESERT_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TV_FAN_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TV_FAN_OSS, scale, position, 8u, 0.4f);
			});
	m_decorFactory.registerCreator(PYRAMID_OSS, [&biome](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new Pyramid(scale, position, biome);
			});
	m_decorFactory.registerCreator(PARA_SIGN_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(PARA_SIGN_OSS, scale, position, 4u, 0.4f);
			});
	m_decorFactory.registerCreator(COLUMN_1_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(COLUMN_1_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(COLUMN_2_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(COLUMN_2_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(COLUMN_3_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(COLUMN_3_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(COLUMN_4_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(COLUMN_4_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(COLUMN_5_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(COLUMN_5_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(COLUMN_KONAMI_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(COLUMN_KONAMI_OSS, scale, position, 2u, 0.5f);
			});
	m_decorFactory.registerCreator(PLANT_JUNGLE_2_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(PLANT_JUNGLE_2_OSS, scale, position, 3u, 0.3f);
			});
	m_decorFactory.registerCreator(DOUBLE_JUMP_SIGN_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(DOUBLE_JUMP_SIGN_OSS, scale, position, 4u, 0.4f);
			});
	m_decorFactory.registerCreator(SKELETON_1_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(SKELETON_1_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(SKELETON_2_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(SKELETON_2_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(SKELETON_3_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(SKELETON_3_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(SKELETON_4_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(SKELETON_4_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(HOUSE_SNOW_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_SNOW_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(HUGE_FLUE_SNOW_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HUGE_FLUE_SNOW_OSS, scale, position, 1u, 0.4f);
			});
	m_decorFactory.registerCreator(TEMPLE_DESERT_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(TEMPLE_DESERT_OSS, scale, position, 3u, 0.2f);
			});
	m_decorFactory.registerCreator(PIPE_1_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(PIPE_1_OSS, scale, position, 1u, 0.2f);
			});
	m_decorFactory.registerCreator(PIPE_2_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(PIPE_2_OSS, scale, position, 1u, 0.2f);
			});
	m_decorFactory.registerCreator(POTION_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(POTION_OSS, scale, position, 1u, 0.2f);
			});
	m_decorFactory.registerCreator(REFINERY_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(REFINERY_OSS, scale, position, 4u, 0.2f);
			});
	m_decorFactory.registerCreator(WATER_CASTLE_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(WATER_CASTLE_OSS, scale, position, 1u, 0.2f);
			});
	m_decorFactory.registerCreator(CAVEMAN_HOUSE_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(CAVEMAN_HOUSE_OSS, scale, position, 4u, 0.2f);
			});
	m_decorFactory.registerCreator(CAVEMAN_CLIMBING_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(CAVEMAN_CLIMBING_OSS, scale, position, 6u, 0.1f);
			});
	m_decorFactory.registerCreator(MONOLITH_OSS, [&biome](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new Monolith(scale, position, biome);
			});
	m_decorFactory.registerCreator(WEIRD_HOUSE_SNOW_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(WEIRD_HOUSE_SNOW_OSS, scale, position, 1u, 0.1f);
			});
	m_decorFactory.registerCreator(HOUSE_FLAT_SNOW_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(HOUSE_FLAT_SNOW_OSS, scale, position, 1u, 0.1f);
			});
	m_decorFactory.registerCreator(WATER_HOUSE_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(WATER_HOUSE_OSS, scale, position, 1u, 0.1f);
			});
	m_decorFactory.registerCreator(WELL_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(WELL_OSS, scale, position, 1u, 0.1f);
			});
	m_decorFactory.registerCreator(WATER_HOUSE_BROKEN_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(WATER_HOUSE_BROKEN_OSS, scale, position, 1u, 0.1f);
			});
	m_decorFactory.registerCreator(OBJECT_ELEVATOR_BOTTOM_FRONT_OSS, [&biome](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new ElevatorStream(scale, position, biome, true);
			});
	m_decorFactory.registerCreator(ROCKET_RAMP_OSS, [](sf::Vector2f const & scale, sf::Vector2f const & position)
			{
				return new InstanceDecor(ROCKET_RAMP_OSS, scale, position, 1u, 0.1f);
			});

	// Get all the gameobjects from instances
	auto const & instances = biome.getInstances();
	for (auto & instance : instances)
	{
		octo::LevelMap const & levelMap = resources.getLevelMap(instance.second);
		for (std::size_t i = 0u; i < levelMap.getSpriteCount(); i++)
		{
			octo::LevelMap::SpriteTrigger const & spriteTrigger = levelMap.getSprite(i);
			sf::FloatRect rect;
			rect.left = spriteTrigger.trigger.left + instance.first * Tile::TileSize - Map::OffsetX;
			rect.top = (-levelMap.getMapSize().y + levelMap.getMapPosY()) * Tile::TileSize + spriteTrigger.trigger.top - Map::OffsetY;
			rect.width = spriteTrigger.trigger.width;
			rect.height = spriteTrigger.trigger.height;
			sf::Vector2f position(rect.left, rect.top + rect.height);
			if (!spriteTrigger.name.compare(NANO_JUMP_OSS))
			{
				if (!Progress::getInstance().canJump())
				{
					std::unique_ptr<NanoRobot> ptr;
					ptr.reset(new JumpNanoRobot(position));
					m_nanoRobotOnInstance.push_back(std::move(ptr));
				}
			}
			else if (!spriteTrigger.name.compare(NANO_DOUBLE_JUMP_OSS))
			{
				if (!Progress::getInstance().canDoubleJump())
				{
					std::unique_ptr<NanoRobot> ptr;
					ptr.reset(new DoubleJumpNanoRobot(position));
					m_nanoRobotOnInstance.push_back(std::move(ptr));
				}
			}
			else if (!spriteTrigger.name.compare(NANO_BALLE_OSS))
			{
				if (!Progress::getInstance().canUseBalle())
				{
					std::unique_ptr<NanoRobot> ptr;
					ptr.reset(new BalleNanoRobot(position));
					m_nanoRobotOnInstance.push_back(std::move(ptr));
				}
			}
			else if (!spriteTrigger.name.compare(NANO_REPAIR_SHIP_OSS))
			{
				if (!Progress::getInstance().canRepairShip())
				{
					std::unique_ptr<NanoRobot> ptr;
					ptr.reset(new RepairShipNanoRobot(position));
					m_nanoRobotOnInstance.push_back(std::move(ptr));
				}
			}
			else if (!spriteTrigger.name.compare(NANO_SLOW_FALL_OSS))
			{
				if (!Progress::getInstance().canSlowFall())
				{
					std::unique_ptr<NanoRobot> ptr;
					ptr.reset(new SlowFallNanoRobot(position));
					m_nanoRobotOnInstance.push_back(std::move(ptr));
				}
			}
			else
			{
				std::unique_ptr<ANpc> npc;
				npc.reset(m_npcFactory.create(spriteTrigger.name.c_str()));
				npc->setArea(rect);
				npc->setPosition(position);
				m_npcs.push_back(std::move(npc));
			}
		}

		// Get all decors
		bool spawnElevator = false;
		for (std::size_t i = 0u; i < levelMap.getDecorCount(); i++)
		{
			octo::LevelMap::Decor decor = resources.getLevelMap(instance.second).getDecor(i);
			sf::Vector2f position = decor.position;
			position.x += instance.first * Tile::TileSize - Map::OffsetX;
			position.y += (-levelMap.getMapSize().y + levelMap.getMapPosY()) * Tile::TileSize - Map::OffsetY;

			if (!decor.name.substr(0, 13).compare("object_portal"))
			{
				std::unique_ptr<Portal> portal;
				if (!decor.name.compare("object_portal_red.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_RED_OSS, "vortex_red", sf::Color(155, 0, 0)));
				else if (!decor.name.compare("object_portal_blue.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_BLUE_OSS, "vortex_blue", sf::Color(0, 0, 155)));
				else if (!decor.name.compare("object_portal_random.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_RANDOM_OSS, "vortex_white", sf::Color(255, 255, 255)));
				else if (!decor.name.compare("object_portal_snow.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_SNOW_OSS, VORTEX_FRAG));
				else if (!decor.name.compare("object_portal_desert.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_DESERT_OSS, VORTEX_FRAG));
				else if (!decor.name.compare("object_portal_jungle.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_JUNGLE_OSS, VORTEX_FRAG));
				else if (!decor.name.compare("object_portal_beach.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_BEACH_OSS, VORTEX_FRAG));
				else if (!decor.name.compare("object_portal.oss"))
					portal.reset(new Portal(biome.getDestination(), OBJECT_PORTAL_OSS, VORTEX_FRAG));
				portal->setBiome(biome);
				portal->setPosition(position + sf::Vector2f(50.f, 350.f));
				m_otherOnInstance.push_back(std::move(portal));
			}
			else if (!decor.name.compare(OBJECT_ELEVATOR_TOP_BACK_OSS))
				spawnElevator = true;
			else if (!decor.name.substr(0, 6).compare("decor_"))
			{
				ADecor * adecor = nullptr;
				if (!decor.name.compare(DECOR_TREE_OSS))
					adecor = new Tree(true);
				else if (!decor.name.compare(DECOR_ROCK_OSS))
					adecor = new Rock();
				else if (!decor.name.compare(DECOR_CRYSTAL_OSS))
					adecor = new Crystal();
				else if (!decor.name.compare(DECOR_MUSHROOM_OSS))
					adecor = new Mushroom();
				else if (!decor.name.compare(DECOR_GROUND_OSS))
					adecor = new GroundRock(true);
				else if (!decor.name.compare(DECOR_RAINBOW_OSS))
					adecor = new Rainbow();
				else if (!decor.name.compare(DECOR_GRASS_OSS))
				{
					adecor = new Grass(true, false);
					if (biome.isDeadlyGrass())
						decor.isFront = true;
				}
				else if (!decor.name.compare(DECOR_GRASS_REVERSE_OSS))
				{
					adecor = new Grass(true, true);
					if (biome.isDeadlyGrass())
						decor.isFront = true;
				}
				if (adecor)
				{
					adecor->setPosition(sf::Vector2f(position.x, position.y + Tile::TileSize));
					if (decor.isFront)
						m_decorManagerInstanceFront.add(adecor);
					else
						m_decorManagerInstanceBack.add(adecor);
				}
			}
			else if (!decor.isFront)
				m_instanceDecors.emplace_back(std::unique_ptr<InstanceDecor>(m_decorFactory.create(decor.name, decor.scale, position)));
			else
				m_instanceDecorsFront.emplace_back(std::unique_ptr<InstanceDecor>(m_decorFactory.create(decor.name, decor.scale, position)));
		}

		bool spawnInstance = false;
		std::size_t y;
		octo::Array3D<octo::LevelMap::TileType> const & map = levelMap.getMap();
		for (y = 0u; y < map.rows(); y++)
		{
			if (map(0, y, 0) != octo::LevelMap::TileType::Empty)
			{
				spawnInstance = true;
				break;
			}
		}

		// For each instance, create an elevator stream
		if (spawnInstance && spawnElevator)
		{
			std::unique_ptr<ElevatorStream> elevator;
			elevator.reset(new ElevatorStream(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.f, 0.f), biome));
			elevator->setPosition(sf::Vector2f(instance.first * Tile::TileSize - elevator->getWidth(), 0.f));
			elevator->setTopY((static_cast<int>(y) - levelMap.getMapSize().y + levelMap.getMapPosY() - 9) * Tile::TileSize);
			elevator->setHeight(400.f);
			std::size_t width = elevator->getWidth() / Tile::TileSize + 2u;
			m_elevators.emplace_back(instance.first - width, width, elevator);
		}
	}

	std::vector<sf::Vector2i> & deathPos = Progress::getInstance().getDeathPos();
	for (std::size_t i = 0; i < deathPos.size(); i++)
	{
		std::unique_ptr<ANpc> npc;
		npc.reset(m_npcFactory.create(OCTO_DEATH_HELMET_OSS));
		npc->setPosition(sf::Vector2f(deathPos[i].x, deathPos[i].y));
		m_npcs.push_back(std::move(npc));
	}

	auto & gameObjects = biome.getGameObjects();
	std::size_t portalCount = 0;

	for (auto & gameObject : gameObjects)
	{
		switch (gameObject.second)
		{
			case GameObjectType::PortalRandom:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_RANDOM_OSS, "vortex_white"));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;
			case GameObjectType::PortalJungle:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_JUNGLE_OSS, name.c_str()));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;
			case GameObjectType::PortalSnow:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_SNOW_OSS, name.c_str()));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;
			case GameObjectType::PortalDesert:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_DESERT_OSS, name.c_str()));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;
			case GameObjectType::PortalWater:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_BEACH_OSS, name.c_str()));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;
			case GameObjectType::Portal:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_OSS, name.c_str()));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;
			case GameObjectType::PortalRed:
				{
					std::string name = "vortex_" + std::to_string(portalCount);
					std::unique_ptr<Portal> portal(new Portal(biome.getDestination(), OBJECT_PORTAL_RED_OSS, name.c_str()));
					portal->setBiome(biome);
					m_portals.emplace_back(gameObject.first, portal->getRadius() * 2.f / Tile::TileSize, portal);
					portalCount++;
				}
				break;

			//NanoRobot
			case GameObjectType::RepairNanoRobot:
					if (!Progress::getInstance().canRepair())
						m_nanoRobots.emplace_back(gameObject.first, 3, new RepairNanoRobot());
				break;
			case GameObjectType::SlowFallNanoRobot:
					if (!Progress::getInstance().canSlowFall())
						m_nanoRobots.emplace_back(gameObject.first, 3, new SlowFallNanoRobot(sf::Vector2f(0.f, 0.f)));
			case GameObjectType::GroundTransformNanoRobot:
					if (!Progress::getInstance().canMoveMap())
						m_nanoRobots.emplace_back(gameObject.first, 3, new GroundTransformNanoRobot());
				break;
			case GameObjectType::WaterNanoRobot:
					if (!Progress::getInstance().canUseWaterJump())
						m_nanoRobots.emplace_back(gameObject.first, 3, new WaterNanoRobot());
				break;
			case GameObjectType::SpiritNanoRobot:
					m_nanoRobots.emplace_back(gameObject.first, 3, new SpiritNanoRobot(sf::Vector2f(0.f, 0.f)));
				break;

			//Object
			case GameObjectType::SpaceShip:
				{
					SpaceShip * spaceship = new SpaceShip(SpaceShip::SpaceShipEvents::Broken);
					m_otherObjectsHigh.emplace_back(gameObject.first, 15, spaceship);
				}
				break;
			case GameObjectType::Bouibouik:
				{
					Bouibouik * simple = new Bouibouik();
					m_otherObjectsLow.emplace_back(gameObject.first, 15, simple);
				}
				break;
			case GameObjectType::WeirdHouseSnow:
				{
					WeirdHouseSnow * simple = new WeirdHouseSnow();
					m_otherObjectsLow.emplace_back(gameObject.first, 15, simple);
				}
				break;
			case GameObjectType::Tent:
				{
					Tent * simple = new Tent();
					m_otherObjectsHigh.emplace_back(gameObject.first, 15, simple);
				}
				break;
			case GameObjectType::Concert:
				{
					Concert * simple = new Concert();
					m_otherObjectsHigh.emplace_back(gameObject.first, 15, simple);
				}
				break;

			//Npc
//Script AddNpc Ground
			case GameObjectType::CedricIceANpc:
				{
					CedricIceANpc * npc = new CedricIceANpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::SkeletonNpc:
				{
					SkeletonNpc * npc = new SkeletonNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::ChristianNpc:
				{
					ChristianNpc * npc = new ChristianNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::CedricWalkNpc:
				{
					CedricWalkNpc * npc = new CedricWalkNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::TheoNpc:
				{
					TheoNpc * npc = new TheoNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::MecanouilleNpc:
				{
					MecanouilleNpc * npc = new MecanouilleNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::TVFanNpc:
				{
					TVFanNpc * npc = new TVFanNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::DeepoNpc:
				{
					DeepoNpc * npc = new DeepoNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::DesertEngine:
				{
					DesertEngine * npc = new DesertEngine();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ClaireNpc:
				{
					ClaireNpc * npc = new ClaireNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::MaryvonneNpc:
				{
					MaryvonneNpc * npc = new MaryvonneNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FishPinkNpc:
				{
					FishPinkNpc * npc = new FishPinkNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FishBlackNpc:
				{
					FishBlackNpc * npc = new FishBlackNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FishBlueNpc:
				{
					FishBlueNpc * npc = new FishBlueNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FishRedNpc:
				{
					FishRedNpc * npc = new FishRedNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::MariaNpc:
				{
					MariaNpc * npc = new MariaNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::JihemNpc:
				{
					JihemNpc * npc = new JihemNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::WaterHouseBroken:
				{
					WaterHouseBroken * npc = new WaterHouseBroken();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ColumnNpc:
				{
					ColumnNpc * npc = new ColumnNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::TiboNpc:
				{
					TiboNpc * npc = new TiboNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::SebNpc:
				{
					SebNpc * npc = new SebNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::BeachBoyFlyNpc:
				{
					BeachBoyFlyNpc * npc = new BeachBoyFlyNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::BeachBoySubNpc:
				{
					BeachBoySubNpc * npc = new BeachBoySubNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::AntoineNpc:
				{
					AntoineNpc * npc = new AntoineNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::UlaNpc:
				{
					UlaNpc * npc = new UlaNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::EngineSnow:
				{
					EngineSnow * npc = new EngineSnow();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FlorentNpc:
				{
					FlorentNpc * npc = new FlorentNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::AnthemJungle:
				{
					AnthemJungle * npc = new AnthemJungle();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::MysticanouilleNpc:
				{
					MysticanouilleNpc * npc = new MysticanouilleNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::AymericNpc:
				{
					AymericNpc * npc = new AymericNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::PepetteNpc:
				{
					PepetteNpc * npc = new PepetteNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::SylvieNpc:
				{
					SylvieNpc * npc = new SylvieNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::LucieNpc:
				{
					LucieNpc * npc = new LucieNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::BeachGuyNpc:
				{
					BeachGuyNpc * npc = new BeachGuyNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ChamanMonsterNpc:
				{
					ChamanMonsterNpc * npc = new ChamanMonsterNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ElliotNpc:
				{
					ElliotNpc * npc = new ElliotNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::CavemanNpc:
				{
					CavemanNpc * npc = new CavemanNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ScientistCedric:
				{
					ScientistCedric * npc = new ScientistCedric();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ScientistLu:
				{
					ScientistLu * npc = new ScientistLu();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ScientistFran:
				{
					ScientistFran * npc = new ScientistFran();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ScientistJu:
				{
					ScientistJu * npc = new ScientistJu();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::WindowGlitchNpc:
				{
					WindowGlitchNpc * npc = new WindowGlitchNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FranGlitchNpc:
				{
					FranGlitchNpc * npc = new FranGlitchNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::JuGlitchNpc:
				{
					JuGlitchNpc * npc = new JuGlitchNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::LuGlitchNpc:
				{
					LuGlitchNpc * npc = new LuGlitchNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::LongChairNpc:
				{
					LongChairNpc * npc = new LongChairNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::Rocket:
				{
					Rocket * npc = new Rocket();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::OctoDeathNpc:
				{
					OctoDeathNpc * npc = new OctoDeathNpc(biome.getWaterLevel(), biome.getWaterColor());
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::CedricEndNpc:
				{
					CedricEndNpc * npc = new CedricEndNpc(biome.getType());
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::TVWhite:
				{
					TVScreen * npc = new TVScreen("render_white_kernel");
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::TVBlack:
				{
					TVScreen * npc = new TVScreen("render_black_kernel");
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FabienNpc:
				{
					FabienNpc * npc = new FabienNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::Door:
				{
					Door * obj = new Door(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.f, 0.f));
					m_otherObjectsLow.emplace_back(gameObject.first, 1, obj);
				}
				break;
			case GameObjectType::CheckPoint:
				{
					CheckPoint * npc = new CheckPoint(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.f, 0.f));
					m_otherObjectsLow.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::Monolith:
				{
					Monolith * npc = new Monolith(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.f, 0.f), biome);
					m_otherObjectsLow.emplace_back(gameObject.first, 30, npc);
				}
				break;
			case GameObjectType::SmokeInstance:
				{
					SmokeInstance * npc = new SmokeInstance(sf::Vector2f(1.f, 1.f), sf::Vector2f(0.f, 0.f));
					m_otherObjectsLow.emplace_back(gameObject.first, 1, npc);
				}
				break;
			case GameObjectType::OverCoolNpc:
				{
					OverCoolNpc * npc = new OverCoolNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::Pedestal:
				{
					Pedestal * npc = new Pedestal();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ForestSpirit2Npc:
				{
					ForestSpirit2Npc * npc = new ForestSpirit2Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ForestSpirit1Npc:
				{
					ForestSpirit1Npc * npc = new ForestSpirit1Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::WolfNpc:
				{
					WolfNpc * npc = new WolfNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ClementineNpc:
				{
					ClementineNpc * npc = new ClementineNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FatNpc:
				{
					FatNpc * npc = new FatNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::PunkNpc:
				{
					PunkNpc * npc = new PunkNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::Snowman1Npc:
				{
					Snowman1Npc * npc = new Snowman1Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::Snowman2Npc:
				{
					Snowman2Npc * npc = new Snowman2Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::Snowman3Npc:
				{
					Snowman3Npc * npc = new Snowman3Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::GuiNpc:
				{
					GuiNpc * npc = new GuiNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::IohannNpc:
				{
					IohannNpc * npc = new IohannNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::CedricStartNpc:
				{
					CedricStartNpc * npc = new CedricStartNpc(biome.getType());
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FranfranNpc:
				{
					FranfranNpc * npc = new FranfranNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::StrangerGirlSnowNpc:
				{
					StrangerGirlSnowNpc * npc = new StrangerGirlSnowNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::StrangerSnowNpc:
				{
					StrangerSnowNpc * npc = new StrangerSnowNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::SnowGirl1Npc:
				{
					SnowGirl1Npc * npc = new SnowGirl1Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::SnowGirl2Npc:
				{
					SnowGirl2Npc * npc = new SnowGirl2Npc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::JuNpc:
				{
					JuNpc * npc = new JuNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FannyNpc:
				{
					FannyNpc * npc = new FannyNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::TurbanNpc:
				{
					TurbanNpc * npc = new TurbanNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::ConstanceNpc:
				{
					ConstanceNpc * npc = new ConstanceNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::WellKeeperNpc:
				{
					WellKeeperNpc * npc = new WellKeeperNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::OldDesertStaticNpc:
				{
					OldDesertStaticNpc * npc = new OldDesertStaticNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::VinceNpc:
				{
					VinceNpc * npc = new VinceNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::JeffMouffyNpc:
				{
					JeffMouffyNpc * npc = new JeffMouffyNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::JellyfishNpc:
				{
					JellyfishNpc * npc = new JellyfishNpc(biome);
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::BirdRedNpc:
				{
					BirdRedNpc * npc = new BirdRedNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::BirdBlueNpc:
				{
					BirdBlueNpc * npc = new BirdBlueNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::CanouilleNpc:
				{
					CanouilleNpc * npc = new CanouilleNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::FaustNpc:
				{
					FaustNpc * npc = new FaustNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::EvaNpc:
				{
					EvaNpc * npc = new EvaNpc(biome.getWaterColor());
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::PierreNpc:
				{
					PierreNpc * npc = new PierreNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::LucienNpc:
				{
					LucienNpc * npc = new LucienNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::PeaNpc:
				{
					PeaNpc * npc = new PeaNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::BrayouNpc:
				{
					BrayouNpc * npc = new BrayouNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			case GameObjectType::AmandineNpc:
				{
					AmandineNpc * npc = new AmandineNpc();
					npc->onTheFloor();
					m_npcsOnFloor.emplace_back(gameObject.first, npc->getBox()->getSize().x / Tile::TileSize, npc);
				}
				break;
			default:
				break;
		}
	}

	// Register position for gameobjects on the ground
	setupGameObjectPosition(m_elevators);
	setupGameObjectPosition(m_portals);
	setupGameObjectPosition(m_npcsOnFloor);
	setupGameObjectPosition(m_nanoRobots);
	setupGameObjectPosition(m_otherObjectsHigh);
	setupGameObjectPosition(m_otherObjectsLow);
	
	// If water level < 0.f, there is no water
	if (biome.getWaterLevel() > 0.f)
		m_water.reset(new Water(biome));
}

template<class T>
void GroundManager::setupGameObjectPosition(std::vector<GameObjectPosition<T>> const & gameObjectPosition)
{
	for (auto & gameObject : gameObjectPosition)
	{
		for (std::size_t i = 0u; i < gameObject.m_width; i++)
		{
			m_tiles->registerWideDecor(gameObject.m_position + i);
			m_tilesPrev->registerWideDecor(gameObject.m_position + i);
		}
	}
}

void GroundManager::setupDecors(ABiome & biome, SkyCycle & cycle)
{
	m_decorManagerBack.setup(&biome);
	m_decorManagerFront.setup(&biome);
	m_decorManagerGround.setup(&biome);
	m_decorManagerInstanceBack.setup(&biome);
	m_decorManagerInstanceFront.setup(&biome);
	m_decorManagerInstanceGround.setup(&biome);
	std::size_t mapSizeX = biome.getMapSize().x;

	std::size_t treeCount = biome.getTreeCount();
	std::size_t rainbowCount = biome.getRainbowCount();
	std::size_t rockCount = static_cast<std::size_t>(biome.getRockCount() / 2.f);
	std::size_t mushroomCount = static_cast<std::size_t>(biome.getMushroomCount() / 2.f);
	std::size_t crystalCount = static_cast<std::size_t>(biome.getCrystalCount() / 2);
	std::size_t groundRockCount = biome.getGroundRockCount();
	std::size_t grassCount = biome.getGrassCount();
	std::size_t totalCount = 0u;

	if (biome.canCreateRainbow())
	{
		for (std::size_t i = 0; i < rainbowCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(new Rainbow(&cycle));
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += rainbowCount;
	}

	if (biome.canCreateRock())
	{
		for (std::size_t i = 0; i < rockCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Rock);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += rockCount;
	}

	if (biome.canCreateTree())
	{
		for (std::size_t i = 0; i < treeCount; i++)
		{
			int x = biome.getTreePositionX();
			m_decorManagerBack.add(DecorManager::DecorTypes::Tree);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += treeCount;
	}

	if (biome.canCreateRock())
	{
		for (std::size_t i = 0; i < rockCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Rock);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += rockCount;
	}

	if (biome.canCreateMushroom())
	{
		for (std::size_t i = 0; i < mushroomCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Mushroom);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += mushroomCount;
		for (std::size_t i = 0; i < mushroomCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerFront.add(DecorManager::DecorTypes::Mushroom);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += mushroomCount;
	}

	if (biome.canCreateGrass())
	{
		for (std::size_t i = 0; i < grassCount; i++)
		{
			int x = biome.getGrassPosX();
			m_decorManagerBack.add(DecorManager::DecorTypes::Grass);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += grassCount;
		for (std::size_t i = 0; i < grassCount; i++)
		{
			int x = biome.getGrassPosX();
			m_decorManagerFront.add(DecorManager::DecorTypes::Grass);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += grassCount;
	}

	if (biome.canCreateCrystal())
	{
		for (std::size_t i = 0; i < crystalCount; i++)
		{
			int x = biome.getCrystalPosX();
			m_decorManagerBack.add(DecorManager::DecorTypes::Crystal);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += crystalCount;
		for (std::size_t i = 0; i < crystalCount; i++)
		{
			int x = biome.getCrystalPosX();
			m_decorManagerFront.add(DecorManager::DecorTypes::Crystal);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += crystalCount;
	}

	for (std::size_t i = 0; i < groundRockCount; i++)
	{
		int x = biome.randomInt(1u, mapSizeX);
		m_decorManagerGround.add(DecorManager::DecorTypes::GroundRock);
		m_tiles->registerDecor(x);
		m_tilesPrev->registerDecor(x);
	}
	totalCount += groundRockCount;

	m_decorPositions.resize(totalCount);
}

NanoRobot * GroundManager::getNanoRobot(NanoRobot * robot)
{
	for (auto it = m_nanoRobots.begin(); it != m_nanoRobots.end(); it++)
	{
		if (it->m_gameObject.get() == robot)
		{
			it->m_gameObject.release();
			m_nanoRobots.erase(it);
			break;
		}
	}
	for (auto it = m_nanoRobotOnInstance.begin(); it != m_nanoRobotOnInstance.end(); it++)
	{
		if ((*it).get() == robot)
		{
			(*it).release();
			m_nanoRobotOnInstance.erase(it);
			break;
		}
	}
	return robot;
}

void	GroundManager::setNextGenerationState(GenerationState state, sf::Vector2f const & octoPos)
{
	Progress::getInstance().setMapMoving(true);
	m_octoPosState = octoPos;
	if (m_transitionTimer >= m_transitionTimerMax)
		m_nextState = state;
}

void GroundManager::setTransitionAppear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).isTransitionType(ETransitionType::e_transition_appear))
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tilesPrev->get(x, y).setStartTransitionY(j, m_tilesPrev->get(x, y + i).getStartTransition(j).y);
	m_tilesPrev->get(x, y).setStartColor(m_tiles->get(x, y).getStartColor());
	setTransitionModify(x, y);
}

void GroundManager::setTransitionDisappear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).isTransitionType(ETransitionType::e_transition_disappear))
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tiles->get(x, y).setStartTransitionY(j, m_tiles->get(x, y + i).getStartTransition(j).y);
	m_tiles->get(x, y).setStartColor(m_tilesPrev->get(x, y).getStartColor());
}

void GroundManager::setTransitionModify(int x, int y)
{
	// Define if it's a quad or a triangle
	if (y - 1 >= 0 && m_tiles->get(x, y - 1).isEmpty() && y + 1 < static_cast<int>(m_tiles->getRows()))
	{
		if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
			m_tiles->get(x, y).setStartTransitionY(0u, m_tiles->get(x, y + 1).getStartTransition(0u).y);
		if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
			m_tiles->get(x, y).setStartTransitionY(1u, m_tiles->get(x, y + 1).getStartTransition(1u).y);
	}
}

void GroundManager::defineTransition(void)
{
	defineTransitionRange(0, m_tiles->getColumns(), 0, m_tiles->getRows());
}

void GroundManager::defineTransition(int x, int y)
{
	int prev = m_tilesPrev->get(x, y).isEmpty();
	int current = m_tiles->get(x, y).isEmpty();

	if (prev && !current) // appear
		m_tiles->get(x, y).setTransitionType(ETransitionType::e_transition_appear);
	else if (!prev && current) // disappear
		m_tiles->get(x, y).setTransitionType(ETransitionType::e_transition_disappear);
	else if (!current && !prev) // already a tile
		m_tiles->get(x, y).setTransitionType(ETransitionType::e_transition_already);
	else // no tile
		m_tiles->get(x, y).setTransitionType(ETransitionType::e_transition_none);
}

void GroundManager::defineTransitionRange(int startX, int endX, int startY, int endY)
{
	// For each tile, define the type and transition type
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
			defineTransition(x, y);
	}
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (m_tiles->get(x, y).isTransitionType(ETransitionType::e_transition_appear))
				setTransitionAppear(x, y);
		}
	}
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (m_tiles->get(x, y).isTransitionType(ETransitionType::e_transition_already))
				setTransitionModify(x, y);
		}
	}
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (m_tiles->get(x, y).isTransitionType(ETransitionType::e_transition_disappear))
				setTransitionDisappear(x, y);
		}
	}
}

void GroundManager::swapMap(void)
{
	m_tiles.swap(m_tilesPrev);
	m_tiles->computeMap();
	computeDecor();
	defineTransition();
}

void GroundManager::computeDecor(void)
{
	m_tiles->computeDecor();
	m_tiles->computeWideDecor();
}

void GroundManager::updateTransition(sf::FloatRect const & cameraRect)
{
	if (m_transitionTimer > m_transitionTimerMax)
	{
		Progress::getInstance().setMapMoving(false);
		m_transitionTimer = m_transitionTimerMax;
	}
	float transition = m_transitionTimer / m_transitionTimerMax;
	float bottomBorder = cameraRect.top + cameraRect.height + Map::OffsetY + Tile::TileSize;
	float rightBorder = cameraRect.left + cameraRect.width + Map::OffsetX + Tile::TileSize;
	Tile * tile;
	Tile * tilePrev;
	TileShape * first;
	sf::Vertex * last;
	bool updateLast;
	std::size_t physicsLineCount = 0u;
	std::size_t columns = m_tiles->getColumns();
	std::size_t rows = m_tiles->getRows();

	// Update tiles
	m_verticesCount = 0u;
	for (std::size_t x = 0u; x < columns; x++)
	{
		first = nullptr;
		last = nullptr;
		physicsLineCount = 0u;
		updateLast = true;
		for (std::size_t y = 0u; y < rows; y++)
		{
			tile = &m_tiles->get(x, y);
			if (tile->isTransitionType(ETransitionType::e_transition_none))
			{
				if (first && tile->isEmpty())
				{
					updateLast = false;
					if (physicsLineCount == 0u)
					{
						first->setEndVertex(last);
						first = nullptr;
						last = nullptr;
						updateLast = true;
						physicsLineCount++;
					}
				}
				continue;
			}
			if (first)
			{
				// Avoid to compute A LOT of transition under the screen
				if ((tile->getStartTransition(0u).y > bottomBorder && tilePrev->getStartTransition(0u).y > bottomBorder)
						|| (tilePrev->getStartTransition(1u).x < cameraRect.left && tile->getStartTransition(1u).x < cameraRect.left)
						|| (tilePrev->getStartTransition(0u).x > rightBorder && tile->getStartTransition(0u).x > rightBorder))
					break;
			}
			tilePrev = &m_tilesPrev->get(x, y);

			// Update tile transition
			// TODO : Uncomment if we when colors changes in ground
			m_vertices[m_verticesCount].color = tile->getStartColor();//(octo::linearInterpolation(tilePrev->getStartColor(), tile->getStartColor(), transition);
			for (std::size_t i = 0u; i < 3u; i++)
			{
				m_vertices[m_verticesCount + i].position.y = octo::linearInterpolation(tilePrev->getStartTransition(i).y, tile->getStartTransition(i).y, transition) - Map::OffsetY;
				m_vertices[m_verticesCount + i + 1u].color = m_vertices[m_verticesCount].color;
			}

			// This case is here to manage bottom transition from instance
			if (tile->getTileType() == octo::LevelMap::TileType::Square)
				m_vertices[m_verticesCount + 3u].position.y = m_vertices[m_verticesCount + 2u].position.y;
			else
				m_vertices[m_verticesCount + 3u].position.y = octo::linearInterpolation(tilePrev->getStartTransition(3u).y, tile->getStartTransition(3u).y, transition) - Map::OffsetY;

			m_vertices[m_verticesCount + 0u].position.x = tilePrev->getStartTransition(0u).x - Map::OffsetX;
			m_vertices[m_verticesCount + 1u].position.x = tilePrev->getStartTransition(1u).x - Map::OffsetX;
			m_vertices[m_verticesCount + 2u].position.x = m_vertices[m_verticesCount + 1u].position.x;
			m_vertices[m_verticesCount + 3u].position.x = m_vertices[m_verticesCount + 0u].position.x;

			// Update physics information
			if (!first)
			{
				first = m_tileShapes.get(x, physicsLineCount);
				first->setSleep(false);
				m_tileShapes.get(x, physicsLineCount)->setVertex(&m_vertices[m_verticesCount]);
				m_tileShapes.get(x, physicsLineCount)->setGameObject(tilePrev);
			}
			if (updateLast)
				last = &m_vertices[m_verticesCount];
			m_verticesCount += 4u;
		}
		if (first)
			first->setEndVertex(last);
		else
			m_tileShapes.get(x, physicsLineCount)->setSleep(true);
		if (physicsLineCount == 0u)
			m_tileShapes.get(x, 1u)->setSleep(true);
	}

	// Update decors
	Map::Decors const & current = m_tiles->getDecorsPosition();
	Map::Decors const & prev = m_tilesPrev->getDecorsPosition();
	for (std::size_t i = 0u; i < m_tiles->getDecorsPosition().size(); i++)
	{
		m_decorPositions[i].y = octo::linearInterpolation(prev[i].second.y, current[i].second.y, transition) - Map::OffsetY + Tile::TileSize;
		m_decorPositions[i].x = current[i].second.x - Map::OffsetX;
	}

	// Update wide decors
	Map::WideDecors const & currentWide = m_tiles->getWideDecorsPosition();
	Map::WideDecors const & prevWide = m_tilesPrev->getWideDecorsPosition();

	placeMax(m_elevators, currentWide, prevWide, transition);
	placeMax(m_portals, currentWide, prevWide, transition);
	placeMax(m_nanoRobots, currentWide, prevWide, transition);
	placeMin(m_npcsOnFloor, currentWide, prevWide, transition);
	placeMax(m_otherObjectsHigh, currentWide, prevWide, transition);
	placeMin(m_otherObjectsLow, currentWide, prevWide, transition);

	// Replace npc around the map
	float mapSizeX = m_mapSize.x * Tile::TileSize;
	for (auto const & npc : m_npcsOnFloor)
	{
		if (npc.m_gameObject->getPosition().x < m_offset.x - mapSizeX / 2.f)
			npc.m_gameObject->addMapOffset(mapSizeX, 0.f);
		else if (npc.m_gameObject->getPosition().x > m_offset.x + mapSizeX / 2.f)
			npc.m_gameObject->addMapOffset(-mapSizeX, 0.f);
	}

	for (auto const & otherOnInstance : m_otherOnInstance)
	{
		if (otherOnInstance->getPosition().x < m_offset.x - mapSizeX / 2.f)
			otherOnInstance->addMapOffset(mapSizeX, 0.f);
		else if (otherOnInstance->getPosition().x > m_offset.x + mapSizeX / 2.f)
			otherOnInstance->addMapOffset(-mapSizeX, 0.f);
	}

	for (auto const & npc : m_npcs)
	{
		if (npc->getPosition().x < m_offset.x - mapSizeX / 2.f)
			npc->addMapOffset(mapSizeX, 0.f);
		else if (npc->getPosition().x > m_offset.x + mapSizeX / 2.f)
			npc->addMapOffset(-mapSizeX, 0.f);
	}

	if (m_water)
	{
		if (m_water->getPosition().x < m_offset.x - mapSizeX / 2.f)
			m_water->addMapOffset(mapSizeX);
		else if (m_water->getPosition().x > m_offset.x + mapSizeX / 2.f)
			m_water->addMapOffset(-mapSizeX);
	}

	for (auto const & decor : m_instanceDecors)
	{
		if (decor->getPosition().x < m_offset.x - mapSizeX / 2.f)
			decor->addMapOffset(mapSizeX, 0.f);
		else if (decor->getPosition().x > m_offset.x + mapSizeX / 2.f)
			decor->addMapOffset(-mapSizeX, 0.f);
	}

	for (auto const & decor : m_instanceDecorsFront)
	{
		if (decor->getPosition().x < m_offset.x - mapSizeX / 2.f)
			decor->addMapOffset(mapSizeX, 0.f);
		else if (decor->getPosition().x > m_offset.x + mapSizeX / 2.f)
			decor->addMapOffset(-mapSizeX, 0.f);
	}

	for (auto it = m_decorManagerInstanceBack.begin(); it != m_decorManagerInstanceBack.end(); it++)
	{
		if ((*it)->getPosition().x < m_offset.x - mapSizeX / 2.f)
			(*it)->setPosition((*it)->getPosition() + sf::Vector2f(mapSizeX, 0.f));
		else if ((*it)->getPosition().x > m_offset.x + mapSizeX / 2.f)
			(*it)->setPosition((*it)->getPosition() - sf::Vector2f(mapSizeX, 0.f));
	}

	for (auto it = m_decorManagerInstanceFront.begin(); it != m_decorManagerInstanceFront.end(); it++)
	{
		if ((*it)->getPosition().x < m_offset.x - mapSizeX / 2.f)
			(*it)->setPosition((*it)->getPosition() + sf::Vector2f(mapSizeX, 0.f));
		else if ((*it)->getPosition().x > m_offset.x + mapSizeX / 2.f)
			(*it)->setPosition((*it)->getPosition() - sf::Vector2f(mapSizeX, 0.f));
	}

	for (auto it = m_decorManagerInstanceGround.begin(); it != m_decorManagerInstanceGround.end(); it++)
	{
		if ((*it)->getPosition().x < m_offset.x - mapSizeX / 2.f)
			(*it)->setPosition((*it)->getPosition() + sf::Vector2f(mapSizeX, 0.f));
		else if ((*it)->getPosition().x > m_offset.x + mapSizeX / 2.f)
			(*it)->setPosition((*it)->getPosition() - sf::Vector2f(mapSizeX, 0.f));
	}

	for (auto const & robot : m_nanoRobotOnInstance)
	{
		if (robot->getTargetPosition().x < m_offset.x - mapSizeX / 2.f)
			robot->addMapOffset(mapSizeX, 0.f);
		else if (robot->getTargetPosition().x > m_offset.x + mapSizeX / 2.f)
			robot->addMapOffset(-mapSizeX, 0.f);
	}
}

template<class T>
void GroundManager::placeMin(std::vector<GameObjectPosition<T>> & objects, Map::WideDecors const & currentDecors, Map::WideDecors const & prevDecors, float transition)
{
	float min;
	float tmp = 0.f;
	for (auto const & object : objects)
	{
		min = std::numeric_limits<float>::lowest();
		for (std::size_t i = object.m_position; i < object.m_position + object.m_width; i++)
		{
			tmp = octo::linearInterpolation(prevDecors[i].second.y, currentDecors[i].second.y, transition);
			if (tmp > min)
				min = tmp;
		}
		object.m_gameObject->setPosition(sf::Vector2f(currentDecors[object.m_position].second.x - Map::OffsetX, min - Map::OffsetY));
	}
}

template<class T>
void GroundManager::placeMax(std::vector<GameObjectPosition<T>> & objects, Map::WideDecors const & currentDecors, Map::WideDecors const & prevDecors, float transition)
{
	float max;
	float tmp = 0.f;
	for (auto const & object : objects)
	{
		max = std::numeric_limits<float>::max();
		for (std::size_t i = object.m_position; i < object.m_position + object.m_width; i++)
		{
			tmp = octo::linearInterpolation(prevDecors[i].second.y, currentDecors[i].second.y, transition);
			if (tmp < max)
				max = tmp;
		}
		object.m_gameObject->setPosition(sf::Vector2f(currentDecors[object.m_position].second.x - Map::OffsetX, max - Map::OffsetY));
	}
}

void GroundManager::defineTransitionBorderTileRange(int startX, int endX, int startY, int endY)
{
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (y - 1 >= 0 && y + 1 < static_cast<int>(m_tiles->getRows()))
			{
				// Define triangle/quad
				if (m_tiles->get(x, y - 1).isEmpty())
				{
					if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
						m_tiles->get(x, y).setStartTransitionY(0u, m_tiles->get(x, y + 1).getStartTransition(0u).y);
					if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
						m_tiles->get(x, y).setStartTransitionY(1u, m_tiles->get(x, y + 1).getStartTransition(1u).y);
				}
				if (m_tilesPrev->get(x, y - 1).isEmpty())
				{
					if (x - 1 >= 0 && m_tilesPrev->get(x - 1, y).isEmpty())
						m_tilesPrev->get(x, y).setStartTransitionY(0u, m_tilesPrev->get(x, y + 1).getStartTransition(0u).y);
					if (x + 1 < static_cast<int>(m_tilesPrev->getColumns()) && m_tilesPrev->get(x + 1, y).isEmpty())
						m_tilesPrev->get(x, y).setStartTransitionY(1u, m_tilesPrev->get(x, y + 1).getStartTransition(1u).y);
				}
			}
		}
	}
	defineTransitionRange(startX, endX, startY, endY);
}

void GroundManager::updateOffset(float)
{
	int ofX = 0;
	int ofY = 0;
	int newOfX = static_cast<int>(m_offset.x / Tile::TileSize);
	int newOfY = static_cast<int>(m_offset.y / Tile::TileSize);
	if (m_oldOffset.x != newOfX)
		ofX = newOfX - m_oldOffset.x;
	if (m_oldOffset.y != newOfY)
		ofY = newOfY - m_oldOffset.y;

	if (ofX >= static_cast<int>(m_tiles->getColumns()) - 1)
		ofX = static_cast<int>(m_tiles->getColumns()) - 1;
	if (ofY >= static_cast<int>(m_tiles->getRows()) - 1)
		ofY = static_cast<int>(m_tiles->getRows()) - 1;
	if (ofX <= -static_cast<int>(m_tiles->getColumns()) + 1)
		ofX = -static_cast<int>(m_tiles->getColumns()) + 1;
	if (ofY <= -static_cast<int>(m_tiles->getRows()) + 1)
		ofY = -static_cast<int>(m_tiles->getRows()) + 1;

	if (ofX)
		computeDecor();
	if (ofX && ofY)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(ofX);
		m_tilesPrev->addOffsetX(ofX);
		m_tiles->addOffsetY(ofY);
		m_tilesPrev->addOffsetY(ofY);
		if (ofX < 0)
		{
			m_tiles->computeMapRangeX(0, -ofX);
			m_tilesPrev->computeMapRangeX(0, -ofX);
			if (ofY < 0)
			{
				m_tiles->computeMapRangeY(0, -ofY);
				m_tilesPrev->computeMapRangeY(0, -ofY);
				defineTransitionBorderTileRange(0, -ofX + 1, -ofY + 1, m_tiles->getRows());
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, -ofY + 1);
			}
			else
			{
				m_tiles->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				m_tilesPrev->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				defineTransitionBorderTileRange(0, ofY + 1, 0, m_tiles->getRows() - ofY - 1);
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - ofY - 1, m_tiles->getRows());
			}
		}
		else
		{
			m_tiles->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			m_tilesPrev->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			if (ofY < 0)
			{
				m_tiles->computeMapRangeY(0, -ofY);
				m_tilesPrev->computeMapRangeY(0, -ofY);
				defineTransitionBorderTileRange(m_tiles->getColumns() - ofX - 1, m_tiles->getColumns(), -ofY + 1, m_tiles->getRows());
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, -ofY + 1);
			}
			else
			{
				m_tiles->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				m_tilesPrev->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				defineTransitionBorderTileRange(m_tiles->getColumns() - ofX - 1, m_tiles->getColumns(), 0, m_tiles->getRows() - ofY - 1);
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - ofY - 1, m_tiles->getRows());
			}
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.x = newOfX;
		m_oldOffset.y = newOfY;
	}
	else if (ofX)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(ofX);
		m_tilesPrev->addOffsetX(ofX);
		if (ofX < 0)
		{
			m_tiles->computeMapRangeX(0, -ofX);
			m_tilesPrev->computeMapRangeX(0, -ofX);
			defineTransitionBorderTileRange(0, -ofX + 1, 0, m_tiles->getRows());
		}
		else
		{
			m_tiles->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			m_tilesPrev->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			defineTransitionBorderTileRange(m_tiles->getColumns() - ofX - 1, m_tiles->getColumns(), 0, m_tiles->getRows());
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.x = newOfX;
	}
	else if (ofY)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(ofY);
		m_tilesPrev->addOffsetY(ofY);
		if (ofY < 0)
		{
			m_tiles->computeMapRangeY(0, -ofY);
			m_tilesPrev->computeMapRangeY(0, -ofY);
			defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, -ofY + 1);
		}
		else
		{
			m_tiles->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
			m_tilesPrev->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
			defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - 20, m_tiles->getRows());
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.y = newOfY;
	}
}

void GroundManager::updateDecors(sf::Time deltatime)
{
	std::size_t i = 0;
	for (auto it = m_decorManagerBack.begin(); it != m_decorManagerBack.end(); it++, i++)
		(*it)->setPosition(m_decorPositions[i]);
	for (auto it = m_decorManagerFront.begin(); it != m_decorManagerFront.end(); it++, i++)
		(*it)->setPosition(m_decorPositions[i]);
	for (auto it = m_decorManagerGround.begin(); it != m_decorManagerGround.end(); it++, i++)
		(*it)->setPosition(m_decorPositions[i]);
	//TODO update decorposition

	octo::Camera& camera = octo::Application::getCamera();
	m_decorManagerBack.update(deltatime, camera);
	m_decorManagerFront.update(deltatime, camera);
	m_decorManagerGround.update(deltatime, camera);
	m_decorManagerInstanceBack.update(deltatime, camera);
	m_decorManagerInstanceFront.update(deltatime, camera);
	m_decorManagerInstanceGround.update(deltatime, camera);
}

void GroundManager::updateGameObjects(sf::Time frametime)
{
	for (auto & decor : m_instanceDecors)
		decor->update(frametime);
	for (auto & decor : m_instanceDecorsFront)
		decor->update(frametime);
	for (auto & object : m_otherObjectsHigh)
		object.m_gameObject->update(frametime);
	for (auto & object : m_otherObjectsLow)
		object.m_gameObject->update(frametime);
	for (auto & elevator : m_elevators)
		elevator.m_gameObject->update(frametime);
	for (auto & portal : m_portals)
		portal.m_gameObject->update(frametime);
	for (auto & nano : m_nanoRobots)
		nano.m_gameObject->update(frametime);
	for (auto & object : m_otherOnInstance)
		object->update(frametime);
	for (auto & npc : m_npcsOnFloor)
		npc.m_gameObject->update(frametime);
	for (auto & npc : m_npcs)
		npc->update(frametime);
	for (auto & nano : m_nanoRobotOnInstance)
		nano->update(frametime);
	if (m_water)
		m_water->update(frametime);
}

void GroundManager::update(float deltatime)
{
	static float accumulator = 0.f;
	float dt = 1.f / 59.f;

	if (deltatime > 0.2f)
		deltatime = 0.2f;
	accumulator += deltatime;

	while (accumulator > dt)
	{
		accumulator -= dt;
		m_transitionTimer += dt;
		// Get the top left of the camera view
		sf::Rect<float> const & rect = octo::Application::getCamera().getRectangle();
		m_offset.x = rect.left;
		m_offset.y = rect.top;

		if (m_transitionTimer >= m_transitionTimerMax)
		{
			bool compute = false;
			if (m_nextState == GenerationState::Next)
			{
				compute = true;
				m_tilesPrev->registerOctoPos(m_octoPosState);
				m_tilesPrev->nextStep();
				m_tiles->registerOctoPos(m_octoPosState);
				m_tiles->registerDepth();
				m_tiles->nextStep();
				m_nextState = GenerationState::None;
			}
			else if (m_nextState == GenerationState::Previous)
			{
				compute = true;
				m_tilesPrev->registerOctoPos(m_octoPosState);
				m_tilesPrev->previousStep();
				m_tiles->registerOctoPos(m_octoPosState);
				m_tiles->registerDepth();
				m_tiles->previousStep();
				m_nextState = GenerationState::None;
			}
			if (compute)
			{
				m_transitionTimer = 0.f;
				swapMap();
			}
		}
		updateOffset(deltatime);
		updateTransition(rect);
	}
	updateDecors(sf::seconds(deltatime));
	updateGameObjects(sf::seconds(deltatime));
}

void GroundManager::drawBack(sf::RenderTarget& render, sf::RenderStates states) const
{
	render.draw(m_decorManagerBack, states);
	render.draw(m_decorManagerInstanceBack, states);
	for (auto & decor : m_instanceDecors)
	{
		if (decor->isInScreen())
			decor->draw(render, states);
	}
	for (auto & decor : m_instanceDecorsFront)
	{
		if (decor->isInScreen())
			decor->draw(render, states);
	}
	for (auto & objectHigh : m_otherObjectsHigh)
	{
		if (objectHigh.m_gameObject->isInScreen())
			objectHigh.m_gameObject->draw(render, states);
	}
	for (auto & objectLow : m_otherObjectsLow)
	{
		if (objectLow.m_gameObject->isInScreen())
			objectLow.m_gameObject->draw(render, states);
	}
	for (auto & elevator : m_elevators)
	{
		elevator.m_gameObject->draw(render, states);
	}
	for (auto & portal : m_portals)
	{
		if (portal.m_gameObject->isInScreen())
			portal.m_gameObject->draw(render, states);
	}
	for (auto & object : m_otherOnInstance)
	{
		if (object->isInScreen())
			object->draw(render, states);
	}
	for (auto & npc : m_npcsOnFloor)
	{
		if (npc.m_gameObject->isInScreen())
			npc.m_gameObject->draw(render, states);
	}
	for (auto & npc : m_npcs)
	{
		if (npc->isInScreen())
			npc->draw(render, states);
	}
}

void GroundManager::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	for (auto & npc : m_npcsOnFloor)
	{
		if (npc.m_gameObject->isInScreen())
			npc.m_gameObject->drawFront(render, states);
	}
	for (auto & elevator : m_elevators)
	{
		elevator.m_gameObject->drawFront(render, states);
	}
	for (auto & objectHigh : m_otherObjectsHigh)
	{
		if (objectHigh.m_gameObject->isInScreen())
			objectHigh.m_gameObject->drawFront(render, states);
	}
	for (auto & objectLow : m_otherObjectsLow)
	{
		if (objectLow.m_gameObject->isInScreen())
			objectLow.m_gameObject->drawFront(render, states);
	}
	for (auto & npc : m_npcs)
	{
		if (npc->isInScreen())
			npc->drawFront(render, states);
	}
	for (auto & decor : m_instanceDecors)
		decor->drawFront(render, states);
	render.draw(m_decorManagerFront, states);
	render.draw(m_decorManagerInstanceFront, states);
	render.draw(m_vertices.get(), m_verticesCount, sf::Quads, states);
	render.draw(m_decorManagerGround, states);
	render.draw(m_decorManagerInstanceGround, states);
	for (auto & nano : m_nanoRobotOnInstance)
	{
		if (nano->isInScreen())
			nano->draw(render, states);
	}
	for (auto & decor : m_instanceDecorsFront)
	{
		if (decor->isInScreen())
			decor->drawFront(render, states);
	}
	for (auto & nano : m_nanoRobots)
	{
		if (nano.m_gameObject->isInScreen())
			nano.m_gameObject->draw(render, states);
	}
}

void GroundManager::drawWater(sf::RenderTarget& render, sf::RenderStates states) const
{
	if (m_water)
		render.draw(*m_water, states);
}

void GroundManager::drawText(sf::RenderTarget& render, sf::RenderStates states) const
{
	for (auto & npc : m_npcsOnFloor)
		npc.m_gameObject->drawText(render, states);
	for (auto & npc : m_npcs)
		npc->drawText(render, states);
	for (auto & nano : m_nanoRobots)
		nano.m_gameObject->drawText(render, states);
	for (auto & nano : m_nanoRobotOnInstance)
		nano->drawText(render, states);
}
