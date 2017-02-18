#ifndef STARSYSTEM_HPP
# define STARSYSTEM_HPP

# include "RandomGenerator.hpp"
# include <ParticleSystem.hpp>

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
	void			setOrigin(sf::Vector2f const & origin);
	void			setEmitTimeRange(float min, float max);
	void			setup(sf::Vector2f const & sizeParticle);
	void			isFromTop(bool value);
	void			update(sf::Time frameTime);

private:
	typedef std::uniform_real_distribution<float>	Dist;

	RandomGenerator	m_generator;
	float			m_creationTimeMin;
	float			m_creationTimeMax;
	float			m_heightMin;
	float			m_heightMax;
	float			m_widthMin;
	float			m_widthMax;
	sf::Vector2f	m_sizeParticle;
	sf::Vector2f	m_speed;
	sf::Vector2f	m_origin;
	sf::Time		m_timer;
	sf::Time		m_nextCreation;
	sf::Color		m_color;
	bool			m_canEmit;
	bool			m_isFromTop;

	virtual void	updateParticle(sf::Time frameTime, Particle & particle);
	virtual bool	isDeadParticle(Particle const & particle);

	void			createOctogon(sf::Vector2f const & size,
								  sf::Vector2f const & sizeCorner,
								  StarSystem::Prototype & prototype);
	void			createTriangle(sf::Vertex const & p1,
								   sf::Vertex const & p2,
								   sf::Vertex const & p3,
								   StarSystem::Prototype & prototype);
};


#endif
