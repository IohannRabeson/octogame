/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainSystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:41:14 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/20 05:55:10 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RainSystem.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Math.hpp>

#include <iostream>

RainSystem::RainSystem() :
	m_engine(std::time(0) ^ 0xB38421),
	m_floatDistribution(0.f, 1.f),
	m_dropChanceDistribution(0, 100),
	m_cameraOffset(50.f, 50.f), 
	m_initialRotation(0.f),
	m_dropChance(0),
	m_dropCountFactor(0)
{
	sf::Vector2f const	DropSize{0.6f, 25.f};

	reset({-DropSize, {DropSize.x, -DropSize.y}, DropSize, {-DropSize.x, DropSize.y}},
		   sf::Quads, 1000u);
	setDropChance(50);
	setDropCountFactor(16);
	setDropVelocity(sf::Vector2f(0.f, 2048.f));
}

void	RainSystem::setCameraRect(sf::FloatRect const& cameraRect)
{
	m_cameraRect = cameraRect;
}

void	RainSystem::update(sf::Time frameTime)
{
	for (auto i = 0u; i < m_dropCountFactor; ++i)
	{
		if (m_dropChanceDistribution(m_engine) < m_dropChance)
			createDrop();
	}
	ParticleSystem::update(frameTime);
}

void	RainSystem::setDropCountFactor(unsigned int factor)
{
	m_dropCountFactor = factor;
}

void	RainSystem::setDropChance(unsigned int chance)
{
	m_dropChance = chance;
}

void	RainSystem::setDropVelocity(sf::Vector2f const& velocity)
{
	sf::Vector2f	normalized = octo::normalized(velocity);

	m_initialVelocity = velocity;
	m_initialRotation = std::atan2(normalized.y, normalized.x) * (180.f / M_PI) - 90;
	std::cout << m_initialRotation << std::endl;
}

void	RainSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	std::get<Component::Position>(particle) += m_initialVelocity * frameTime.asSeconds();
}

bool	RainSystem::isDeadParticle(Particle const& particle)
{
	float				bottom = m_cameraRect.top + m_cameraRect.height + m_cameraOffset.y;

	return (std::get<Component::Position>(particle).y > bottom);
}

void			RainSystem::createDrop()
{
	sf::Vector2f		pos;

	pos.x = m_cameraRect.left - m_cameraOffset.x + m_floatDistribution(m_engine) * (m_cameraRect.width + m_cameraOffset.x);
	pos.y = m_cameraRect.top - m_cameraOffset.y * m_floatDistribution(m_engine);
	emplace(sf::Color(180, 180, 180), pos, m_initialRotation);
}
