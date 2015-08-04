/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DropSystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:41:14 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 04:32:55 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DropSystem.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Math.hpp>

#include <iostream>

DropSystem::DropSystem() :
	m_engine(std::time(0) ^ 0xB38421),
	m_floatDistribution(0.f, 1.f),
	m_cameraBottom(0.f),
	m_initialRotation(0.f),
	m_dropPerSeconds(0),
	m_canCreateDrop(true),
	m_color(255, 255, 255, 200)
{
}

void	DropSystem::setDropRect(sf::FloatRect const& dropRect)
{
	m_dropRect = dropRect;
}

void	DropSystem::setDropColor(sf::Color const & color)
{
	m_color = color;
}

void	DropSystem::setDropSize(sf::Vector2f const & dropSize)
{
	reset({{-dropSize.x, 0.f}, {0.f, -dropSize.y}, {dropSize.x, 0.f}},
		   sf::Triangles, 1000u);
}

void	DropSystem::setDropPerSecond(float count)
{
	m_dropPerSeconds = count;
	if (count == 0.f)
		m_canCreateDrop = false;
	else
	{
		m_dropInterval = sf::seconds(1.f / count);
		m_canCreateDrop = true;
	}
}

void	DropSystem::setDropSpeed(float speed)
{
	sf::Vector2f	normalized;
	float			radRotation = 0.f;

	m_initialVelocity.y = speed;
	normalized = octo::normalized(m_initialVelocity);
	radRotation = std::atan2(normalized.y, normalized.x) - (octo::PiDiv2);
	m_initialRotation = octo::rad2Deg(radRotation);
}

void	DropSystem::setDropAngle(float angle)
{
	float const	angleRad = octo::deg2Rad(angle);

	m_initialVelocity.x = m_initialVelocity.y * std::tan(angleRad);
	m_initialRotation = -angle;
}

void	DropSystem::update(sf::Time frameTime)
{
	m_dropTimer += frameTime;
	while (getCapacity() > 0 && m_dropTimer > m_dropInterval)
	{
		createDrop();
		m_dropTimer -= m_dropInterval;
	}
	ParticleSystem::update(frameTime);
}

void	DropSystem::update(sf::Time frameTime, octo::VertexBuilder & builder)
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

void	DropSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	std::get<Component::Position>(particle) += std::get<Velocity>(particle) * frameTime.asSeconds();
}

bool	DropSystem::isDeadParticle(Particle const& particle)
{
	static float const	BottomMargin{64.f};
	float				bottom = m_cameraBottom + BottomMargin;

	return (std::get<Component::Position>(particle).y > bottom);
}

void			DropSystem::createDrop()
{
	sf::Vector2f		pos;

	pos.x = m_dropRect.left + m_floatDistribution(m_engine) * (m_dropRect.width * 2.f);

	pos.y = m_dropRect.top * m_floatDistribution(m_engine);
	emplace(m_color, pos, sf::Vector2f(1.f, 1.f), m_initialRotation, m_initialVelocity);
}

