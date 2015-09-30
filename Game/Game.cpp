#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "LevelOneBiome.hpp"
#include "LevelTwoBiome.hpp"
#include "LevelThreeBiome.hpp"
#include "LevelFourBiome.hpp"
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
#include "JumpNanoRobot.hpp"
#include "DoubleJumpNanoRobot.hpp"
#include "FranfranNpc.hpp"
#include "JuNpc.hpp"
#include "FannyNpc.hpp"
#include "CedricNpc.hpp"
#include "GuiNpc.hpp"
#include "PunkNpc.hpp"
#include "JeffMouffyNpc.hpp"
#include "TurbanNpc.hpp"
#include "VinceNpc.hpp"
#include "AmandineNpc.hpp"
#include "BrayouNpc.hpp"
#include "EvaNpc.hpp"
#include "OldDesertStaticNpc.hpp"
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
	m_keyS(false),
	m_keyF(false),
	m_soundGeneration(nullptr)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	m_biomeManager.registerBiome<LevelOneBiome>(Level::LevelOne);
	m_biomeManager.registerBiome<LevelTwoBiome>(Level::LevelTwo);
	m_biomeManager.registerBiome<LevelThreeBiome>(Level::LevelThree);
	m_biomeManager.registerBiome<LevelFourBiome>(Level::LevelFour);
	m_biomeManager.registerBiome<DefaultBiome>(Level::Default);
}

Game::~Game(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void	Game::loadLevel(void)
{
	m_biomeManager.changeBiome(Progress::getInstance().getNextDestination(), 0x12345);

	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	sf::Vector2f const & startPosition = m_biomeManager.getCurrentBiome().getOctoStartPosition();

	// Reset last values
	postEffect.removeEffects();
	// Reset PhysycsEngine
	octo::Application::getCamera().setCenter(startPosition);
	m_physicsEngine.unregisterAllShapes();
	m_physicsEngine.unregisterAllTiles();
	m_physicsEngine.setIterationCount(octo::Application::getOptions().getValue<std::size_t>("iteration_count")); // TODO : remove from default
	m_physicsEngine.setGravity(sf::Vector2f(0.f, 600.f));
	m_physicsEngine.setTileCollision(true);
	m_physicsEngine.setContactListener(this);

	m_skyCycle.reset(new SkyCycle());
	m_skyManager.reset(new SkyManager());
	m_groundManager.reset(new GroundManager());
	m_parallaxScrolling.reset(new ParallaxScrolling());
	m_musicPlayer.reset(new MusicPlayer());
	m_octo.reset(new CharacterOcto());

	m_skyCycle->setup(m_biomeManager.getCurrentBiome());
	m_skyManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_groundManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_parallaxScrolling->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_musicPlayer->setup(m_biomeManager.getCurrentBiome());
	m_octo->setup(m_biomeManager.getCurrentBiome());
	m_octo->setPosition(startPosition);

	//TODO: THINK ABOUT A BETTER WAY
	update(sf::seconds(0.0001f));
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
	followPlayer(frameTime);
	m_skyCycle->update(frameTime, m_biomeManager.getCurrentBiome());
	moveMap();
	m_groundManager->update(frameTime.asSeconds());
	m_parallaxScrolling->update(frameTime.asSeconds());
	m_skyManager->update(frameTime);
	m_musicPlayer->update(frameTime, m_octo->getPosition());
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
			break;
		case GameObjectType::Bouibouik:
			gameObjectCast<Bouibouik>(gameObject)->startBalle();
			break;
		case GameObjectType::Portal:
			octo->usePortal(*gameObjectCast<Portal>(gameObject));
			break;
		case GameObjectType::CedricNpc:
			gameObjectCast<CedricNpc>(gameObject)->startBalle();
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
			if (!gameObjectCast<DoubleJumpNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canDoubleJump() && Progress::getInstance().canOpenDoubleJump())
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
		case GameObjectType::RepairNanoRobot:
			if (!gameObjectCast<RepairNanoRobot>(gameObject)->isTravelling() && !Progress::getInstance().canRepair())
			{
				NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<RepairNanoRobot>(gameObject));
				ptr->transfertToOcto();
				m_octo->giveRepairNanoRobot(static_cast<RepairNanoRobot *>(ptr));
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
		case GameObjectType::FranfranNpc:
			gameObjectCast<FranfranNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::EvaNpc:
			gameObjectCast<EvaNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::BrayouNpc:
			gameObjectCast<BrayouNpc>(gameObject)->collideOctoEvent(octo);
			break;
		case GameObjectType::AmandineNpc:
			gameObjectCast<AmandineNpc>(gameObject)->collideOctoEvent(octo);
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
		case GameObjectType::JeffMouffyNpc:
			gameObjectCast<JeffMouffyNpc>(gameObject)->collideOctoEvent(octo);
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

void Game::moveMap()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	if (m_soundGeneration != nullptr && !m_keyS && !m_keyF)
	{
		m_soundGeneration->stop();
		m_soundGeneration = nullptr;
	}
	if (m_keyS || Progress::getInstance().canValidChallenge())
	{
		if (Progress::getInstance().canMoveMap())
		{
			m_groundManager->setNextGenerationState(GroundManager::GenerationState::Next);
			if (m_soundGeneration == nullptr)
			{
				m_soundGeneration = audio.playSound(resources.getSound(GROUND_WAV));
				m_soundGeneration->setLoop(true);
			}
		}
	}
	else if (m_keyF)
	{
		if (Progress::getInstance().canMoveMap())
		{
			m_groundManager->setNextGenerationState(GroundManager::GenerationState::Previous);
			if (m_soundGeneration == nullptr)
			{
				m_soundGeneration = audio.playSound(resources.getSound(GROUND_WAV), 1.f);
				m_soundGeneration->setLoop(true);
			}
		}
	}
}

bool Game::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::S:
			m_keyS = true;
			Progress::getInstance().moveMap();
			break;
		case sf::Keyboard::F:
			Progress::getInstance().moveMap();
			m_keyF = true;
			break;
		default:
			break;
	}
	return true;
}

