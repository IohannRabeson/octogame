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
#include "Rainbow.hpp"

#include <VertexBuilder.hpp>

#include <cassert>

DecorManager::DecorManager(std::size_t maxVertexCount) :
	m_vertices(new sf::Vertex[maxVertexCount]),
	m_count(maxVertexCount),
	m_used(0u),
	m_biome(nullptr)
{
	registerDecors();
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
	octo::VertexBuilder	builder(m_vertices.get(), m_count);
	float const			minVisibleX = camera.getCenter().x - camera.getSize().x;
	float const			maxVisibleX = camera.getCenter().x + camera.getSize().x;
	float				elementX = 0.f;

	for (auto element : m_elements)
	{
		elementX = element->getPosition().x;
		if (element->isDisabledIfOutOfScreen() == false ||
			(elementX >= minVisibleX && elementX <= maxVisibleX))
		{
			element->update(frameTime, builder, *m_biome);
		}
	}
	m_used = builder.getUsed();
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
	m_factory.registerCreator<Rainbow>(DecorTypes::Rainbow);
}
