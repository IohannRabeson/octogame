#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"
#include "ElevatorStream.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <SFML/Audio/Listener.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

Game::Game() :
	m_physicsEngine(PhysicsEngine::getInstance()),
	m_skyCycle(nullptr),
	m_skyManager(nullptr),
	m_groundManager(nullptr),
	m_parallaxScrolling(nullptr),
	m_musicPlayer(nullptr),
	m_octo(nullptr),
	m_npc(nullptr)
{
}

void	Game::setup()
{
	m_biomeManager.registerBiome<DefaultBiome>("test");

	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);

	// Reset last values
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.removeShaders();

	// Reset PhysycsEngine
	m_physicsEngine.unregisterAllShapes();
	m_physicsEngine.unregisterAllTiles();
	m_physicsEngine.setIterationCount(octo::Application::getOptions().getValue<std::size_t>("iteration_count"));
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
	m_octo->setup();

	// TODO: fix, for npcs, if we dont update once, value are not initialized well, and npc go through instance map
	update(sf::seconds(0.f));
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
	if (shapeA->getGameObject() != nullptr
			&& gameObjectCast<CharacterOcto>(shapeA->getGameObject()) != nullptr
			&& shapeB->getGameObject() != nullptr
			&& gameObjectCast<ElevatorStream>(shapeB->getGameObject()) != nullptr)
	{
		m_octo->setTopElevator(gameObjectCast<ElevatorStream>(shapeB->getGameObject())->getTopY());
		m_octo->onCollision(GameObjectType::Elevator, collisionDirection);
	}
}

void Game::onTileShapeCollision(TileShape * tileShape, AShape * shape, sf::Vector2f const & collisionDirection)
{
	if (shape->getGameObject() != nullptr
			&& gameObjectCast<CharacterOcto>(shape->getGameObject()) != nullptr)
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
			m_groundManager->setNextGenerationState(GroundManager::GenerationState::Next);
		break;
		case sf::Keyboard::R:
			m_groundManager->setNextGenerationState(GroundManager::GenerationState::Previous);
		break;
		case sf::Keyboard::L:
			loadLevel("new_biome");
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
	m_groundManager->drawFront(render, states);
	render.draw(m_skyManager->getDecorsFront(), states);
	render.draw(m_skyManager->getFilter(), states);
	//m_physicsEngine.debugDraw(render);
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
