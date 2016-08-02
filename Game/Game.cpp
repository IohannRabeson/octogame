#include "Game.hpp"
#include "AGameObject.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "ChallengeManager.hpp"
#include "Challenges.hpp"
#include "CameraMovement.hpp"
#include "PostEffectLayer.hpp"

// Biomes
#include "IceABiome.hpp"
#include "IceBBiome.hpp"
#include "IceCBiome.hpp"
#include "IceDBiome.hpp"
#include "DesertABiome.hpp"
#include "DesertBBiome.hpp"
#include "DesertCBiome.hpp"
#include "DesertDBiome.hpp"
#include "JungleABiome.hpp"
#include "JungleBBiome.hpp"
#include "JungleCBiome.hpp"
#include "JungleDBiome.hpp"
#include "WaterABiome.hpp"
#include "WaterBBiome.hpp"
#include "RandomBiome.hpp"
#include "RewardsBiome.hpp"

//Objects
#include "ElevatorStream.hpp"
#include "Bouibouik.hpp"
#include "Tent.hpp"
#include "SpaceShip.hpp"
#include "Concert.hpp"

//Nano
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
#include "RepairShipNanoRobot.hpp"
#include "JumpNanoRobot.hpp"
#include "SlowFallNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "WaterNanoRobot.hpp"

//Npc
//Script AddNpc Include
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
#include "Snowman1Npc.hpp"
#include "FranfranNpc.hpp"
#include "CanouilleNpc.hpp"
#include "JuNpc.hpp"
#include "FannyNpc.hpp"
#include "CedricStartNpc.hpp"
#include "GuiNpc.hpp"
#include "PunkNpc.hpp"
#include "ClementineNpc.hpp"
#include "FatNpc.hpp"
#include "PeaNpc.hpp"
#include "PierreNpc.hpp"
#include "JeffMouffyNpc.hpp"
#include "TurbanNpc.hpp"
#include "VinceNpc.hpp"
#include "AmandineNpc.hpp"
#include "FaustNpc.hpp"
#include "WolfNpc.hpp"
#include "ConstanceNpc.hpp"
#include "BrayouNpc.hpp"
#include "Snowman2Npc.hpp"
#include "EvaNpc.hpp"
#include "OldDesertStaticNpc.hpp"
#include "JellyfishNpc.hpp"
#include "BirdRedNpc.hpp"
#include "WellKeeperNpc.hpp"
#include "LucienNpc.hpp"
#include "IohannNpc.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <SFML/Audio/Listener.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

Game::Game(void) :
	m_physicsEngine(PhysicsEngine::getInstance()),
	m_musicPlayer(MusicManager::getInstance()),
	m_skyCycle(nullptr),
	m_skyManager(nullptr),
	m_groundManager(nullptr),
	m_parallaxScrolling(nullptr),
	m_octo(nullptr),
	m_konami(nullptr),
	m_keyGroundRight(false),
	m_keyGroundLeft(false),
	m_soundGeneration(nullptr),
	m_groundVolume(0.7f),
	m_groundSoundTime(sf::Time::Zero),
	m_groundSoundTimeMax(sf::seconds(0.6f)),
	m_slowTimeInfosCoef(1.f),
	m_skipFrames(0u),
	m_skipFramesMax(3u)
{
	InputListener::addInputListener();

	m_biomeManager.registerBiome<IceABiome>(Level::IceA);
	m_biomeManager.registerBiome<IceBBiome>(Level::IceB);
	m_biomeManager.registerBiome<IceCBiome>(Level::IceC);
	m_biomeManager.registerBiome<IceDBiome>(Level::IceD);
	m_biomeManager.registerBiome<DesertABiome>(Level::DesertA);
	m_biomeManager.registerBiome<DesertBBiome>(Level::DesertB);
	m_biomeManager.registerBiome<DesertCBiome>(Level::DesertC);
	m_biomeManager.registerBiome<DesertDBiome>(Level::DesertD);
	m_biomeManager.registerBiome<JungleABiome>(Level::JungleA);
	m_biomeManager.registerBiome<JungleBBiome>(Level::JungleB);
	m_biomeManager.registerBiome<JungleCBiome>(Level::JungleC);
	m_biomeManager.registerBiome<JungleDBiome>(Level::JungleD);
	m_biomeManager.registerBiome<WaterABiome>(Level::WaterA);
	m_biomeManager.registerBiome<WaterBBiome>(Level::WaterB);

	m_biomeManager.registerBiome<RandomBiome>(Level::Random);
	m_biomeManager.registerBiome<RewardsBiome>(Level::Rewards);
}

