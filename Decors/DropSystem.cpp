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

DropSystem::DropSystem() :
	m_engine(std::time(0) ^ 0xB38421),
	m_floatDistribution(0.f, 1.f),
	m_maxDropCount(80u),
	m_speed(0.f),
	m_color(255, 255, 255, 200),
	m_dropPerSeconds(0),
	m_canCreateDrop(true)
{
	m_particles.resize(m_maxDropCount);
}

void	DropSystem::setDropRect(sf::FloatRect const& dropRect)
{
	m_dropRect = dropRect;
}

void	DropSystem::setDrop(sf::Vector2f const & dropSize, float speed, sf::Color const & color, ABiome & biome)
{
	m_waterLevel = biome.getWaterLevel();
	for (auto &particle : m_particles)
		particle.isAlive = false;
	m_size = dropSize;
	m_speed = speed;
	m_color = color;
}

void	DropSystem::setDropPerSecond(float count)
{
	m_dropPerSeconds = count;
	if (count <= 0.f)
		m_canCreateDrop = false;
	else
	{
		m_dropInterval = sf::seconds(1.f / count);
		m_canCreateDrop = true;
	}
}

void	DropSystem::update(sf::Time frameTime, float angle, octo::VertexBuilder & builder)
{
	static float const		BottomMargin{64.f};
	octo::Camera const &	camera = octo::Application::getCamera();
	float					cameraBottom = camera.getRectangle().top + camera.getSize().y;
	float					bottom = cameraBottom + BottomMargin;

	m_dropTimer += frameTime;
	float velocityY = m_speed * frameTime.asSeconds();
	for (auto &particle : m_particles)
	{
		if (particle.isAlive)
		{
			particle.position.x += particle.velocity.x * frameTime.asSeconds();
			particle.position.y += velocityY;
			builder.createTriangle(particle.p1 + particle.position, particle.p2 + particle.position, particle.p3 + particle.position, m_color);
		}
		else if (particle.isAlive == false && m_canCreateDrop && m_dropTimer > m_dropInterval)
		{
			createDrop(particle, angle);
			m_dropTimer -= m_dropInterval;
		}

		if (isDeadParticle(particle, bottom))
			particle.isAlive = false;
	}
}

bool	DropSystem::isDeadParticle(Particle const& particle, float bottom)
{
	if (particle.position.y > m_waterLevel)
		return true;
	else if (particle.position.y > bottom)
		return true;
	return false;
}

void			DropSystem::createDrop(Particle & particle, float angle)
{
	particle.p1 = sf::Vector2f(-m_size.x, 0.f);
	particle.p2 = sf::Vector2f(m_size.x, 0.f);
	particle.p3 = sf::Vector2f(0.f, -m_size.y);
	float radAngle = angle * octo::Deg2Rad;
	float cosAngle = std::cos(-radAngle);
	float sinAngle = std::sin(-radAngle);
	octo::rotateVector(particle.p1, cosAngle, sinAngle);
	octo::rotateVector(particle.p2, cosAngle, sinAngle);
	octo::rotateVector(particle.p3, cosAngle, sinAngle);

	particle.position.x = m_dropRect.left + m_floatDistribution(m_engine) * m_dropRect.width;
	particle.position.y = m_dropRect.top + m_floatDistribution(m_engine) * m_dropRect.height;
	particle.velocity.y = m_speed;
	particle.velocity.x = particle.velocity.y * std::tan(radAngle);
	particle.isAlive = true;
}

