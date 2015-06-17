/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lightning.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/12 17:25:02 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/15 16:07:11 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lightning.hpp"
#include "DecorBuilder.hpp"

#include <list>

static sf::Vector2f	perpendicular(sf::Vector2f const& v)
{
	return (sf::Vector2f(-v.y, v.x));
}

void	Lightning::setup(ABiome& biome)
{
	(void)biome;
	Segment				newP0;
	Segment				newP1;
	sf::Vector2f		midPoint;
	std::list<Segment>	segments;
	
	segments.emplace_back(getPosition(), sf::Vector2f(300, 1000));
	fractalizeSegments(segments, 6);
	m_segments = std::move(segments);
}

void	Lightning::update(sf::Time frameTime, DecorBuilder& builder, ABiome& biome)
{
	for (auto const& segment : m_segments)
	{
		createSegment(segment, builder);
	}
	(void)frameTime;
	(void)builder;
	(void)biome;
}

void	Lightning::createSegment(Segment const& segment, DecorBuilder& builder)const
{
	sf::Vector2f	p = perpendicular(segment.direction());

	builder.createQuad(segment.p0() - p, segment.p0() + p, segment.p1() + p, segment.p1() - p, sf::Color::Red);	
}

void	Lightning::fractalizeSegments(std::list<Segment>& segments, std::size_t count)
{
	sf::Vector2f							midPoint;
	float									offsetFactor = 200.f;
	sf::Vector2f							start;
	sf::Vector2f							end;

	for (std::size_t i = 0u; i < count; ++i)
	{
		for (auto it = segments.begin(); it != segments.end();)
		{
			midPoint = it->middle();
			midPoint += perpendicular(it->direction()) * m_dist(m_engine) * offsetFactor;
			start = it->p0();
			end = it->p1();
			it = segments.erase(it);
			it = std::next(segments.emplace(it, start, midPoint));
			it = std::next(segments.emplace(it, midPoint, end));
		}
		offsetFactor /= 2.f;
	}
}
