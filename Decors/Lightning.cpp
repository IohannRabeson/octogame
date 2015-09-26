/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lightning.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/12 17:25:02 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/25 17:12:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lightning.hpp"

#include <list>
#include <algorithm>

#include <Math.hpp>
#include <VertexBuilder.hpp>
#include <Interpolations.hpp>

#include <iostream> // TEST

static sf::Vector2f	perpendicular(sf::Vector2f const& v)
{
	return (sf::Vector2f(-v.y, v.x));
}

static sf::Vector2f	rotate(sf::Vector2f d, float radians)
{
	float const	sinValue = std::sin(radians);
	float const	cosValue = std::cos(radians);

	return (sf::Vector2f(d.x * cosValue - d.y * sinValue, d.x * sinValue + d.y * cosValue));
}

//
//	class Lightning
//
Lightning::Lightning(std::size_t maxVertices) :
	m_vertices(new sf::Vertex[maxVertices]),
	m_verticeCount(maxVertices),
	m_builder(m_vertices.get(), m_verticeCount),
	m_defaultMiddleOffsetFactor(150.f),
	m_defaultBranchProbability(0.5f),
	m_defaultBranchMaxAngle(45.f),
	m_defaultBranchMaxLenght(0.7f),
	m_defaultColor(sf::Color::White),
	m_defaultFractalLevel(3u),
	m_defaultCycleTime(sf::seconds(1.f / 3.f)),
	m_currentArc(0u),
	m_triggerTime(sf::seconds(1.f / 6.f))
{
	std::random_device rd;
	m_engine.seed(rd());
}

void	Lightning::update(sf::Time frameTime)
{
	ArcPtr	arc;

	m_currentTime += frameTime;
	if (m_currentTime >= m_triggerTime)
	{
		m_arcs.at(m_currentArc)->restart();
		m_currentArc = (m_currentArc + 1) % m_arcs.size();
		m_currentTime -= m_triggerTime;
	}
	m_builder.clear();
	for (auto& arc : m_arcs)
	{
		if (arc->isExpired() == false)
		{
			arc->update(frameTime);
			arc->buildVertices(m_builder);
		}
	}
}

void	Lightning::update(sf::Time frameTime, octo::VertexBuilder & builder)
{
	ArcPtr	arc;

	m_currentTime += frameTime;
	if (m_currentTime >= m_triggerTime)
	{
		m_arcs.at(m_currentArc)->restart();
		m_currentArc = (m_currentArc + 1) % m_arcs.size();
		m_currentTime -= m_triggerTime;
	}
	m_builder.clear();
	for (auto& arc : m_arcs)
	{
		if (arc->isExpired() == false)
		{
			arc->update(frameTime);
			arc->buildVertices(builder);
		}
	}
}

void	Lightning::draw(sf::RenderTarget& render)const
{
	render.draw(m_vertices.get(), m_builder.getUsed(), sf::Triangles);
}

std::size_t	Lightning::addArc(sf::Vector2f const& p0,
							  sf::Vector2f const& p1,
							  float thickness)
{
	std::size_t	id = m_arcs.size();

	m_arcs.emplace_back(
			std::make_shared<Arc>(m_engine, m_dist, p0, p1, m_defaultColor,
									thickness, m_defaultMiddleOffsetFactor,
									m_defaultBranchProbability,
									m_defaultBranchMaxAngle,
									m_defaultBranchMaxLenght,
									m_defaultFractalLevel, m_defaultCycleTime));
	return (id);
}

void	Lightning::setArc(std::size_t id, sf::Vector2f const& p0, sf::Vector2f const& p1)
{
	m_arcs.at(id)->setSegment(p0, p1);
}

Lightning::Arc&	Lightning::getArc(std::size_t id)
{
	return (*m_arcs.at(id));
}

Lightning::Arc const&	Lightning::getArc(std::size_t id)const
{
	return (*m_arcs.at(id));
}

void	Lightning::removeArc(std::size_t id)
{
	m_arcs.erase(std::next(m_arcs.begin(), id));
}

void	Lightning::clear()
{
	m_arcs.clear();
}

std::size_t		Lightning::getArcCount()const
{
	return (m_arcs.size());
}

