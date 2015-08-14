#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"
#include "MapInstance.hpp"

#include "PixelGlitch.hpp"

#include "PixelPotion.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>
#include <Interpolations.hpp>
#include <Options.hpp>
#include <Console.hpp>

Game::Game() :
	m_physicsEngine(PhysicsEngine::getInstance())
{
}

void	Game::setup()
{
	m_biomeManager.registerBiome<DefaultBiome>("test");
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	octo::Console&			console = octo::Application::getConsole();

	graphics.addKeyboardListener(this);
	graphics.addKeyboardListener(&m_octo);
	
	// Register glitches
	m_glitchManager.addGlitch(std::unique_ptr<PixelGlitch>(new PixelGlitch()));

	// Register potions
	m_potionManager.addPotion("pixels", std::unique_ptr<PixelPotion>(new PixelPotion()));

	// Register commands
	console.addCommand(L"test.potion.spawn", [this](std::string const& key)
			{
				try
				{
					m_potionManager.startPotion(key);
				}
				catch (std::exception const& e)
				{
					octo::Console&	console = octo::Application::getConsole();

					console.printError(e);
				}
					
			});
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);

	m_skyCycle.setup(m_biomeManager.getCurrentBiome());
	m_skyManager.setup(m_biomeManager.getCurrentBiome(), m_skyCycle);
	m_groundManager.setup(m_biomeManager.getCurrentBiome(), m_skyCycle);
	m_parallaxScrolling.setup(m_biomeManager.getCurrentBiome(), m_skyCycle);

	m_physicsEngine.setIterationCount(octo::Application::getOptions().getValue<std::size_t>("iteration_count"));
	m_physicsEngine.setTileCollision(true);
	m_physicsEngine.setContactListener(this);
}

void	Game::update(sf::Time frameTime)
{
	m_skyCycle.update(frameTime, m_biomeManager.getCurrentBiome());
	m_groundManager.update(frameTime.asSeconds());
	m_parallaxScrolling.update(frameTime.asSeconds());
	m_physicsEngine.update(frameTime.asSeconds());
	m_octo.update(frameTime);
	followPlayer();
	m_skyManager.update(frameTime);
	m_glitchManager.update(frameTime);
	m_potionManager.update(frameTime);
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
		m_octo.onCollision(GameObjectType::Tile);

	// don't forget to check if shapeA->getGameObject() != nullptr
	// Utiliser gameObjectCast pour réupérer le bon objet avec shapeA->getGameObject()
	(void)tileShape;
}

bool Game::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::E:
			m_groundManager.setNextGenerationState(GroundManager::GenerationState::Next);
			break;
		case sf::Keyboard::R:
			m_groundManager.setNextGenerationState(GroundManager::GenerationState::Previous);
			break;
		case sf::Keyboard::F1:
			m_glitchManager.startRandomGlitch(sf::seconds(0.15f));
			break;
		default:
		break;
	}
	return true;
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyManager.getDecorsBack(), states);
	render.draw(m_parallaxScrolling, states);
	render.draw(m_groundManager.getDecorsBack(), states);
	// Draw pnj
//	m_physicsEngine.debugDraw(render);
	render.draw(m_octo, states);
	render.draw(m_groundManager.getDecorsFront(), states);
	render.draw(m_skyManager.getDecorsFront(), states);
	render.draw(m_groundManager, states);
	render.draw(m_groundManager.getDecorsGround(), states);
	render.draw(m_skyManager.getFilter(), states);

}

void	Game::followPlayer()
{
	octo::Camera&	camera = octo::Application::getCamera();
	m_cameraPos = camera.getCenter();
	m_octoPos = m_octo.getPosition();
	sf::Vector2f cameraPos;
	cameraPos.x = octo::linearInterpolation(m_octoPos.x, m_cameraPos.x, 0.98);
	cameraPos.y = octo::linearInterpolation(m_octoPos.y, m_cameraPos.y, 0.9);

	camera.setCenter(cameraPos);
}
