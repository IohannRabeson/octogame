#ifndef BEAMSYSTEM_HPP
# define BEAMSYSTEM_HPP

# include "ABiome.hpp"
# include <ParticleSystem.hpp>
# include <random>

class BeamSystem : public octo::ParticleSystem<sf::Time, float, float>
{
	enum MyComponent
	{
		ElapsedTime = Component::User,
		UpSpeed,
		AngularVelocity
	};

public:
	BeamSystem(void);
	virtual ~BeamSystem(void) = default;

	void	setWidth(float width);
	void	setHeight(float height);
	void	setRotationFactor(float factor);
	void	setBiome(ABiome & biome);
	float	getHeight(void) const;
	float	getWidth(void) const;
	void	updateParticle(sf::Time frameTime, Particle& particle);
	void	update(sf::Time frameTime);
	void	createParticle();

private:
	bool	isDeadParticle(Particle const& particle);

	typedef std::uniform_real_distribution<float>	Distri;

	sf::Time				m_cycleTime;
	float					m_speedUp;
	float					m_width;
	float					m_height;
	float					m_rotationFactor;
	sf::Time				m_emitTimer;
	sf::Time				m_emitInterval;
	std::mt19937			m_random;
	Distri					m_distri;
	ABiome *				m_biome;
};

#endif
