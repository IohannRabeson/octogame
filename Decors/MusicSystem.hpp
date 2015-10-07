#ifndef MUSICSYSTEM_HPP
# define MUSICSYSTEM_HPP

# include <ParticleSystem.hpp>
# include <random>

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
	typedef std::uniform_real_distribution<float>	Dist;
	typedef std::bernoulli_distribution				DistBool;

	sf::Vector2f	m_emitter;
	std::mt19937	m_engine;
	Dist			m_creationTimeDistri;
	Dist			m_lifeTimeDistri;
	DistBool		m_boolDistri;
	sf::Time		m_timer;
	sf::Time		m_nextCreation;
	sf::Color		m_color;
	bool			m_canEmit;

	virtual void	updateParticle(sf::Time frameTime, Particle & particle);
	virtual bool	isDeadParticle(Particle const & particle);

};


#endif
