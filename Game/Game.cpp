#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "IceABiome.hpp"
#include "IceBBiome.hpp"
#include "IceCBiome.hpp"
#include "IceDBiome.hpp"
#include "DesertABiome.hpp"
#include "JungleABiome.hpp"
#include "WaterABiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"
#include "ElevatorStream.hpp"
#include "Bouibouik.hpp"
#include "Tent.hpp"
#include "SpaceShip.hpp"
#include "Concert.hpp"
#include "AGameObject.hpp"
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
#include "RepairShipNanoRobot.hpp"
#include "JumpNanoRobot.hpp"
#include "SlowFallNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "WaterNanoRobot.hpp"
//Script AddNpc Include
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
#include "CedricNpc.hpp"
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
	m_skyCycle(nullptr),
	m_skyManager(nullptr),
	m_groundManager(nullptr),
	m_parallaxScrolling(nullptr),
	m_musicPlayer(nullptr),
	m_octo(nullptr),
	m_konami(nullptr),
	m_keyGroundRight(false),
	m_keyGroundLeft(false),
	m_soundGeneration(nullptr),
	m_groundVolume(100.f),
	m_groundSoundTime(sf::Time::Zero),
	m_groundSoundTimeMax(sf::seconds(0.6f))
{
	InputListener::addInputListener();

	m_biomeManager.registerBiome<IceABiome>(Level::IceA);
	m_biomeManager.registerBiome<IceBBiome>(Level::IceB);
	m_biomeManager.registerBiome<IceCBiome>(Level::IceC);
	m_biomeManager.registerBiome<IceDBiome>(Level::IceD);
	m_biomeManager.registerBiome<DesertABiome>(Level::DesertA);
	m_biomeManager.registerBiome<JungleABiome>(Level::JungleA);
	m_biomeManager.registerBiome<WaterABiome>(Level::WaterA);
	m_biomeManager.registerBiome<DefaultBiome>(Level::Default);
}

Game::~Game(void)
{
	if (m_soundGeneration != nullptr)
		m_soundGeneration->stop();
	InputListener::removeInputListener();
	Progress::getInstance().save();
}

void	Game::loadLevel(void)
{
	m_biomeManager.changeBiome(Progress::getInstance().getNextDestination(), 0x12345);
	Progress::getInstance().setLastDestination(m_biomeManager.getCurrentBiome().getId());

	octo::AudioManager& audio = octo::Application::getAudioManager();
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	sf::Vector2f const & startPosition = m_biomeManager.getCurrentBiome().getOctoStartPosition();

	// Reset last values
	postEffect.removeEffects();
	audio.reset();
	// Reset PhysycsEngine
	octo::Application::getCamera().setCenter(startPosition - sf::Vector2f(0.f, 200.f));
	m_physicsEngine.unregisterAllShapes();
	m_physicsEngine.unregisterAllTiles();
	m_physicsEngine.setIterationCount(8u);
	m_physicsEngine.setGravity(sf::Vector2f(0.f, 600.f));
	m_physicsEngine.setTileCollision(true);
	m_physicsEngine.setContactListener(this);

	m_skyCycle.reset(new SkyCycle());
	m_skyManager.reset(new SkyManager());
	m_groundManager.reset(new GroundManager());
	m_parallaxScrolling.reset(new ParallaxScrolling());
	m_musicPlayer.reset(new MusicManager());
	m_octo.reset(new CharacterOcto());
	m_konami.reset(new KonamiCode());

	m_skyCycle->setup(m_biomeManager.getCurrentBiome());
	m_skyManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_groundManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_parallaxScrolling->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_musicPlayer->setup(m_biomeManager.getCurrentBiome());
	m_octo->setup(m_biomeManager.getCurrentBiome());
	m_octo->setStartPosition(startPosition);
}

sf::Vector2f	Game::getOctoBubblePosition(void) const
{
	return m_octo->getBubblePosition();
}

