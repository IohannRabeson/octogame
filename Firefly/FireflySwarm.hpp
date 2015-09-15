/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflySwarm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 17:07:48 by irabeson          #+#    #+#             */
/*   Updated: 2015/09/14 16:11:54 by jbalestr         ###   ########.fr       */
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
# include <memory>
# include <functional>

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
	class AbstractPopulation;
	class UniquePopulation;
	class UniformPopulation;
	class NormalPopulation;

	explicit FireflySwarm(std::size_t capacity);

	void				setTexture(sf::Texture const& texture);
	void				setPositionBehavior(AbstractPositionBehavior* behavior);
	void				setTarget(sf::Vector2f const& position);

	std::size_t			create(SpawnMode spawnMode,
							   sf::Vector2f const& position,
							   sf::Color const& color,
							   float radius,
							   float haloRadius,
							   float speed);

	std::size_t			create(SpawnMode spawnMode,
							   sf::Vector2f const& position,
							   AbstractPopulation& population);

	void				create(SpawnMode spawnMode,
							   sf::Vector2f const& position,
							   AbstractPopulation& population,
							   std::size_t count);

	void				kill(std::size_t id);	
	void				killAll(); 
	void				update(sf::Time frameTime);
	void				draw(sf::RenderTarget& render)const;

	std::size_t			getCount()const;
	std::size_t			getCapacity()const;
	Firefly&			getFirefly(std::size_t id);
	Firefly const&		getFirefly(std::size_t id)const;
	sf::Vector2f const &	getPositionById(std::size_t id) const;
	sf::Vector2f const &	getTarget(void) const;

private:
	std::size_t			consumeId();
	void				commitFirefly(std::size_t id, Firefly & fly);
	void				spawnFirefly(Firefly& fly,
									 SpawnMode spawnMode,
									 sf::Vector2f const& position);

	Firefly&			createFirefly(std::size_t id,
									  sf::Color const& color,
									  float radius,
									  float haloRadius,
									  float speed);

	Firefly&			createFirefly(std::size_t id, AbstractPopulation& population);

	void				killFirefly(std::size_t id, Firefly& fly);
	void				setupQuad(std::size_t id, Firefly& fly);
	void				destroyQuad(std::size_t id);
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

	BSpline			path;
	sf::Color		color;
	float			speed;
	float			radius;
	float			haloRadius;
	float			pathPosition;
	sf::Time		time;
	sf::Time		maxTime;
	sf::Vector2f	position;
	bool			alive;
};

#endif
