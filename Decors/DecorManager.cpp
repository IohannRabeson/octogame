/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 03:39:50 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 14:58:44 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DecorManager.hpp"
#include "ADecor.hpp"
#include "Crystal.hpp"
#include "Star.hpp"
#include "Cloud.hpp"
#include "Rock.hpp"
#include "Tree.hpp"
#include "Sun.hpp"
#include "Moon.hpp"
#include "Rainbow.hpp"
#include "Mushroom.hpp"
#include "GroundRock.hpp"
#include "Sky.hpp"
#include "SunLight.hpp"
#include "Grass.hpp"
#include "Progress.hpp"

#include <cassert>

DecorManager::DecorManager(std::size_t maxVertexCount) :
	m_vertices(new sf::Vertex[maxVertexCount]),
	m_count(maxVertexCount),
	m_used(0u),
	m_biome(nullptr)
{
	registerDecors();
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

DecorManager::~DecorManager()
{
	clear();
}

void	DecorManager::setup(ABiome* biome)
{
	m_biome = biome;
}

DecorManager::Iterator	DecorManager::add(DecorTypes type)
{
	std::unique_ptr<ADecor>	newDecor(m_factory.create(type));
	Iterator				it;

	it = add(newDecor.get());
	newDecor.release();
	return (it);
}

DecorManager::Iterator	DecorManager::add(ADecor* decor)
{
	assert(m_biome != nullptr);
	Iterator	it;

	m_elements.push_back(decor);
	it = std::prev(m_elements.end());
	decor->setup(*m_biome);
	return (it);
}

DecorManager::Iterator	DecorManager::erase(Iterator iterator)
{
	if (iterator != m_elements.end())
		delete (*iterator);
	return m_elements.erase(iterator);
}

void	DecorManager::clear()
{
	for (auto element : m_elements)
		delete element;
	m_elements.clear();
}

void	DecorManager::update(sf::Time frameTime, octo::Camera const& camera)
{
	float const			leftLimitX = camera.getCenter().x - camera.getSize().x / 1.8f;
	float const			rightLimitX = camera.getCenter().x + camera.getSize().x / 1.8f;
	float const			upLimitY = camera.getCenter().y - camera.getSize().x / 2.0f;
	float const			downLimitY = camera.getCenter().y + camera.getSize().x / 2.0f;

	float const			leftLimitXbis = camera.getCenter().x - camera.getSize().x;
	float const			rightLimitXbis = camera.getCenter().x + camera.getSize().x;
	float const			upLimitYbis = camera.getCenter().y - camera.getSize().x;
	float const			downLimitYbis = camera.getCenter().y + camera.getSize().x * 2.f;

	sf::Vector2f		position;

	m_builder.clear();

	for (auto element : m_elements)
	{
		position = element->getPosition();

		if (!element->isDisabledIfOutOfScreen())
		{
			element->update(frameTime, m_builder, *m_biome);
		}
		else if ((position.x >= leftLimitXbis && position.x <= rightLimitXbis) &&
			(position.y >= upLimitYbis && position.y <= downLimitYbis))
		{
			if ((position.x >= leftLimitX && position.x <= rightLimitX) &&
				(position.y >= upLimitY && position.y <= downLimitY))
			{
				element->update(frameTime, m_builder, *m_biome);
			}
			else if (!element->dieOutOfScreen())
			{
				element->update(frameTime, m_builder, *m_biome);
			}
		}
	}
	m_used = m_builder.getUsed();
}

void	DecorManager::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	states.transform *= getTransform();
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

void	DecorManager::registerDecors()
{
	m_factory.registerCreator<Crystal>(DecorTypes::Crystal);
	m_factory.registerCreator<Star>(DecorTypes::Star);
	m_factory.registerCreator<Cloud>(DecorTypes::Cloud);
	m_factory.registerCreator<Rock>(DecorTypes::Rock);
	m_factory.registerCreator<Tree>(DecorTypes::Tree);
	m_factory.registerCreator<Sun>(DecorTypes::Sun);
	m_factory.registerCreator<Moon>(DecorTypes::Moon);
	m_factory.registerCreator<Rainbow>(DecorTypes::Rainbow);
	m_factory.registerCreator<Mushroom>(DecorTypes::Mushroom);
	m_factory.registerCreator<GroundRock>(DecorTypes::GroundRock);
	m_factory.registerCreator<Sky>(DecorTypes::Sky);
	m_factory.registerCreator<SunLight>(DecorTypes::SunLight);
	m_factory.registerCreator<Grass>(DecorTypes::Grass);
}