Game::~Game(void)
{
	if (m_soundGeneration != nullptr)
		m_soundGeneration->stop();
	InputListener::removeInputListener();
}

void	Game::loadLevel(void)
{
	Progress &					progress = Progress::getInstance();
	octo::AudioManager&			audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	sf::Vector2f				startPosition;

	if (progress.isMenu())
	{
		m_biomeManager.changeBiome(Level::Rewards, 0x12345);
		startPosition = m_biomeManager.getCurrentBiome().getOctoStartPosition();
	}
	else
	{
		m_biomeManager.changeBiome(progress.getNextDestination(), 0x12345);
		progress.setCurrentDestination(m_biomeManager.getCurrentBiome().getId());
		if (progress.getRespawnType() == Progress::RespawnType::Portal)
		{
			startPosition = m_biomeManager.getCurrentBiome().getOctoStartPosition();
			progress.setCheckPointPosition(startPosition);
		}
		else // if octo died
			startPosition = progress.getCheckPointPosition();
	}

	auto & gameObjects = m_biomeManager.getCurrentBiome().getGameObjects();
	int portalCount = 0u;
	for (auto & gameObject : gameObjects)
	{
		switch (gameObject.second)
		{
			case GameObjectType::PortalRandom:
			case GameObjectType::PortalJungle:
			case GameObjectType::PortalSnow:
			case GameObjectType::PortalDesert:
			case GameObjectType::PortalWater:
			case GameObjectType::Portal:
				portalCount++;
				break;
			default:
				break;
		}
	}

	// Reset last values
	postEffect.removeEffects();
	PostEffectLayer::getInstance().clear();
	PostEffectLayer::getInstance().registerShader(CIRCLE_RAINBOW_FRAG, CIRCLE_RAINBOW_FRAG);
	PostEffectLayer::getInstance().registerShader(VISION_TROUBLE_FRAG, VISION_TROUBLE_FRAG);
	PostEffectLayer::getInstance().registerShader(PERSISTENCE_FRAG, PERSISTENCE_FRAG);
	PostEffectLayer::getInstance().registerShader(PIXELATE_FRAG, PIXELATE_FRAG);
	PostEffectLayer::getInstance().registerShader(DISPLACEMENT_FRAG, DISPLACEMENT_FRAG);
	PostEffectLayer::getInstance().registerShader(KERNEL_POST_EFFECT_FRAG, KERNEL_POST_EFFECT_FRAG);
	PostEffectLayer::getInstance().registerShader("render_black_kernel", KERNEL_POST_EFFECT_FRAG);
	PostEffectLayer::getInstance().registerShader("render_white_kernel", KERNEL_POST_EFFECT_FRAG);
	PostEffectLayer::getInstance().registerShader(WATER_FRAG, WATER_FRAG);
	PostEffectLayer::getInstance().registerShader(VORTEX_FRAG, VORTEX_FRAG);
	PostEffectLayer::getInstance().registerShader("vortex_red", VORTEX_FRAG);
	PostEffectLayer::getInstance().registerShader("vortex_blue", VORTEX_FRAG);
	for (int i = 0u; i < portalCount; i++)
	{
		std::string name = "vortex_" + std::to_string(i);
		PostEffectLayer::getInstance().registerShader(name.c_str(), VORTEX_FRAG);
	}
	PostEffectLayer::getInstance().registerShader(DUPLICATE_SCREEN_FRAG, DUPLICATE_SCREEN_FRAG);
	PostEffectLayer::getInstance().registerShader(CUTSCENE_FRAG, CUTSCENE_FRAG);

	ChallengeManager::getInstance().reset();
	audio.reset();
	// Reset PhysycsEngine
	octo::Application::getCamera().setCenter(startPosition - sf::Vector2f(0.f, 200.f));
	m_physicsEngine.unregisterAllShapes();
	m_physicsEngine.unregisterAllTiles();
	m_physicsEngine.setIterationCount(8u);
	m_physicsEngine.setGravity(sf::Vector2f(0.f, 600.f));
	m_physicsEngine.setTileCollision(true);
	m_physicsEngine.setContactListener(this);
	m_musicPlayer.setup(m_biomeManager.getCurrentBiome());

	m_skyCycle.reset(new SkyCycle());
	m_skyManager.reset(new SkyManager());
	m_groundManager.reset(new GroundManager());
	m_parallaxScrolling.reset(new ParallaxScrolling());
	m_octo.reset(new CharacterOcto());
	m_konami.reset(new KonamiCode());
	m_cameraMovement.reset(new CameraMovement());

	m_skyCycle->setup(m_biomeManager.getCurrentBiome());
	m_skyManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_groundManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_parallaxScrolling->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_octo->setup(m_biomeManager.getCurrentBiome());
	m_octo->setStartPosition(startPosition);

	audio.playSound(resources.getSound(PORTAL_END_OGG), 1.f);
	m_soundGeneration = audio.playSound(resources.getSound(GROUND_OGG), 0.f);
	m_soundGeneration->setLoop(true);
}

