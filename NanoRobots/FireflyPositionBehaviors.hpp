/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyPositionBehaviors.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/17 19:27:36 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/17 19:33:45 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIREFLYPOSITIONBEHAVIORS_HPP
# define FIREFLYPOSITIONBEHAVIORS_HPP
# include "FireflySwarm.hpp"

class FireflySwarm::AbstractPositionBehavior
{
public:
	virtual ~AbstractPositionBehavior(){}

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target) = 0;
};

class FireflySwarm::StrictPositionBehavior : public AbstractPositionBehavior
{
public:
	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target);
};

class FireflySwarm::RectanglePositionBehavior : public AbstractPositionBehavior
{
	typedef std::uniform_real_distribution<float>	Distribution;
	typedef std::mt19937							Engine;
public:
	RectanglePositionBehavior(std::size_t seed, float halfWidth, float halfHeight);

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target);
	void					setHalfSize(sf::Vector2f const& halfSize);
	sf::Vector2f const&		getHalfSize()const;
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
	CirclePositionBehavior(std::size_t seed, float radius);

	virtual sf::Vector2f	getPathPosition(sf::Vector2f const& target);

	void					setRadius(float radius);
	float					getRadius()const;
private:
	Engine				m_engine;
	TrigoDistribution	m_trigoDistribution;
	RadiusDistribution	m_radiusDistribution;
	float				m_radius;
};

#endif
