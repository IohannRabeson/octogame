/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflySwarm.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 17:09:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/09/14 16:12:10 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireflySwarm.hpp"
#include "FireflyPositionBehaviors.hpp"
#include "FireflyPopulation.hpp"
#include <Interpolations.hpp>

FireflySwarm::Firefly::Firefly() :
	path(0),
	speed(0.f),
	radius(0.f),
	haloRadius(0.f),
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

/*!
 *	Create a new firefly
 *	\param	spawnMode How the firefly is spawned
 *	\param	the spawn position
 *	\param	color initial color
 *	\param radius Radius of the particle
 *	\param haloRadius Radius of the hola of the particle
 *	\param initial speed
 */
std::size_t		FireflySwarm::create(SpawnMode spawnMode,
									 sf::Vector2f const& position,
								   	 sf::Color const& color,
								   	 float radius,
									 float haloRadius,
								   	 float speed)
{
	std::size_t	newId = consumeId();
	Firefly&	fly = createFirefly(newId, color, radius, haloRadius, speed);

	spawnFirefly(fly, spawnMode, position);
	commitFirefly(newId, fly);
	return (newId);
}

std::size_t	FireflySwarm::create(SpawnMode spawnMode,
						   		 sf::Vector2f const& position,
						   		 AbstractPopulation& population)
{
	std::size_t	newId = consumeId();
	Firefly&	fly = createFirefly(newId, population);

	spawnFirefly(fly, spawnMode, position);
	commitFirefly(newId, fly);
	return (newId);
}

void	FireflySwarm::kill(std::size_t id)
{
	Firefly&	fly = getFirefly(id);

	killFirefly(id, fly);
}

void	FireflySwarm::killAll()
{
	m_count = 0;
	m_deads.clear();
}

void	FireflySwarm::update(sf::Time frameTime)
{
	for (std::size_t i = 0u; i < m_count; ++i)
	{
		Firefly&	fly = m_fireflies[i];

		if (fly.alive)
		{
			assert(fly.path.size() < 5);
			fly.time += frameTime;
			if (fly.maxTime != sf::Time::Zero && fly.time >= fly.maxTime)
			{
				killFirefly(i, fly);
			}
			else
			{
				fly.pathPosition += fly.speed * frameTime.asSeconds();
				if (fly.pathPosition > 1.f)
				{
					fly.path.pushBack(m_behavior->getPathPosition(m_target));
					fly.path.popFront();
					fly.pathPosition -= 1.f;
				}
				commitFirefly(i, fly);
			}
		}
	}
}

void	FireflySwarm::draw(sf::RenderTarget& render)const
{
	render.draw(m_vertices.get(), m_count * 8u, sf::Quads, m_texture);
}

std::size_t		FireflySwarm::getCount()const
{
	return (m_count - m_deads.size());
}

std::size_t		FireflySwarm::getCapacity()const
{
	return (m_capacity);
}

sf::Vector2f const &	FireflySwarm::getPositionById(std::size_t id) const
{
	return getFirefly(id).position;
}

sf::Vector2f const &	FireflySwarm::getTarget(void) const
{
	return m_target;
}

std::size_t		FireflySwarm::consumeId()
{
	std::size_t	newId = m_count;

	if (m_deads.size() > 0)
	{
		newId = m_deads.top();
		m_deads.pop();
	}
	else
	{
		++m_count;
	}
	if (newId >= m_capacity)
	{
		throw std::runtime_error("firefly swarm: not enough firefly reserved");
	}
	return (newId);
}

void	FireflySwarm::spawnFirefly(Firefly& fly, SpawnMode spawnMode, sf::Vector2f const& position)
{
	switch (spawnMode)
	{
		case SpawnMode::Stressed:
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			fly.path.pushBack(m_behavior->getPathPosition(m_target));
			fly.path.pushBack(m_behavior->getPathPosition(m_target));
			break;
		case SpawnMode::Normal:
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			fly.path.pushBack(m_behavior->getPathPosition(position));
			break;
		case SpawnMode::Lazy:
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			fly.path.pushBack(m_behavior->getPathPosition(position));
			fly.path.pushBack(m_behavior->getPathPosition(position));
			break;
		case SpawnMode::Static:
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			fly.path.pushBack(position);
			break;
		default:
			break;
	}
}

