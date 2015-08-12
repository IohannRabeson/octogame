#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"
#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "RectangleShape.hpp"
#include "MapInstance.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>
#include <Interpolations.hpp>

Game::Game() :
	m_physicsEngine(PhysicsEngine::getInstance())
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

	m_skyCycle.setup(m_biomeManager.getCurrentBiome());
	m_skyManager.setup(m_biomeManager.getCurrentBiome(), m_skyCycle);
	m_groundManager.setup(m_biomeManager.getCurrentBiome(), m_skyCycle);
	m_parallaxScrolling.setup(m_biomeManager.getCurrentBiome(), m_skyCycle);

	m_physicsEngine.setIterationCount(4u);
	m_physicsEngine.setTileCollision(true);
	m_physicsEngine.setContactListener(this);

	m_npc.setup(sf::FloatRect(25, 0, 500, 0));
}

void	Game::update(sf::Time frameTime)
{
	m_skyCycle.update(frameTime, m_biomeManager.getCurrentBiome());
	m_groundManager.update(frameTime.asSeconds());
	m_parallaxScrolling.update(frameTime.asSeconds());
	m_physicsEngine.update(frameTime.asSeconds());
	m_npc.update(frameTime);
	m_skyManager.update(frameTime);
}

void Game::onShapeCollision(AShape * shapeA, AShape * shapeB)
{
	// don't forget to check if shapeA->getGameObject() != nullptr
	// Utiliser gameObjectCast pour réupérer le bon objet avec shapeA->getGameObject()
	(void)shapeA;
	(void)shapeB;
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

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyManager.getDecorsBack(), states);
	render.draw(m_parallaxScrolling, states);
	render.draw(m_groundManager.getDecorsBack(), states);
	// Draw Octo and pnj
	m_physicsEngine.debugDraw(render);
	render.draw(m_npc, states);
	render.draw(m_groundManager.getDecorsFront(), states);
	render.draw(m_skyManager.getDecorsFront(), states);
	render.draw(m_groundManager, states);
	render.draw(m_groundManager.getDecorsGround(), states);
	render.draw(m_skyManager.getFilter(), states);
}
