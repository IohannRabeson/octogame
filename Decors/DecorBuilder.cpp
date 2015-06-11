/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorBuilder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 05:35:34 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 17:34:02 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DecorBuilder.hpp"
#include <stdexcept>

DecorBuilder::DecorBuilder(sf::Vertex* vertices, std::size_t count) :
	m_vertices(vertices),
	m_size(count),
	m_used(0u)
{
}

void	DecorBuilder::createTriangle(sf::Vector2f const& p0,
								     sf::Vector2f const& p1,
								     sf::Vector2f const& p2,
								     sf::Color const& color)
{
	if (m_used + 3 > m_size)
		throw std::runtime_error("decor builder: no more vertices");
	sf::Vertex* const	vertices = m_vertices + m_used;

	vertices[0].position = p0;
	vertices[1].position = p1;
	vertices[2].position = p2;
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	m_used += 3;
}

void	DecorBuilder::createQuad(sf::Vector2f const& p0,
								 sf::Vector2f const& p1,
								 sf::Vector2f const& p2,
								 sf::Vector2f const& p3,
								 sf::Color const& color)
{
	createTriangle(p0, p1, p2, color);
	createTriangle(p0, p3, p2, color);
	static_cast<void>(p3);
}

std::size_t DecorBuilder::getUsed()const
{
	return (m_used);
}