sf::Vector2f	Game::getOctoBubblePosition(void) const
{
	return m_octo->getBubblePosition();
}

void	Game::update(sf::Time frameTime)
{
	m_octo->resetCollidingTileCount();
	//std::cout << "GAME UPDATE" << std::endl;
	PostEffectLayer::getInstance().enableShader(VORTEX_FRAG, false);
	if (m_skipFrames < m_skipFramesMax)
	{
		m_skipFrames++;
		frameTime = sf::seconds(0.016f);
	}
	frameTime = frameTime / m_slowTimeInfosCoef;
	// update the PhysicsEngine as first
	m_physicsEngine.update(frameTime.asSeconds());
	m_cameraMovement->update(frameTime, *m_octo);
	m_musicPlayer.update(frameTime, m_octo->getPosition());
	sf::Vector2f const & octoPos = m_octo->getPosition();
	sf::Listener::setPosition(sf::Vector3f(octoPos.x, octoPos.y, 0.f));
	m_octo->update(frameTime);
	m_skyCycle->update(frameTime, m_biomeManager.getCurrentBiome());
	moveMap(frameTime);
	m_groundManager->update(frameTime.asSeconds());
	m_parallaxScrolling->update(frameTime.asSeconds());
	m_skyManager->update(frameTime);
	m_konami->update(frameTime, m_octo->getPosition());
	m_octo->startKonamiCode(m_konami->canStartEvent());
	ChallengeManager::getInstance().update(m_biomeManager.getCurrentBiome(), m_octo->getPosition(), frameTime);
}

void Game::onShapeCollision(AShape * shapeA, AShape * shapeB, sf::Vector2f const & collisionDirection)
{
	if (shapeA->getGameObject() == nullptr || shapeB->getGameObject() == nullptr)
		return;

	if (gameObjectCast<CharacterOcto>(shapeA->getGameObject()))
		onCollision(gameObjectCast<CharacterOcto>(shapeA->getGameObject()), shapeB->getGameObject(), collisionDirection);
	else if (gameObjectCast<CharacterOcto>(shapeB->getGameObject()))
		onCollision(gameObjectCast<CharacterOcto>(shapeB->getGameObject()), shapeA->getGameObject(), collisionDirection);
	else if (gameObjectCast<CharacterOcto::OctoEvent>(shapeB->getGameObject()))
		onCollisionEvent(gameObjectCast<CharacterOcto::OctoEvent>(shapeB->getGameObject())->m_octo, shapeA->getGameObject(), collisionDirection);
	else if (gameObjectCast<CharacterOcto::OctoEvent>(shapeA->getGameObject()))
		onCollisionEvent(gameObjectCast<CharacterOcto::OctoEvent>(shapeA->getGameObject())->m_octo, shapeB->getGameObject(), collisionDirection);
}

