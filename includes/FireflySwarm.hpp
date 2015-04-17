/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflySwarm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 17:07:48 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/17 18:08:35 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIREFLYSWARM_HPP
# define FIREFLYSWARM_HPP
# include <Stack.hpp>
# include <SFML/System/Vector2.hpp>
# include <SFML/System/Time.hpp>
# include <SFML/Graphics/Vertex.hpp>
# include <SFML/Graphics/Texture.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include "BSpline.hpp"

# include <random>

class FireflySwarm
{
	struct Firefly;
public:
	enum class SpawnMode
	{
		Stressed,
		Normal,
		Lazy,
		Static
	};

	class AbstractPositionBehavior;
	class StrictPositionBehavior;
	class RectanglePositionBehavior;
	class CirclePositionBehavior;
	class RingPositionBehavior;

	explicit FireflySwarm(std::size_t capacity);

	void				setTexture(sf::Texture const& texture);
	void				setPositionBehavior(AbstractPositionBehavior* behavior);
	void				setTarget(sf::Vector2f const& position);

	std::size_t			create(SpawnMode spawnMode,
							   sf::Vector2f const& position,
							   sf::Color const& color,
							   float radius,
							   float speed);
	
	void				killAll(); 
	void				update(sf::Time frameTime);
	void				draw(sf::RenderTarget& render)const;

	std::size_t			getCount()const;
	std::size_t			getCapacity()const;
private:
	std::size_t			consumeId();
	void				updateFirefly(std::size_t id, sf::Time frameTime);
	void				commitFirefly(std::size_t id);
	sf::Vector2f		getFireflyPosition(std::size_t id)const;
	Firefly&			createFirefly(std::size_t id,
									  sf::Color const& color,
									  float radius,
									  float speed);
	void				setupQuad(std::size_t id, sf::Color const& color);
	void				hideQuad(std::size_t id);
private:
	typedef std::unique_ptr<AbstractPositionBehavior>	PositionBehavior;

	std::unique_ptr<Firefly[]>			m_fireflies;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	octo::Stack<std::size_t>			m_deads;
	sf::Vector2f						m_target;
	std::size_t							m_capacity;
	std::size_t							m_count;
	sf::Texture const*					m_texture;
	PositionBehavior					m_behavior;
};

struct FireflySwarm::Firefly
{
	Firefly();

	BSpline		path;
	float		speed;
	float		radius;
	float		pathPosition;
	bool		alive;
};

class FireflySwarm::AbstractPositionBehavior
{
public:
	virtual ~AbstractPositionBehavior(){}

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target) = 0;
};

class FireflySwarm::StrictPositionBehavior : public AbstractPositionBehavior
{
public:
	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target)
	{
		return (target);
	}
};

class FireflySwarm::RectanglePositionBehavior : public AbstractPositionBehavior
{
	typedef std::uniform_real_distribution<float>	Distribution;
	typedef std::mt19937							Engine;
public:
	RectanglePositionBehavior(std::size_t seed, float halfWidth, float halfHeight) :
		m_engine(seed),
		m_distribution(-1.f, 1.f),
		m_halfSize(halfWidth, halfHeight)
	{
	}

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target)
	{
		sf::Vector2f const	rnd(m_distribution(m_engine) * m_halfSize.x,
								m_distribution(m_engine) * m_halfSize.y);
		
		return (target + rnd);
	}
private:
	Engine			m_engine;
	Distribution	m_distribution;
	sf::Vector2f	m_halfSize;
};

class FireflySwarm::CirclePositionBehavior : public AbstractPositionBehavior
{
	typedef std::uniform_real_distribution<float>	TrigoDistribution;
	typedef std::uniform_real_distribution<float>	RadiusDistribution;
	typedef std::mt19937							Engine;
public:
	CirclePositionBehavior(std::size_t seed, float radius) :
		m_engine(seed),
		m_trigoDistribution(-M_PI, M_PI),
		m_radiusDistribution(0.f, 1.f),
		m_radius(radius)
	{
	}

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target)
	{
		float			angle = m_trigoDistribution(m_engine);
		float			r = m_radiusDistribution(m_engine) * m_radius;
		sf::Vector2f	rnd(std::cos(angle) * r, std::sin(angle) * r);
		
		return (target + rnd);
	}

	void					setRadius(float radius)
	{
		m_radius = radius;
	}
private:
	Engine				m_engine;
	TrigoDistribution	m_trigoDistribution;
	RadiusDistribution	m_radiusDistribution;
	float				m_radius;
};

class FireflySwarm::RingPositionBehavior : public AbstractPositionBehavior
{
	typedef std::uniform_real_distribution<float>	TrigoDistribution;
	typedef std::uniform_real_distribution<float>	RadiusDistribution;
	typedef std::mt19937							Engine;
public:
	RingPositionBehavior(std::size_t seed, float radius, float internal) :
		m_engine(seed),
		m_trigoDistribution(-M_PI, M_PI),
		m_radiusDistribution(0.f, 1.f),
		m_radius(radius),
		m_internalRadius(internal)
	{
	}

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target)
	{
		float			angle = m_trigoDistribution(m_engine);
		float			r = m_internalRadius + (m_radiusDistribution(m_engine) * (m_radius - m_internalRadius));
		sf::Vector2f	rnd(std::cos(angle) * r, std::sin(angle) * r);
		
		return (target + rnd);
	}

	void					setRadius(float radius)
	{
		m_radius = radius;
	}

private:
	Engine				m_engine;
	TrigoDistribution	m_trigoDistribution;
	RadiusDistribution	m_radiusDistribution;
	float				m_radius;
	float				m_internalRadius;
};

#endif