//
//	class Lightning::Arc
//
Lightning::Arc::Arc(Engine& engine,
					Distri& dist,
					sf::Vector2f const& p0,
					sf::Vector2f const& p1,
					sf::Color const& color,
					float thickness,
					float middleOffsetFactor,
					float branchProbability,
					float branchAngle,
					float branchLenght,
					std::size_t fractalLevel,
					sf::Time cycleTime) :
	m_engine(engine),
	m_dist(dist),
	m_branchDist(branchProbability),
	m_color(color),
	m_thickness(thickness),
	m_middleOffsetFactor(middleOffsetFactor),
	m_fractalLevel(fractalLevel),
	m_branchAngle(octo::deg2Rad(branchAngle)),
	m_branchLenght(branchLenght),
	m_cycleTime(cycleTime)
{
	m_segments.emplace_back(p0, p1);
	fractalizeSegments();
}
	
void	Lightning::Arc::setBranchProbability(float probability)
{
	m_branchDist.param(std::bernoulli_distribution::param_type(probability));
}

void	Lightning::Arc::setColor(sf::Color const& color)
{
	m_color = color;
}

void	Lightning::Arc::setThickness(float thickness)
{
	m_thickness = thickness;
}

void	Lightning::Arc::setMiddleOffsetFactor(float factor)
{
	m_middleOffsetFactor = factor;
}

void	Lightning::Arc::setFractalLevel(std::size_t level)
{
	m_fractalLevel = level;
}

/*!	Set the maximum angle for a new branch
 *	\param angle Angle in degrees
 */
void	Lightning::Arc::setBranchMaxAngle(float angle)
{
	m_branchAngle = octo::deg2Rad(angle);
}

void	Lightning::Arc::setBranchMaxLenght(float lenght)
{
	m_branchLenght = lenght;
}

void	Lightning::Arc::restart()
{
	m_currentTime = sf::Time::Zero;
	m_color.a = 0;
	fractalizeSegments();
}

void	Lightning::Arc::update(sf::Time frameTime)
{
	if (m_currentTime < m_cycleTime)
	{
		m_currentTime += frameTime;
		updateColor();
	}
}

void	Lightning::Arc::setSegment(sf::Vector2f const& p0, sf::Vector2f const& p1)
{
	m_segments.clear();
	m_segments.emplace_back(p0, p1);
}

bool	Lightning::Arc::isExpired()const
{
	return (m_currentTime >= m_cycleTime);
}

//
//	This method attemps m_segments contains at least one segment and
//	fractalize theses segments.
//
//	This type of algorithm is called "mid point algorithm" because at each iteration
//	he uses the middle point of the current segment.
//	Each segment is cutted in two parts and the point between is moved
//	along the perpendicular, then 2 segments are created in replacement of the first.
//
void	Lightning::Arc::fractalizeSegments()
{
	sf::Vector2f	midPoint;
	sf::Vector2f	start;
	sf::Vector2f	end;
	sf::Vector2f	direction;
	float			offsetFactor = m_middleOffsetFactor;		
	
	m_fractalizedSegments.assign(m_segments.begin(), m_segments.end());
	for (std::size_t i = 0u; i < m_fractalLevel; ++i)
	{
		for (auto it = m_fractalizedSegments.begin(); it != m_fractalizedSegments.end();)
		{
			midPoint = it->middle();
			midPoint += perpendicular(it->direction()) * m_dist(m_engine) * offsetFactor;
			start = it->p0();
			end = it->p1();
			it = m_fractalizedSegments.erase(it);
			it = std::next(m_fractalizedSegments.emplace(it, start, midPoint));
			it = std::next(m_fractalizedSegments.emplace(it, midPoint, end));
			if (m_branchDist(m_engine))
			{
				direction = end - midPoint;
				end = midPoint + rotate(direction, m_dist(m_engine) * m_branchAngle) * m_branchLenght;
				it = std::next(m_fractalizedSegments.emplace(it, midPoint, end));
			}
		}
		offsetFactor /= 2.f;
	}
}


void	Lightning::Arc::updateColor()
{
	sf::Time const	HalfTime = m_cycleTime / 2.f;

	if (m_currentTime < HalfTime)
	{
		m_color.a = std::min<sf::Uint8>(m_currentTime / HalfTime * 255.f, 255.f);
	}
	else
	{
		m_color.a = std::max<float>((1.f - (m_currentTime - HalfTime) / HalfTime) * 255.f, 0.f);
	}
}

void	Lightning::Arc::buildVertices(octo::VertexBuilder& builder)const
{
	for (auto const& segment : m_fractalizedSegments)
	{
		builder.createLine(segment.p0(), segment.p1(), m_thickness, m_color);
	}
}
