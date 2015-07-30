#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "RectangleShape.hpp"
#include "MapInstance.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>
#include <Interpolations.hpp>

Game::Game() :
	m_engine(PhysicsEngine::getInstance())
{
}

void	Game::setup()
{
	m_biomeManager.registerBiome<DefaultBiome>("test");
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	m_engine.setIterationCount(4u);
	m_engine.setTileCollision(true);
	m_engine.setContactListener(this);
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);

	m_gameClock.setup(m_biomeManager.getCurrentBiome());
	m_skyManager.setup(m_biomeManager.getCurrentBiome(), m_gameClock);
	m_groundManager.setup(m_biomeManager.getCurrentBiome());
	m_parallaxScrolling.setup(m_biomeManager.getCurrentBiome());

	auto const & instances = m_biomeManager.getCurrentBiome().getInstances();
	for (auto & instance : instances)
	{
		octo::LevelMap const & levelMap = octo::Application::getResourceManager().getLevelMap(instance.second);
		for (std::size_t i = 0u; i < levelMap.getSpriteCount(); i++)
		{
			//TODO: finish this
			octo::LevelMap::SpriteTrigger const & spriteTrigger = levelMap.getSprite(i);
			RectangleShape * rect = m_engine.createRectangle();
			rect->setPosition(sf::Vector2f(spriteTrigger.trigger.getPosition().x + instance.first * Tile::TileSize, (-levelMap.getMapSize().y + MapInstance::HeightOffset) * Tile::TileSize + spriteTrigger.trigger.getPosition().y));
			rect->setSize(spriteTrigger.trigger.getSize());
			rect->setApplyGravity(false);
			rect->setType(AShape::Type::e_trigger);
		}
	}
}

void	Game::update(sf::Time frameTime)
{
	m_gameClock.update(frameTime);
	m_skyManager.update(frameTime);
	m_groundManager.update(frameTime.asSeconds());
	m_parallaxScrolling.update(frameTime.asSeconds());
	m_engine.update(frameTime.asSeconds());
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
		default:
		break;
	}
	return true;
}

void Game::onShapeCollision(AShape * shapeA, AShape * shapeB)
{
	//TODO: implements gameobject behaviour
	(void)shapeA;
	(void)shapeB;
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyManager.getDecorsBack(), states);
	render.draw(m_parallaxScrolling, states);
	render.draw(m_groundManager.getDecorsBack(), states);
	// Draw Octo and pnj
	render.draw(m_groundManager.getDecorsFront(), states);
	render.draw(m_groundManager, states);
	render.draw(m_groundManager.getDecorsGround(), states);
	render.draw(m_skyManager.getDecorsFront(), states);
	m_engine.debugDraw(render);
}
