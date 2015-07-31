/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainSystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:41:14 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 04:32:55 by irabeson         ###   ########.fr       */
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
	m_cameraBottom(0.f),
	m_initialRotation(0.f),
	m_dropPerSeconds(0),
	m_canCreateDrop(true),
	m_margin(0.f)
{
	sf::Vector2f const	DropSize{0.6f, 25.f};

	reset({-DropSize, {0.f, -DropSize.y}, DropSize},
		   sf::Triangles, 1000u);
	setDropSpeed(1024.f);
	setDropPerSecond(20);
}

void	RainSystem::setRainRect(sf::FloatRect const& rainRect)
{
	m_rainRect = rainRect;
}

void	RainSystem::update(sf::Time frameTime)
{
	m_dropTimer += frameTime;
	while (getCapacity() > 0 && m_dropTimer > m_dropInterval)
	{
		createDrop();
		m_dropTimer -= m_dropInterval;
	}
	ParticleSystem::update(frameTime);
}

void	RainSystem::update(sf::Time frameTime, octo::VertexBuilder & builder)
{
	m_dropTimer += frameTime;
	octo::Camera const & camera = octo::Application::getCamera();
	m_cameraBottom = camera.getRectangle().top + camera.getSize().y;
	while (m_canCreateDrop && getCapacity() > 0 && m_dropTimer > m_dropInterval)
	{
		createDrop();
		m_dropTimer -= m_dropInterval;
	}
	ParticleSystem::update(frameTime, builder);
}

void	RainSystem::setDropPerSecond(std::size_t count)
{
	m_dropPerSeconds = count;
	if (count == 0u)
		m_canCreateDrop = false;
	else
	{
		m_dropInterval = sf::seconds(1.f / count);
		m_canCreateDrop = true;
	}
}

void	RainSystem::setDropSpeed(float speed)
{
	sf::Vector2f	normalized;
	float			radRotation = 0.f;

	m_initialVelocity.y = speed;
	normalized = octo::normalized(m_initialVelocity);
	radRotation = std::atan2(normalized.y, normalized.x) - (octo::PiDiv2);
	m_initialRotation = octo::rad2Deg(radRotation);
}

void	RainSystem::setDropAngle(float angle)
{
	float const	angleRad = octo::deg2Rad(angle);

	m_initialVelocity.x = m_initialVelocity.y * std::tan(angleRad);
	m_initialRotation = -angle;
}

void	RainSystem::setMargin(float margin)
{
	m_margin = margin;
}

void	RainSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	std::get<Component::Position>(particle) += std::get<Velocity>(particle) * frameTime.asSeconds();
}

bool	RainSystem::isDeadParticle(Particle const& particle)
{
	static float const	BottomMargin{64.f};
	float				bottom = m_cameraBottom + BottomMargin;

	return (std::get<Component::Position>(particle).y > bottom);
}

void			RainSystem::createDrop()
{
	static float const	TopMargin{64.f};
	sf::Vector2f		pos;

	pos.x = m_rainRect.left - m_margin +
			m_floatDistribution(m_engine) *	(m_rainRect.width + m_margin * 2.f);

	pos.y = m_rainRect.top - TopMargin * m_floatDistribution(m_engine);
	emplace(sf::Color(255, 255, 255), pos, sf::Vector2f(1.f, 1.f), m_initialRotation, m_initialVelocity);
}