void	Game::update(sf::Time frameTime)
{
	// update the PhysicsEngine as first
	m_physicsEngine.update(frameTime.asSeconds());
	sf::Vector2f const & octoPos = m_octo->getPosition();
	sf::Listener::setPosition(sf::Vector3f(octoPos.x, octoPos.y, 0.f));
	m_octo->update(frameTime);
	m_skyCycle->update(frameTime, m_biomeManager.getCurrentBiome());
	moveMap(frameTime);
	m_groundManager->update(frameTime.asSeconds());
	m_parallaxScrolling->update(frameTime.asSeconds());
	m_skyManager->update(frameTime);
	m_musicPlayer->update(frameTime, m_octo->getPosition());
	m_konami->update(frameTime, m_octo->getPosition());
	m_octo->startKonamiCode(m_konami->canStartEvent());
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
				octo->onCollision(GameObjectType::Elevator, collisionDirection);
			}
			break;
		case GameObjectType::Tent:
			gameObjectCast<Tent>(gameObject)->startBalle();
			break;
		case GameObjectType::Concert:
			gameObjectCast<Concert>(gameObject)->startBalle();
			m_musicPlayer->startBalleMusic(gameObjectCast<Concert>(gameObject)->getEffectDuration(), MusicManager::MusicNameArea::Concert);
			break;
		case GameObjectType::Bouibouik:
			gameObjectCast<Bouibouik>(gameObject)->startBalle();
			break;
		case GameObjectType::Portal:
			octo->usePortal(*gameObjectCast<Portal>(gameObject));
			break;
		case GameObjectType::CedricNpc:
			gameObjectCast<CedricNpc>(gameObject)->startBalle();
			if (gameObjectCast<CedricNpc>(gameObject)->getId() == 0u)
				m_musicPlayer->startBalleMusic(gameObjectCast<CedricNpc>(gameObject)->getEffectDuration(), MusicManager::MusicNameArea::CedricChallenge);
			break;
		case GameObjectType::JumpNanoRobot:
			if (!gameObjectCast<JumpNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canJump())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<JumpNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr);
			}
			break;
		case GameObjectType::DoubleJumpNanoRobot:
			if (!gameObjectCast<DoubleJumpNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canDoubleJump() && Progress::getInstance().canOpenDoubleJump() && !Progress::getInstance().canValidChallenge())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<DoubleJumpNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr);
			}
			break;
		case GameObjectType::GroundTransformNanoRobot:
			if (!gameObjectCast<GroundTransformNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canMoveMap())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<GroundTransformNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr);
			}
			break;
		case GameObjectType::SlowFallNanoRobot:
			if (!gameObjectCast<SlowFallNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canSlowFall())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<SlowFallNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(ptr);
			}
			break;
		case GameObjectType::WaterNanoRobot:
			if (!gameObjectCast<WaterNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canUseWaterJump())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<WaterNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(static_cast<WaterNanoRobot *>(ptr));
			}
			break;
		case GameObjectType::RepairNanoRobot:
			if (!gameObjectCast<RepairNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canRepair())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<RepairNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveRepairNanoRobot(static_cast<RepairNanoRobot *>(ptr));
			}
			break;
		case GameObjectType::RepairShipNanoRobot:
			if (!gameObjectCast<RepairShipNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canRepairShip())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<RepairShipNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveNanoRobot(static_cast<RepairShipNanoRobot *>(ptr));
			}
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
		case GameObjectType::CedricNpc:
			gameObjectCast<CedricNpc>(gameObject)->collideOctoEvent(octo);
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
		m_octo->onCollision(GameObjectType::Tile, collisionDirection);
	}
	(void)tileShape;
}

void Game::moveMap(sf::Time frameTime)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	float						volume = 0.f;

	if (m_soundGeneration != nullptr && !m_keyGroundRight && !m_keyGroundLeft && !Progress::getInstance().canValidChallenge())
	{
		m_groundSoundTime -= frameTime;
		if (m_groundSoundTime < sf::Time::Zero)
			m_groundSoundTime = sf::Time::Zero;
		volume = m_groundVolume * (m_groundSoundTime / m_groundSoundTimeMax);
		m_soundGeneration->setVolume(volume);
	}
	if (m_keyGroundRight || m_keyGroundLeft || Progress::getInstance().canValidChallenge())
	{
		if (Progress::getInstance().canMoveMap())
		{
			if (m_keyGroundRight)
				m_groundManager->setNextGenerationState(GroundManager::GenerationState::Previous);
			else if (m_keyGroundLeft)
				m_groundManager->setNextGenerationState(GroundManager::GenerationState::Next);
			else
				m_groundManager->setNextGenerationState(GroundManager::GenerationState::Next);
			if (m_soundGeneration == nullptr)
			{
				m_soundGeneration = audio.playSound(resources.getSound(GROUND_OGG), 0.f);
				m_soundGeneration->setLoop(true);
			}
			else
			{
				m_groundSoundTime += frameTime;
				if (m_groundSoundTime > m_groundSoundTimeMax)
					m_groundSoundTime = m_groundSoundTimeMax;
				volume = m_groundVolume * (m_groundSoundTime / m_groundSoundTimeMax);
				m_soundGeneration->setVolume(volume);
			}
		}
	}
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
		default:
			break;
	}
	return true;
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyManager->getDecorsBack(), states);
	render.draw(*m_parallaxScrolling, states);
	//m_musicPlayer->debugDraw(render);
	//m_physicsEngine.debugDraw(render);
	m_groundManager->drawBack(render, states);
	render.draw(*m_octo, states);
	m_groundManager->drawFront(render, states);
	render.draw(m_skyManager->getDecorsFront(), states);
	m_octo->drawNanoRobot(render, states);
	m_skyManager->drawBirds(render, states);
	m_groundManager->drawWater(render, states);
	render.draw(m_skyManager->getFilter(), states);
	m_groundManager->drawText(render, states);
	m_octo->drawText(render, states);
	render.draw(*m_konami);
}
