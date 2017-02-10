#ifndef MUSICSYSTEM_HPP
# define MUSICSYSTEM_HPP

# include <ParticleSystem.hpp>
# include "RandomGenerator.hpp"

class MusicSystem : public octo::ParticleSystem<sf::Time, sf::Time, bool>
{
public:
	enum MyComponent
	{
		Time = User,
		Life,
		Side
	};

	MusicSystem();

	void			canEmit(bool canEmit);
	void			setColor(sf::Color const & color);
	void			setEmitter(sf::Vector2f const & emitter);
	void			update(sf::Time frameTime);

private:
	RandomGenerator	m_generator;
	sf::Vector2f	m_emitter;
	sf::Time		m_timer;
	sf::Time		m_nextCreation;
	sf::Color		m_color;
	bool			m_canEmit;

	virtual void	updateParticle(sf::Time frameTime, Particle & particle);
	virtual bool	isDeadParticle(Particle const & particle);

};


#endif
