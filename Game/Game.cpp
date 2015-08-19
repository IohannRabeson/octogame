#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

Game::Game() :
	m_physicsEngine(PhysicsEngine::getInstance()),
	m_skyCycle(nullptr),
	m_skyManager(nullptr),
	m_groundManager(nullptr),
	m_parallaxScrolling(nullptr),
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
	m_physicsEngine.setTileCollision(true);
	m_physicsEngine.setContactListener(this);

	m_skyCycle.reset(new SkyCycle());
	m_skyManager.reset(new SkyManager());
	m_sunLight.reset(new SunLight());
	m_groundManager.reset(new GroundManager());
	m_parallaxScrolling.reset(new ParallaxScrolling());
	m_octo.reset(new CharacterOcto());
	m_npc.reset(new CharacterNpc());

	m_skyCycle->setup(m_biomeManager.getCurrentBiome());
	m_skyManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_sunLight->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_groundManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_parallaxScrolling->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_octo->setup();
	m_npc->setup(sf::Vector2f(0, 0), sf::FloatRect(0, 0, 800, 0));

	octo::Camera & camera = octo::Application::getCamera();
	camera.setCenter(m_octo->getPosition());
}

void	Game::update(sf::Time frameTime)
{
	m_physicsEngine.update(frameTime.asSeconds());
	m_octo->update(frameTime);
	followPlayer();
	m_skyCycle->update(frameTime, m_biomeManager.getCurrentBiome());
	m_sunLight->update(frameTime);
	m_groundManager->update(frameTime.asSeconds());
	m_parallaxScrolling->update(frameTime.asSeconds());
	m_npc->update(frameTime);
	m_skyManager->update(frameTime);
}

void Game::onShapeCollision(AShape * shapeA, AShape * shapeB)
{
	(void) shapeA;
	(void) shapeB;
	// don't forget to check if shapeA->getGameObject() != nullptr
	// Utiliser gameObjectCast pour réupérer le bon objet avec shapeA->getGameObject()
}

void Game::onTileShapeCollision(TileShape * tileShape, AShape * shape)
{
	if (shape->getGameObject() != nullptr
			&& gameObjectCast<CharacterOcto>(shape->getGameObject()) != nullptr)
		m_octo->onCollision(GameObjectType::Tile);

	// don't forget to check if shapeA->getGameObject() != nullptr
	// Utiliser gameObjectCast pour réupérer le bon objet avec shapeA->getGameObject()
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
	render.draw(m_groundManager->getDecorsBack(), states);
	// Draw Octo and pnj
	render.draw(*m_octo, states);
	render.draw(*m_npc, states);
//	m_physicsEngine.debugDraw(render);
	render.draw(m_groundManager->getDecorsFront(), states);
	render.draw(m_skyManager->getDecorsFront(), states);
	render.draw(*m_groundManager, states);
	render.draw(m_groundManager->getDecorsGround(), states);
	render.draw(*m_sunLight, states);
}

void	Game::followPlayer()
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::Vector2f cameraPos = camera.getCenter();
	sf::Vector2f const & octoPos = m_octo->getPosition();
	cameraPos.x = octo::linearInterpolation(octoPos.x, cameraPos.x, 0.98);
	cameraPos.y = octo::linearInterpolation(octoPos.y, cameraPos.y, 0.9);

	camera.setCenter(cameraPos);
}
