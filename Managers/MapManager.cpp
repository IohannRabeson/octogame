#include "MapManager.hpp"
#include "../ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>

MapManager::MapManager(void) :
	m_texture(nullptr)
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(&m_decorManager);
}

MapManager::~MapManager(void)
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(&m_decorManager);
}

void MapManager::init(Map::EMapType p_mapType)
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();

	m_transitionManager.init(this, &m_biome, p_mapType);
	m_decorManager.init(this, &m_biome);
	m_collisionManager.init(this);
	m_texture = &resources.getTexture(OCTO_PNG);
}

void MapManager::update(float pf_deltatime)
{
	/*static float accumulator = 0.f;
	static sf::Clock clock;
	float dt = 1.f / 120.f;

	// Always update TransitionManager first
	m_transitionManager.update(pf_deltatime);
	m_staticObjectManager.update(pf_deltatime);

	accumulator += clock.getElapsedTime().asSeconds();
	if (accumulator > 0.2f)
		accumulator = 0.2f;

	while (accumulator > dt)
	{
		m_collisionManager.update(dt);
		accumulator -= dt;
	}

	//float alpha = accumulator / dt;
	clock.restart();
*/
	m_transitionManager.update(pf_deltatime);
	m_decorManager.update(pf_deltatime);
	m_collisionManager.update(pf_deltatime);
}

void MapManager::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	render.draw(m_decorManager);
	render.draw(m_transitionManager);

	/*sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(m_collisionManager.getPlayer().getGlobalBounds().left, m_collisionManager.getPlayer().getGlobalBounds().top));
	rect.setSize(sf::Vector2f(m_collisionManager.getPlayer().getGlobalBounds().width, m_collisionManager.getPlayer().getGlobalBounds().height));
	render.draw(rect);
	*/
	states.texture = m_texture;
	render.draw(&m_collisionManager.getPlayer().m_vertices[0], 4, sf::Quads, states);
	m_collisionManager.debugDraw(render);
}

TransitionManager & MapManager::getTransitionManager(void)
{
	return m_transitionManager;
}

DecorManager & MapManager::getDecorManager(void)
{
	return m_decorManager;
}

Player & MapManager::getPlayer(void) const
{
	return m_collisionManager.getPlayer();
}
