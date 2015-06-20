/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainSystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:32:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/20 04:27:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAINSYSTEM_HPP
# define RAINSYSTEM_HPP

# include "ParticleSystem.hpp"

# include <random>

class RainSystem : public ParticleSystem<>
{
public:
	RainSystem();

	void			setCameraRect(sf::FloatRect const& cameraRect);

	/*!	Define the maximum of drops produced at each frame. */
	void			setDropCountFactor(unsigned int factor);

	/*!	Define the percent of chance to produce drops at each frame. */
	void			setDropChance(unsigned int chance);

	void			setDropVelocity(sf::Vector2f const& velocity);

	void			setDropGravity(float gravity);

	void			update(sf::Time frameTime);
private:
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);
	void			createDrop();
private:
	typedef std::uniform_real_distribution<float>		FDist;
	typedef std::uniform_int_distribution<unsigned int>	UIDist;

	std::mt19937				m_engine;
	std::vector<unsigned int>	m_dropChances;
	FDist						m_floatDistribution;
	UIDist						m_dropChanceDistribution;
	sf::FloatRect				m_cameraRect;
	sf::Vector2f				m_initialVelocity;
	float						m_initialRotation;
	unsigned int				m_dropChance;
	unsigned int				m_dropCountFactor;
};

#endif
