/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainSystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/20 02:32:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 03:45:24 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAINSYSTEM_HPP
# define RAINSYSTEM_HPP

# include <ParticleSystem.hpp>

# include <random>

class RainSystem : public octo::ParticleSystem<sf::Vector2f>
{
	enum MyComponent
	{
		Velocity = User
	};
public:
	RainSystem();

	void			setRainRect(sf::FloatRect const& cameraRect);

	/*!	Define the number of drops produced at each seconds.
	 *
	 *	Default is 20.	
	 */
	void			setDropPerSecond(unsigned int count);

	/*!	Define the drop fall angle
	 *	\param angle Angle of drop falls in degrees, 0 means the drops falls straight.
	 */
	void			setDropAngle(float angle);

	/*!	Define the drop falls speed.
	 *	\param speed Falling speed
	 */
	void			setDropSpeed(float speed);

	/*!	Enlarge at left and right the emitter range.
	 *	This methode is usefull if the camera is moving.
	 *	\param margin Margin size, default is 0.
	 */
	void			setMargin(float margin);

	void			update(sf::Time frameTime);
	void			update(sf::Time frameTime, octo::VertexBuilder & builder);
private:
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);
	void			createDrop();
	void			updateHorizontalOffset();
private:
	typedef std::uniform_real_distribution<float>		FDist;

	std::mt19937				m_engine;
	FDist						m_floatDistribution;
	sf::FloatRect				m_cameraRect;
	sf::Vector2f				m_initialVelocity;
	float						m_initialRotation;
	unsigned int				m_dropPerSeconds;
	sf::Time					m_dropInterval;
	sf::Time					m_dropTimer;
	float						m_horizontalOffset;
	float						m_margin;
};

#endif
