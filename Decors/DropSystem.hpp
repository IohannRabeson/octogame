/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DropSystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:32:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 03:45:24 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DROPSYSTEM_HPP
# define DROPSYSTEM_HPP

# include "ABiome.hpp"
# include <ParticleSystem.hpp>

# include <random>

class DropSystem
{
	struct Particle
	{
		sf::Vector2f	p1;
		sf::Vector2f	p2;
		sf::Vector2f	p3;
		sf::Vector2f	velocity;
		sf::Vector2f	position;
		bool			isAlive;
	};
public:
	DropSystem();
	~DropSystem() = default;


	/*!	Define drop properties
	 *
	 */
	void	setDrop(sf::Vector2f const & dropSize, float speed, sf::Color const & color, ABiome & biome);

	/*!	Define the rectangle where the drops will appears. Drops are destroy when their y position is lower than the bottom of camera.
	 *
	 */
	void			setDropRect(sf::FloatRect const& dropRect);

	/*!	Define the number of drops produced at each seconds.
	 *
	 */
	void			setDropPerSecond(float count);

	void			update(sf::Time frameTime, float angle, octo::VertexBuilder & builder);
private:
	void			updateParticle(sf::Time frameTime, Particle& particle);
	bool			isDeadParticle(Particle const& particle, float bottom);
	void			createDrop(Particle & particle, float angle);
private:
	typedef std::uniform_real_distribution<float>		FDist;

	std::mt19937				m_engine;
	FDist						m_floatDistribution;
	std::size_t					m_maxDropCount;
	std::vector<Particle>		m_particles;
	sf::Vector2f				m_size;
	float						m_speed;
	sf::Color					m_color;

	sf::FloatRect				m_dropRect;
	unsigned int				m_dropPerSeconds;
	sf::Time					m_dropInterval;
	sf::Time					m_dropTimer;
	bool						m_canCreateDrop;
	float						m_waterLevel;
};

#endif
