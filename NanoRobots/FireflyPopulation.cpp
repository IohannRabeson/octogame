/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyPopulation.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 13:32:32 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/04 02:50:47 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireflyPopulation.hpp"

//
//	Abstract population
//
FireflySwarm::AbstractPopulation::~AbstractPopulation()
{
}

//
//	Unique population
//
void	FireflySwarm::UniquePopulation::setupFirefly(FireflySwarm::Firefly& fly)
{
	fly.color = m_color;
	fly.speed = m_speed;
	fly.radius = m_radius;
	fly.haloRadius = m_haloRadius;
	fly.maxTime = m_maxTime;
}

//
//	Uniform population
//
FireflySwarm::UniformPopulation::UniformPopulation(std::size_t seed,
												  octo::IColorProvider const* colors,
												  float minSpeed,
												  float maxSpeed,
												  float minRadius,
												  float maxRadius,
												  float minHaloRadius,
												  float maxHaloRadius,
												  sf::Time minMaxTime,
												  sf::Time maxMaxTime) :
	m_engine(seed),
	m_distribution(-1.f, 1.f),
	m_colors(colors),
	m_minSpeed(minSpeed),
	m_maxSpeed(maxSpeed),
	m_minRadius(minRadius),
	m_maxRadius(maxRadius),
	m_minHaloRadius(minHaloRadius),
	m_maxHaloRadius(maxHaloRadius),
	m_minMaxTime(minMaxTime),
	m_maxMaxTime(maxMaxTime)
{
}

void	FireflySwarm::UniformPopulation::setupFirefly(FireflySwarm::Firefly& fly)
{
	fly.color = m_colors->getColor(m_engine() % m_colors->getColorCount());
	fly.speed = getRandom(m_minSpeed, m_maxSpeed);
	fly.radius = getRandom(m_minRadius, m_maxRadius);
	fly.haloRadius = getRandom(m_minHaloRadius, m_maxHaloRadius);
	fly.maxTime = getRandom(m_minMaxTime, m_maxMaxTime);
}

void	FireflySwarm::UniformPopulation::setColors(octo::IColorProvider const* colors)
{
	m_colors = colors;
}

void				FireflySwarm::UniformPopulation::setMinSpeed(float value)
{
	m_minSpeed = value;
}

float				FireflySwarm::UniformPopulation::getMinSpeed()const
{
	return (m_minSpeed);
}

void				FireflySwarm::UniformPopulation::setMaxSpeed(float value)
{
	m_maxSpeed = value;
}

float				FireflySwarm::UniformPopulation::getMaxSpeed()const
{
	return (m_maxSpeed);
}

void				FireflySwarm::UniformPopulation::setMinRadius(float value)
{
	m_minRadius = value;
}

float				FireflySwarm::UniformPopulation::getMinRadius()const
{
	return (m_minRadius);
}

void				FireflySwarm::UniformPopulation::setMaxRadius(float value)
{
	m_maxRadius = value;
}

float				FireflySwarm::UniformPopulation::getMaxRadius()const
{
	return (m_maxRadius);
}

void	FireflySwarm::UniformPopulation::setMinHaloRadius(float value)
{
	m_minHaloRadius = value;
}

float	FireflySwarm::UniformPopulation::getMinHaloRadius()const
{
	return (m_minHaloRadius);
}

void	FireflySwarm::UniformPopulation::setMaxHaloRadius(float value)
{
	m_maxHaloRadius = value;
}

float	FireflySwarm::UniformPopulation::getMaxHaloRadius()const
{
	return (m_maxHaloRadius);
}
