#include "DecorManager.hpp"
#include "MapManager.hpp"
#include "Tree.hpp"
#include "Crystal.hpp"
#include "Rock.hpp"
#include "Star.hpp"
#include "Sun.hpp"
#include "Cloud.hpp"
#include "Moon.hpp"

#include <iostream>

DecorManager::DecorManager(void)
{
}

DecorManager::~DecorManager(void)
{
	for (size_t i = 0; i < m_decors.size(); i++)
		delete m_decors[i];
}

void DecorManager::init(MapManager * p_mapManager, Biome * p_biome)
{
	m_mapManager = p_mapManager;
	m_biome = p_biome;

	// Init decors
	float x;
	int i = 0;
	m_decors.push_back(new Sun());
	m_decors[i++]->init(p_biome);

	m_decors.push_back(new Moon());
	m_decors[i++]->init(p_biome);

	m_decors.push_back(new Star());
	m_decors[i++]->init(p_biome);

	int total = 0;
	total += m_biome->m_cloud.mn_nb;
	while (i < total)
	{
		m_decors.push_back(new Cloud());
		m_decors[i++]->init(p_biome);
	}
	total += m_biome->m_rock.mn_nb;
	while (i < total)
	{
			m_decors.push_back(new Rock());
			m_decors[i]->init(p_biome);
			x = random() % m_biome->mn_width;
			m_decors[i]->setPosition(sf::Vector2f(x, 0.f));
		i++;
	}
	total += m_biome->m_tree.mn_nb;
	while (i < total)
	{
			m_decors.push_back(new Tree());
			m_decors[i]->init(p_biome);
			x = random() % m_biome->mn_width;
			m_decors[i]->setPosition(sf::Vector2f(x, 0.f));
		i++;
	}
	total += m_biome->m_crystal.mn_nb;
	while (i < total)
	{
			m_decors.push_back(new Crystal());
			m_decors[i]->init(p_biome);
			x = random() % m_biome->mn_width;
			m_decors[i]->setPosition(sf::Vector2f(x, 0.f));
		i++;
	}
	total += m_biome->m_cloud.mn_nb;
	while (i < total)
	{
		m_decors.push_back(new Cloud());
		m_decors[i++]->init(p_biome);
	}
	setPosition();
	m_mapManager->getTransitionManager().computeDecor();
}

void DecorManager::setPosition(void)
{
	for (size_t i = 0; i < m_decors.size(); i++)
		m_decors[i]->updatePosition(m_mapManager->getTransitionManager().getHeight(m_decors[i]->getPosition().x));
}

void DecorManager::update(float pf_deltatime)
{
	m_offsetX = m_mapManager->getTransitionManager().getMapOffsetX();
	int delta = 500;
	for (size_t i = 0; i < m_decors.size(); i++)
	{
		int originX = m_decors[i]->getOriginX();
		if (originX >= m_offsetX - delta && originX <= m_offsetX + 1920 + delta)
			m_decors[i]->update(pf_deltatime);
		else
			m_decors[i]->updateOrigin(pf_deltatime);
	}
}

void DecorManager::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	int delta = 500;
	for (size_t i = 0; i < m_decors.size(); i++)
	{
		int originX = m_decors[i]->getOriginX();
		if (originX >= m_offsetX - delta && originX <= m_offsetX + 1920 + delta)
			render.draw(*m_decors[i], states);
	}
}

bool DecorManager::onPressed (sf::Event::KeyEvent const &event)
{
	if (event.code == sf::Keyboard::P)
	{
		for (size_t i = 0; i < m_decors.size(); i++)
			m_decors[i]->randomDecor();
	}
	if (event.code == sf::Keyboard::O)
	{
		for (size_t i = 0; i < m_decors.size(); i++)
			m_decors[i]->iceDecor();
	}
	return true;
}
