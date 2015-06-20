/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainSystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:32:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/21 01:26:43 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAINSYSTEM_HPP
# define RAINSYSTEM_HPP

# include "ParticleSystem.hpp"

# include <random>

class RainSystem : public ParticleSystem<sf::Vector2f>
{
	enum MyComponent
	{
		Velocity = User
	};
public:
	RainSystem();

	void			setCameraRect(sf::FloatRect const& cameraRect);

	/*!	Define the number of drops produced at each seconds.
	 *
	 *	Default is 20.	
	 */
	void			setDropPerSecond(unsigned int count);

	void			setDropAngle(float angle);
	void			setDropGravity(float gravity);

	void			update(sf::Time frameTime);
private:
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);
	void			createDrop();
	void			updateHorizontalOffset();
private:
	typedef std::uniform_real_distribution<float>		FDist;
	typedef std::uniform_int_distribution<unsigned int>	UIDist;

	std::mt19937				m_engine;
	std::vector<unsigned int>	m_dropChances;
	FDist						m_floatDistribution;
	sf::FloatRect				m_cameraRect;
	sf::Vector2f				m_initialVelocity;
	float						m_initialRotation;
	unsigned int				m_dropPerSeconds;
	sf::Time					m_dropInterval;
	sf::Time					m_dropTimer;
	float						m_horizontalOffset;
};

#endif
