/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyPositionBehaviors.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/17 19:29:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/17 19:35:32 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireflyPositionBehaviors.hpp"

//
//	StrictPositionBehavior
//
sf::Vector2f	FireflySwarm::StrictPositionBehavior::getPathPosition(sf::Vector2f const& target)
{
	return (target);
}

//
//	RectanglePositionBehavior
//
FireflySwarm::RectanglePositionBehavior::RectanglePositionBehavior(std::size_t seed, float halfWidth, float halfHeight) :
	m_engine(seed),
	m_distribution(-1.f, 1.f),
	m_halfSize(halfWidth, halfHeight)
{
}

sf::Vector2f	FireflySwarm::RectanglePositionBehavior::getPathPosition(sf::Vector2f const& target)
{
	sf::Vector2f const	rnd(m_distribution(m_engine) * m_halfSize.x,
							m_distribution(m_engine) * m_halfSize.y);
	
	return (target + rnd);
}

void	FireflySwarm::RectanglePositionBehavior::setHalfSize(sf::Vector2f const& halfSize)
{
	m_halfSize = halfSize;
}

sf::Vector2f const&		FireflySwarm::RectanglePositionBehavior::getHalfSize()const
{
	return (m_halfSize);
}

//
//	CirclePositionBehavior
//
FireflySwarm::CirclePositionBehavior::CirclePositionBehavior(std::size_t seed, float radius) :
	m_engine(seed),
	m_trigoDistribution(-M_PI, M_PI),
	m_radiusDistribution(0.f, 1.f),
	m_radius(radius)
{
}

sf::Vector2f	FireflySwarm::CirclePositionBehavior::getPathPosition(sf::Vector2f const& target)
{
	float			angle = m_trigoDistribution(m_engine);
	float			r = m_radiusDistribution(m_engine) * m_radius;
	sf::Vector2f	rnd(std::cos(angle) * r, std::sin(angle) * r);
	
	return (target + rnd);
}

void	FireflySwarm::CirclePositionBehavior::setRadius(float radius)
{
	m_radius = radius;
}

float	FireflySwarm::CirclePositionBehavior::getRadius()const
{
	return (m_radius);
}
