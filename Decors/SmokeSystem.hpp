#ifndef SMOKESYSTEM_HPP
# define SMOKESYSTEM_HPP

# include <ParticleSystem.hpp>
# include <DefaultGraphicsListeners.hpp>
# include <random>

class SmokeSystem : public octo::ParticleSystem<sf::Time, sf::Time, float>,
					public octo::DefaultMouseListener
{
public:
	enum MyComponent
	{
		Time = User,
		Life,
		Direction
	};

	SmokeSystem();

	void			update(sf::Time frameTime);	
private:
	void			createOctogon(sf::Vector2f const & size,
								  sf::Vector2f const & sizeCorner,
								  SmokeSystem::Prototype & prototype);
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);

	virtual void	onMoved(sf::Event::MouseMoveEvent const&);
	virtual void	onPressed(sf::Event::MouseButtonEvent const&);
	virtual void	onReleased(sf::Event::MouseButtonEvent const&);
private:
	typedef std::uniform_real_distribution<float>	Dist;

	sf::Vector2f	m_emitter;
	bool			m_emit;
	std::mt19937	m_engine;
	Dist			m_lifeTimeDistri;
	Dist			m_directionDistri;
};

#endif
