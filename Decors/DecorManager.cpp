/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 03:39:50 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 18:34:34 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DecorManager.hpp"
#include "DecorBuilder.hpp"
#include "ADecor.hpp"

DecorManager::DecorManager(std::size_t maxVertexCount) :
	m_vertices(new sf::Vertex[maxVertexCount]),
	m_count(maxVertexCount),
	m_used(0u)
{
	registerDecors();
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
	Iterator	it;

	m_elements.push_back(decor);
	it = std::prev(m_elements.end());
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

void	DecorManager::update(sf::Time frameTime, ABiome& biome, octo::Camera const& camera)
{
	DecorBuilder	builder(m_vertices.get(), m_count);
	float const		minVisibleX = camera.getCenter().x - camera.getSize().x;
	float const		maxVisibleX = camera.getCenter().x + camera.getSize().x;
	float			elementX = 0.f;

	for (auto element : m_elements)
	{
		elementX = element->getPosition().x;
		if (elementX >= minVisibleX && elementX <= maxVisibleX)
		{
			element->update(frameTime, builder, biome);
		}
	}
	m_used = builder.getUsed();
}

void	DecorManager::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

void	DecorManager::registerDecors()
{

}
