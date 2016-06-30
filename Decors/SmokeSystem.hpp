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
	void			setup(sf::Vector2f const & sizeParticle);
	void			setPosition(sf::Vector2f const & position);
	sf::Vector2f	getPositionEmitter(void) const;
	void			setColor(sf::Color const & color);
	void			setVelocity(sf::Vector2f const & direction);
	void			setScaleFactor(float scaleFactor);
	void			setDispersion(float dispersion);
	void			setEmitTimeRange(float min, float max);
	void			setGrowTimeRange(float min, float max);
	void			setLifeTimeRange(float min, float max);
	void			setScaleRange(float min, float max);
	void			setCanEmit(bool canEmit);

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

	sf::Vector2f	m_sizeParticle;
	sf::Color		m_color;
	sf::Vector2f	m_velocity;
	sf::Vector2f	m_emitter;
	sf::Time		m_emitTimer;
	sf::Time		m_emitTime;
	bool			m_canEmit;
	float			m_lifeScaleFactor;
	float			m_dispersion;
	std::mt19937	m_engine;
	DistReal		m_emitTimeDistri;
	DistReal		m_growTimeDistri;
	DistReal		m_lifeTimeDistri;
	DistInt			m_sideDistri;
	DistReal		m_scaleDistri;
};

#endif
