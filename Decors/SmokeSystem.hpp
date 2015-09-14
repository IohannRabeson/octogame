#ifndef SMOKESYSTEM_HPP
# define SMOKESYSTEM_HPP

# include <ParticleSystem.hpp>
# include <DefaultGraphicsListeners.hpp>
# include <random>

class SmokeSystem : public octo::ParticleSystem<sf::Time, sf::Time, sf::Time, sf::Time, int>
{
public:
	enum MyComponent
	{
		GrowTimer = User,
		GrowTimerMax,
		LifeTimer,
		LifeTimerMax,
		Behaviour
	};

	SmokeSystem();

	void			update(sf::Time frameTime);
	void			setPosition(sf::Vector2f const & position);
	void			setVelocity(sf::Vector2f const & direction);
	void			setEmitTimeMax(float min, float max);
	void			setColor(sf::Color const & color);

private:
	void			createOctogon(sf::Vector2f const & size,
								  sf::Vector2f const & sizeCorner,
								  SmokeSystem::Prototype & prototype);
	void			createTriangle(sf::Vertex const & p1,
								   sf::Vertex const & p2,
								   sf::Vertex const & p3,
								   SmokeSystem::Prototype & prototype);
	void			createParticle(void);
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);

private:
	typedef std::uniform_real_distribution<float>	DistReal;
	typedef std::uniform_int_distribution<int>		DistInt;

	sf::Vector2f	m_size;
	sf::Color		m_color;
	sf::Vector2f	m_velocity;
	sf::Vector2f	m_emitter;
	sf::Time		m_emitTimer;
	sf::Time		m_emitInterval;
	float			m_lifeScaleFactor;
	std::mt19937	m_engine;
	DistReal		m_emitIntervalDistri;
	DistReal		m_growTimeDistri;
	DistReal		m_lifeTimeDistri;
	DistInt			m_sideDistri;
	DistReal		m_scaleDistri;
};

#endif