bool	Game::onReleased(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::S:
			m_keyS = false;
			break;
		case sf::Keyboard::F:
			m_keyF = false;
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
	m_groundManager->drawBack(render, states);
	render.draw(*m_octo, states);
	//m_physicsEngine.debugDraw(render);
	m_groundManager->drawFront(render, states);
	render.draw(m_skyManager->getDecorsFront(), states);
	m_octo->drawNanoRobot(render, states);
	m_skyManager->drawBirds(render, states);
	m_groundManager->drawWater(render, states);
	render.draw(m_skyManager->getFilter(), states);
	m_groundManager->drawText(render, states);
	m_octo->drawText(render, states);
}

void	Game::followPlayer(sf::Time frameTime)
{
	float frameTimeSeconds = frameTime.asSeconds();
	octo::Camera & camera = octo::Application::getCamera();
	sf::Vector2f const & cameraSize = camera.getSize();
	sf::Vector2f cameraPos = camera.getCenter();
	sf::Vector2f octoUpPos = m_octo->getPosition();
	sf::Vector2f octoDownPos = octoUpPos;

	octoDownPos.y -= cameraSize.y / 4.f;
	cameraPos.x = octo::linearInterpolation(octoUpPos.x, cameraPos.x, 1.f - frameTimeSeconds);
	if (octoDownPos.y >= cameraPos.y)
		cameraPos.y = octo::linearInterpolation(octoDownPos.y, cameraPos.y, 1.f - frameTimeSeconds * 5.f);
	else if (octoUpPos.y <= cameraPos.y)
		cameraPos.y = octo::linearInterpolation(octoUpPos.y , cameraPos.y, 1.f - frameTimeSeconds * 4.f);

	camera.setCenter(cameraPos);
}