void Game::onCollision(CharacterOcto * octo, AGameObjectBase * gameObject, sf::Vector2f const & collisionDirection)
{
	switch (gameObject->getObjectType())
	{
		case GameObjectType::Elevator:
			if (gameObjectCast<ElevatorStream>(gameObject)->isActivated())
			{
				octo->setTopElevator(gameObjectCast<ElevatorStream>(gameObject)->getTopY());
				octo->onCollision(nullptr, GameObjectType::Elevator, collisionDirection);
			}
			break;
		case GameObjectType::Portal:
			if (gameObjectCast<Portal>(gameObject)->isActivated())
				octo->usePortal(*gameObjectCast<Portal>(gameObject));
			break;
		case GameObjectType::CheckPoint:
			gameObjectCast<CheckPoint>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::JumpNanoRobot:
			if (!gameObjectCast<JumpNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canJump())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<JumpNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr, true);
			}
			break;
		case GameObjectType::DoubleJumpNanoRobot:
			if (!gameObjectCast<DoubleJumpNanoRobot>(gameObject)->isTravelling())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<DoubleJumpNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr, true);
			}
			break;
		case GameObjectType::GroundTransformNanoRobot:
			if (!gameObjectCast<GroundTransformNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canMoveMap())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<GroundTransformNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr, true);
			}
			break;
		case GameObjectType::SlowFallNanoRobot:
			if (!gameObjectCast<SlowFallNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canSlowFall())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<SlowFallNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr, true);
			}
			break;
		case GameObjectType::WaterNanoRobot:
			if (!gameObjectCast<WaterNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canUseWaterJump())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<WaterNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr, true);
			}
			break;
		case GameObjectType::RepairShipNanoRobot:
			if (!gameObjectCast<RepairShipNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canRepairShip())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<RepairShipNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr, true);
			}
			break;
		case GameObjectType::RepairNanoRobot:
			if (!gameObjectCast<RepairNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canRepair())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<RepairNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveRepairNanoRobot(static_cast<RepairNanoRobot *>(ptr), true);
			}
			break;
		case GameObjectType::Rocket:
			gameObjectCast<Rocket>(gameObject)->collideOctoEvent(octo);
			break;
		default:
			break;
	}
}

void Game::onCollisionEvent(CharacterOcto * octo, AGameObjectBase * gameObject, sf::Vector2f const & collisionDirection)
{
	(void)collisionDirection;
	switch (gameObject->getObjectType())
	{
		case GameObjectType::Elevator:
			octo->repairElevator(*gameObjectCast<ElevatorStream>(gameObject));
			break;
		case GameObjectType::Portal:
			gameObjectCast<Portal>(gameObject)->appear();
			break;
//Script AddNpc GameObject
		case GameObjectType::WindowGlitchNpc:
			gameObjectCast<WindowGlitchNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::FranGlitchNpc:
			gameObjectCast<FranGlitchNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::JuGlitchNpc:
			gameObjectCast<JuGlitchNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::LuGlitchNpc:
			gameObjectCast<LuGlitchNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::LongChairNpc:
			gameObjectCast<LongChairNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::OctoDeathNpc:
			gameObjectCast<OctoDeathNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::CedricStartNpc:
			gameObjectCast<CedricStartNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::CedricEndNpc:
			gameObjectCast<CedricEndNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::TVScreen:
			gameObjectCast<TVScreen>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::FabienNpc:
			gameObjectCast<FabienNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::CheckPoint:
			gameObjectCast<CheckPoint>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::OverCoolNpc:
			gameObjectCast<OverCoolNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::Pedestal:
			gameObjectCast<Pedestal>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::ForestSpirit2Npc:
			gameObjectCast<ForestSpirit2Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::ForestSpirit1Npc:
			gameObjectCast<ForestSpirit1Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::BirdBlueNpc:
			gameObjectCast<BirdBlueNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::StrangerSnowNpc:
			gameObjectCast<StrangerSnowNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::StrangerGirlSnowNpc:
			gameObjectCast<StrangerGirlSnowNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::SnowGirl2Npc:
			gameObjectCast<SnowGirl2Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::SnowGirl1Npc:
			gameObjectCast<SnowGirl1Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::Snowman3Npc:
			gameObjectCast<Snowman3Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::Snowman1Npc:
			gameObjectCast<Snowman1Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::FranfranNpc:
			gameObjectCast<FranfranNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::EvaNpc:
			gameObjectCast<EvaNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::PierreNpc:
			gameObjectCast<PierreNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::PeaNpc:
			gameObjectCast<PeaNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::BrayouNpc:
			gameObjectCast<BrayouNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::Snowman2Npc:
			gameObjectCast<Snowman2Npc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::AmandineNpc:
			gameObjectCast<AmandineNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::FaustNpc:
			gameObjectCast<FaustNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::WolfNpc:
			gameObjectCast<WolfNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::ConstanceNpc:
			gameObjectCast<ConstanceNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::JuNpc:
			gameObjectCast<JuNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::FannyNpc:
			gameObjectCast<FannyNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::GuiNpc:
			gameObjectCast<GuiNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::TurbanNpc:
			gameObjectCast<TurbanNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::CanouilleNpc:
			gameObjectCast<CanouilleNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::JeffMouffyNpc:
			gameObjectCast<JeffMouffyNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::JellyfishNpc:
			gameObjectCast<JellyfishNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::BirdRedNpc:
			gameObjectCast<BirdRedNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::WellKeeperNpc:
			gameObjectCast<WellKeeperNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::OldDesertStaticNpc:
			gameObjectCast<OldDesertStaticNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::VinceNpc:
			gameObjectCast<VinceNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::LucienNpc:
			gameObjectCast<LucienNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::PunkNpc:
			gameObjectCast<PunkNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::IohannNpc:
			gameObjectCast<IohannNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::ClementineNpc:
			gameObjectCast<ClementineNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::FatNpc:
			gameObjectCast<FatNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::SpaceShip:
			octo->collideSpaceShip(gameObjectCast<SpaceShip>(gameObject));
			break;
		default:
			break;
	}
}