void	FireflySwarm::commitFirefly(std::size_t id, Firefly & fly)
{
	static sf::Vector2f const	TopLeft = {-1.f, -1.f};
	static sf::Vector2f const	TopRight = {1.f, -1.f};
	static sf::Vector2f const	BottomRight = {1.f, 1.f};
	static sf::Vector2f const	BottomLeft = {-1.f, 1.f};
	std::size_t	const			Offset = id * 8u;
	sf::Color					haloColor = fly.color;
	fly.position = fly.path.compute(fly.pathPosition);

	// Quad du centre
	m_vertices[Offset + 0].position = fly.position + TopLeft * fly.radius;
	m_vertices[Offset + 1].position = fly.position + TopRight * fly.radius;
	m_vertices[Offset + 2].position = fly.position + BottomRight * fly.radius;
	m_vertices[Offset + 3].position = fly.position + BottomLeft * fly.radius;
	// Quad du halo
	m_vertices[Offset + 4].position = fly.position + TopLeft * fly.haloRadius;
	m_vertices[Offset + 5].position = fly.position + TopRight * fly.haloRadius;
	m_vertices[Offset + 6].position = fly.position + BottomRight * fly.haloRadius;
	m_vertices[Offset + 7].position = fly.position + BottomLeft * fly.haloRadius;
	// Couleurs
	haloColor.a = 100;
	m_vertices[Offset + 0].color = fly.color;
	m_vertices[Offset + 1].color = fly.color;
	m_vertices[Offset + 2].color = fly.color;
	m_vertices[Offset + 3].color = fly.color;
	m_vertices[Offset + 4].color = haloColor;
	m_vertices[Offset + 5].color = haloColor;
	m_vertices[Offset + 6].color = haloColor;
	m_vertices[Offset + 7].color = haloColor;
}

FireflySwarm::Firefly&	FireflySwarm::createFirefly(std::size_t id,
													sf::Color const& color,
													float radius,
													float haloRadius,
													float speed)
{
	Firefly&	fly = getFirefly(id);

	fly.path.clear();
	fly.color = color;
	fly.speed = speed;
	fly.radius = radius;
	fly.haloRadius = haloRadius;
	fly.maxTime = sf::Time::Zero;
	fly.alive = true;
	fly.pathPosition = 0.f;
	fly.time = sf::Time::Zero;
	setupQuad(id, fly);
	return (fly);
}

FireflySwarm::Firefly&	FireflySwarm::createFirefly(std::size_t id, AbstractPopulation& population)
{
	Firefly&	fly = getFirefly(id);

	fly.path.clear();
	population.setupFirefly(fly);
	fly.alive = true;
	fly.pathPosition = 0.f;
	fly.time = sf::Time::Zero;
	setupQuad(id, fly);
	return (fly);
}

void	FireflySwarm::killFirefly(std::size_t id, Firefly& fly)
{
	if (fly.alive)
	{
		destroyQuad(id);
		fly.alive = false;
		m_deads.push(id);
		if (m_deads.size() == m_capacity)
		{
			m_count = 0;
			m_deads.clear();
		}
	}
}

FireflySwarm::Firefly&	FireflySwarm::getFirefly(std::size_t id)
{
	assert( id < m_count );
	return (m_fireflies[id]);
}

FireflySwarm::Firefly const&	FireflySwarm::getFirefly(std::size_t id)const
{
	assert( id < m_count );
	return (m_fireflies[id]);
}

void	FireflySwarm::setupQuad(std::size_t id, Firefly& fly)
{
	std::size_t	const	Offset = id * 8u;
	sf::Vector2f		texSize(m_texture->getSize());
	// TODO: ajouter un truc pour controler la couleur du halo
	sf::Color			haloColor = fly.color;

	haloColor.a = 100;
	m_vertices[Offset + 0].color = fly.color;
	m_vertices[Offset + 1].color = fly.color;
	m_vertices[Offset + 2].color = fly.color;
	m_vertices[Offset + 3].color = fly.color;
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

void	FireflySwarm::destroyQuad(std::size_t id)
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
