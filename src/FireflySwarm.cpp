/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflySwarm.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 17:09:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/17 01:36:37 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireflySwarm.hpp"
#include <Interpolations.hpp>

FireflySwarm::Firefly::Firefly() :
	path(7),
	speed(0.f),
	radius(0.f),
	pathPosition(0.f),
	alive(false)
{
}

FireflySwarm::FireflySwarm(std::size_t capacity) :
	m_fireflies(new Firefly[capacity]),
	m_vertices(new sf::Vertex[capacity * 8u]),
	m_deads(capacity),
	m_capacity(capacity),
	m_count(0u),
	m_texture(nullptr),
	m_behavior(nullptr)
{
}

void	FireflySwarm::setPositionBehavior(AbstractPositionBehavior* behavior)
{
	m_behavior.reset(behavior);
}

void	FireflySwarm::setTexture(sf::Texture const& texture)
{
	m_texture = &texture;
}

void	FireflySwarm::setTarget(sf::Vector2f const& position)
{
	m_target = position;
}

std::size_t		FireflySwarm::create(sf::Vector2f const& position,
								   	 sf::Color const& color,
								   	 float radius,
								   	 float speed)
{
	std::size_t	newId = consumeId();

	createFirefly(newId, position, color, radius, speed);
	return (newId);
}

void	FireflySwarm::killAll()
{
	m_count = 0;
}

void	FireflySwarm::update(sf::Time frameTime)
{
	for (std::size_t i = 0u; i < m_count; ++i)
	{
		if (m_fireflies[i].alive)
		{
			updateFirefly(i, frameTime);
		}
	}
}

void	FireflySwarm::draw(sf::RenderTarget& render)const
{
	render.draw(m_vertices.get(), m_count * 8u, sf::Quads, m_texture);
}


std::size_t		FireflySwarm::getCount()const
{
	return (m_count);
}

std::size_t		FireflySwarm::getCapacity()const
{
	return (m_capacity);
}

std::size_t		FireflySwarm::consumeId()
{
	std::size_t	newId = m_count;

	if (newId >= m_capacity)
	{
		std::runtime_error("firefly swarm: not enough firefly reserved");
	}
	++m_count;
	return (newId);
}

void	FireflySwarm::updateFirefly(std::size_t id, sf::Time frameTime)
{
	Firefly&		fly = m_fireflies[id];
	int				newIntegerPart = 0;
	int				oldIntegerPart = 0;
	sf::Vector2f	newPos;

	oldIntegerPart = static_cast<int>(fly.pathPosition);
	fly.pathPosition += fly.speed * frameTime.asSeconds();
	newIntegerPart = static_cast<int>(fly.pathPosition);
	if (newIntegerPart > oldIntegerPart && static_cast<std::size_t>(newIntegerPart + 4) > fly.path.size())
	{
		fly.path.pushBack(m_behavior->getPathPosition(m_target));
		fly.path.popFront();
		fly.pathPosition -= 1.f;
	}
	commitFirefly(id);
}

void	FireflySwarm::commitFirefly(std::size_t id)
{
	static sf::Vector2f const	TopLeft = {-1.f, -1.f};
	static sf::Vector2f const	TopRight = {1.f, -1.f};
	static sf::Vector2f const	BottomRight = {1.f, 1.f};
	static sf::Vector2f const	BottomLeft = {-1.f, 1.f};
	sf::Vector2f const			position = getFireflyPosition(id);
	float const					radius = m_fireflies[id].radius;
	std::size_t	const			Offset = id * 8u;

	m_vertices[Offset + 0].position = position + TopLeft * radius;
	m_vertices[Offset + 1].position = position + TopRight * radius;
	m_vertices[Offset + 2].position = position + BottomRight * radius;
	m_vertices[Offset + 3].position = position + BottomLeft * radius;
	m_vertices[Offset + 4].position = position + TopLeft * radius * 4.f;
	m_vertices[Offset + 5].position = position + TopRight * radius * 4.f;
	m_vertices[Offset + 6].position = position + BottomRight * radius * 4.f;
	m_vertices[Offset + 7].position = position + BottomLeft * radius * 4.f;
}

sf::Vector2f	FireflySwarm::getFireflyPosition(std::size_t id)const
{
	Firefly const&	fly = m_fireflies[id];

	return (fly.path.compute(fly.pathPosition));
}

void	FireflySwarm::createFirefly(std::size_t id, sf::Vector2f const& position,
						   		 	sf::Color const& color,	float radius, float speed)
{
	Firefly&	fly = m_fireflies[id];

	fly.path.pushBack(m_behavior->getPathPosition(position));
	fly.speed = speed;
	fly.radius = radius;
	fly.alive = true;
	fly.pathPosition = 4.f;
	setupQuad(id, color);
}

void	FireflySwarm::setupQuad(std::size_t id, sf::Color const& color)
{
	std::size_t	const	Offset = id * 8u;
	sf::Vector2f		texSize(m_texture->getSize());
	sf::Color			haloColor = color;

	haloColor.a = 100;
	m_vertices[Offset + 0].color = color;
	m_vertices[Offset + 1].color = color;
	m_vertices[Offset + 2].color = color;
	m_vertices[Offset + 3].color = color;
	m_vertices[Offset + 4].color = haloColor;
	m_vertices[Offset + 5].color = haloColor;
	m_vertices[Offset + 6].color = haloColor;
	m_vertices[Offset + 7].color = haloColor;
	m_vertices[Offset + 0].texCoords = sf::Vector2f(0.f, 0.f);
	m_vertices[Offset + 1].texCoords = sf::Vector2f(texSize.x, 0.f);
	m_vertices[Offset + 2].texCoords = sf::Vector2f(texSize.x, texSize.y);
	m_vertices[Offset + 3].texCoords = sf::Vector2f(0.f, texSize.y);
	m_vertices[Offset + 4].texCoords = sf::Vector2f(0.f, 0.f);
	m_vertices[Offset + 5].texCoords = sf::Vector2f(texSize.x, 0.f);
	m_vertices[Offset + 6].texCoords = sf::Vector2f(texSize.x, texSize.y);
	m_vertices[Offset + 7].texCoords = sf::Vector2f(0.f, texSize.y);
}

void	FireflySwarm::hideQuad(std::size_t id)
{
	std::size_t	const	Offset = id * 8u;

	m_vertices[Offset + 0].position = sf::Vector2f();
	m_vertices[Offset + 1].position = sf::Vector2f();
	m_vertices[Offset + 2].position = sf::Vector2f();
	m_vertices[Offset + 3].position = sf::Vector2f();
	m_vertices[Offset + 4].position = sf::Vector2f();
	m_vertices[Offset + 5].position = sf::Vector2f();
	m_vertices[Offset + 6].position = sf::Vector2f();
	m_vertices[Offset + 7].position = sf::Vector2f();
}