void Game::onTileShapeCollision(TileShape * tileShape, AShape * shape, sf::Vector2f const & collisionDirection)
{
	if (shape->getGameObject() && gameObjectCast<CharacterOcto>(shape->getGameObject()))
	{
		m_octo->onCollision(tileShape, GameObjectType::Tile, collisionDirection);
	}
}

void Game::moveMap(sf::Time frameTime)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	float						volume = 0.f;

	if (m_soundGeneration != nullptr && !m_keyGroundRight && !m_keyGroundLeft && m_groundSoundTime > sf::Time::Zero)
		m_groundSoundTime -= frameTime;

	if ((m_keyGroundRight || m_keyGroundLeft) && Progress::getInstance().canMoveMap())
	{
		if (m_keyGroundRight)
			m_groundManager->setNextGenerationState(GroundManager::GenerationState::Previous, m_octo->getPosition());
		else
			m_groundManager->setNextGenerationState(GroundManager::GenerationState::Next, m_octo->getPosition());

		if (m_groundSoundTime < m_groundSoundTimeMax)
			m_groundSoundTime += frameTime;
	}
	volume = m_groundVolume * (m_groundSoundTime / m_groundSoundTimeMax);
	m_soundGeneration->setVolume(volume * audio.getSoundVolume());

	if (Progress::getInstance().isMenu() || (ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Duplicate).enable() && !Progress::getInstance().isValidateChallenge(ChallengeManager::Effect::Duplicate)))
		m_groundManager->setNextGenerationState(GroundManager::GenerationState::Previous, m_octo->getPosition());
}

bool	Game::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::GroundLeft:
			m_keyGroundLeft = true;
			Progress::getInstance().moveMap();
			break;
		case OctoKeys::GroundRight:
			m_keyGroundRight = true;
			Progress::getInstance().moveMap();
			break;
		case OctoKeys::Infos:
			m_cameraMovement->shake(5.f, 1.f, 0.01f);
			m_slowTimeInfosCoef = 10.f;
			break;
		default:
			break;
	}
	return true;
}

bool	Game::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::GroundLeft:
			m_keyGroundLeft = false;
			break;
		case OctoKeys::GroundRight:
			m_keyGroundRight = false;
			break;
		case OctoKeys::Infos:
			m_slowTimeInfosCoef = 1.f;
			break;
		default:
			break;
	}
	return true;
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	if (m_skipFrames < m_skipFramesMax)
		return;
	render.clear();
	render.draw(m_skyManager->getDecorsBack(), states);
	render.draw(*m_parallaxScrolling, states);
	//m_musicPlayer.debugDraw(render);
	m_physicsEngine.debugDraw(render);
	m_groundManager->drawBack(render, states);
	render.draw(*m_octo, states);
	m_groundManager->drawFront(render, states);
	m_octo->drawNanoRobot(render, states);
	render.draw(m_skyManager->getDecorsFront(), states);
	m_groundManager->drawWater(render, states);
	render.draw(m_skyManager->getFilter(), states);
	m_groundManager->drawText(render, states);
	m_octo->drawText(render, states);
	render.draw(*m_konami);
	//m_cameraMovement->debugDraw(render);
}
