#ifndef STARSYSTEM_HPP
# define STARSYSTEM_HPP

# include <ParticleSystem.hpp>
# include <random>

class StarSystem : public octo::ParticleSystem<sf::Time>
{
public:
	enum MyComponent
	{
		Time = User
	};

	StarSystem();

	void			canEmit(bool canEmit);
	void			setColor(sf::Color const & color);
	void			setSpeed(sf::Vector2f const & speed);
	void			update(sf::Time frameTime);

private:
	typedef std::uniform_real_distribution<float>	Dist;

	std::mt19937	m_engine;
	Dist			m_creationTimeDistri;
	Dist			m_heightDistri;
	sf::Vector2f	m_speed;
	sf::Time		m_timer;
	sf::Time		m_nextCreation;
	sf::Color		m_color;
	bool			m_canEmit;

	virtual void	updateParticle(sf::Time frameTime, Particle & particle);
	virtual bool	isDeadParticle(Particle const & particle);

};


#endif
