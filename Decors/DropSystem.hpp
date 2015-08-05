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

# include <ParticleSystem.hpp>

# include <random>

class DropSystem : public octo::ParticleSystem<sf::Vector2f>
{
	enum MyComponent
	{
		Velocity = User
	};
public:
	DropSystem();

	/*!	Define the rectangle where the drops will appears. Drops are destroy when their y position is lower than the bottom of camera.
	 *
	 */
	void			setDropRect(sf::FloatRect const& dropRect);

	/*!	Define the color of drops.
	 *
	 */
	void			setDropColor(sf::Color const& color);

	/*!	Define the size of drops.
	 *
	 */
	void			setDropSize(sf::Vector2f const & dropSize);

	/*!	Define the number of drops produced at each seconds.
	 *
	 */
	void			setDropPerSecond(float count);

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
	sf::FloatRect				m_dropRect;
	sf::Vector2f				m_initialVelocity;
	float						m_initialRotation;
	unsigned int				m_dropPerSeconds;
	sf::Time					m_dropInterval;
	sf::Time					m_dropTimer;
	bool						m_canCreateDrop;
	sf::Color					m_color;
};

#endif
