#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "LevelOneBiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"
#include "ElevatorStream.hpp"
#include "Progress.hpp"
#include "AGameObject.hpp"
#include "GroundTransformNanoRobot.hpp"
#include "RepairNanoRobot.hpp"
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
	m_octo(nullptr)
{
	//TODO remove
	Progress::getInstance().setCanWalk(true);
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

Game::~Game(void)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void	Game::setup(void)
{
	//TODO name == biome name;
	m_biomeManager.registerBiome<LevelOneBiome>("Level_One");
	m_biomeManager.registerBiome<DefaultBiome>("Default");
}

void	Game::loadLevel(std::string const & fileName)
{
	m_biomeManager.changeBiome(fileName, 0x12345);

	// Reset last values
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.removeEffects();
	// Reset PhysycsEngine
	Progress::getInstance().setupInfoLevel(m_biomeManager.getCurrentBiome());
	octo::Application::getCamera().setCenter(sf::Vector2f(0.f, 700.f));
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
	m_octo->setup();
	m_octo->setPosition(m_biomeManager.getCurrentBiome().getOctoStartPosition());
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
	if (gameObjectCast<ElevatorStream>(gameObject))
	{
		if (gameObjectCast<ElevatorStream>(gameObject)->isActivated())
		{
			octo->setTopElevator(gameObjectCast<ElevatorStream>(gameObject)->getTopY());
			octo->onCollision(GameObjectType::Elevator, collisionDirection);
		}
	}
	else if (gameObjectCast<Portal>(gameObject))
	{
		//TODO
	}
	else if (gameObjectCast<GroundTransformNanoRobot>(gameObject))
	{
		if (!gameObjectCast<GroundTransformNanoRobot>(gameObject)->isTravelling())
		{
			NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<GroundTransformNanoRobot>(gameObject));
			ptr->transfertToOcto();
			m_octo->giveNanoRobot(ptr);
		}
	}
	else if (gameObjectCast<RepairNanoRobot>(gameObject))
	{
		if (!gameObjectCast<RepairNanoRobot>(gameObject)->isTravelling())
		{
			NanoRobot * ptr = m_groundManager->getNanoRobot(gameObjectCast<RepairNanoRobot>(gameObject));
			ptr->transfertToOcto();
			m_octo->giveRepairNanoRobot(static_cast<RepairNanoRobot *>(ptr));
		}
	}
}

void Game::onCollisionEvent(CharacterOcto * octo, AGameObjectBase * gameObject, sf::Vector2f const & collisionDirection)
{
	(void)octo;
	(void)collisionDirection;
	if (gameObjectCast<ElevatorStream>(gameObject))
	{
		octo->repairElevator(*gameObjectCast<ElevatorStream>(gameObject));
	}
	else if (gameObjectCast<Portal>(gameObject))
	{
		gameObjectCast<Portal>(gameObject)->appear();
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

bool Game::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::E:
			if (Progress::getInstance().getNanoRobotCount())
				m_groundManager->setNextGenerationState(GroundManager::GenerationState::Next);
		break;
		case sf::Keyboard::R:
			if (Progress::getInstance().getNanoRobotCount())
				m_groundManager->setNextGenerationState(GroundManager::GenerationState::Previous);
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
	//m_physicsEngine.debugDraw(render);
	m_groundManager->drawBack(render, states);
	render.draw(*m_octo, states);
	m_groundManager->drawFront(render, states);
	render.draw(m_skyManager->getDecorsFront(), states);
	render.draw(m_skyManager->getFilter(), states);
	m_octo->drawNanoRobot(render, states);
	m_groundManager->drawText(render, states);
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
		cameraPos.y = octo::linearInterpolation(octoUpPos.y , cameraPos.y, 1.f - frameTimeSeconds * 2.f);

	camera.setCenter(cameraPos);
}
