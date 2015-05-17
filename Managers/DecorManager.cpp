#include "DecorManager.hpp"
#include "MapManager.hpp"
#include "Tree.hpp"
#include "Crystal.hpp"
#include "Rock.hpp"
#include "Star.hpp"
#include "Sun.hpp"

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
	int max = m_biome->mn_width;
	int min = 0;
	int i = 0;
	m_decors.push_back(new Sun());
	m_decors[i]->init(p_biome);
	i++;
	while (i < m_biome->m_rock.mn_nb)
	{
		m_decors.push_back(new Rock());
		m_decors[i]->init(p_biome);
		x = random() % m_biome->mn_width;
		m_decors[i]->setPosition(sf::Vector2f(x, 0.f));
		i++;
	}
	while (i < m_biome->m_tree.mn_nb + m_biome->m_rock.mn_nb)
	{
		m_decors.push_back(new Tree());
		m_decors[i]->init(p_biome);
		x = static_cast<float>(random() % (max - min) + min);
		m_decors[i]->setPosition(sf::Vector2f(x, 0.f));
		max -= m_biome->mn_width / m_biome->mn_nbDecor / 2.0f;
		min += m_biome->mn_width / m_biome->mn_nbDecor / 2.0f;
		i++;
	}
	while (i < m_biome->m_crystal.mn_nb + m_biome->m_tree.mn_nb + m_biome->m_rock.mn_nb)
	{
		m_decors.push_back(new Crystal());
		m_decors[i]->init(p_biome);
		x = random() % m_biome->mn_width;
		m_decors[i]->setPosition(sf::Vector2f(x, 0.f));
		i++;
	}
	//m_decors[i]->setPosition(sf::Vector2f(x, m_mapManager->getTransitionManager().getHeight(x)->position.y));
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
	m_offsetX = m_mapManager->getTransitionManager().getMapOffsetX() * 8;
	int delta = 500;
	for (size_t i = 0; i < m_decors.size(); i++)
	{
		int originX = m_decors[i]->getOriginX();
		if (originX >= m_offsetX - delta && originX <= m_offsetX + 1900 + delta)
			m_decors[i]->update(pf_deltatime);
		else
			m_decors[i]->updateOrigin();
	}
}

void DecorManager::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	int delta = 500;
	for (size_t i = 0; i < m_decors.size(); i++)
	{
		int originX = m_decors[i]->getOriginX();
		if (originX >= m_offsetX - delta && originX <= m_offsetX + 1900 + delta)
			render.draw(*m_decors[i]);
	}
}

bool DecorManager::onPressed (sf::Event::KeyEvent const &event)
{
	if (event.code == sf::Keyboard::P)
	{
		for (size_t i = 0; i < m_decors.size(); i++)
			m_decors[i]->randomDecor();
	}
	return true;
}
