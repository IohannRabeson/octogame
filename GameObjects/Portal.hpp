#ifndef PORTAL_HPP
# define PORTAL_HPP

# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <AnimatedSprite.hpp>
# include <CharacterSprite.hpp>
# include <ParticleSystem.hpp>
# include <Math.hpp>
# include <Interpolations.hpp>

# include "AGameObject.hpp"
# include "ABiome.hpp"
# include "IPlaceable.hpp"
# include "ResourceDefinitions.hpp"

# include <random>
# include <ctime>

class CircleShape;

class Portal : public AGameObject<GameObjectType::Portal>, public IPlaceable
{
public:
	enum State
	{
		FirstAppear,
		Appear,
		Activated,
		Disappear
	};
	enum Events
	{
		Closed,
		Opening,
		Opened
	};

private:
	class PortalParticle : public octo::ParticleSystem<sf::Time, sf::Time, sf::Vector2f, sf::Vector2f>
	{
	public:
		enum MyComponent
		{
			Time = User,
			Life,
			Start,
			End
		};

		PortalParticle(void);
		void setRadius(float radius);
		void setBiome(ABiome & biome);
		inline void setColor(sf::Color const & color) { m_color = color; }
		inline void setEmitter(sf::Vector2f const & emitter) { m_emitter = emitter; }
		inline void setMaxParticle(std::size_t maxParticle) { m_maxParticle = maxParticle; }
		void update(sf::Time frameTime);

	private:
		void updateParticle(sf::Time frameTime, Particle& particle);
		bool isDeadParticle(Particle const& particle);

		typedef std::uniform_real_distribution<float>	Dist;
		sf::Vector2f	m_emitter;
		std::size_t		m_maxParticle;
		sf::Color		m_color;
		float			m_radius;
		std::mt19937	m_engine;
		Dist			m_lifeTimeDistri;
		Dist			m_directionDistri;
		Dist			m_distanceDistri;
		ABiome *		m_biome;
	};

public:
	Portal(Level destination, ResourceKey key);
	virtual ~Portal(void);

	void addMapOffset(float x, float y);
	sf::Vector2f const & getPosition(void) const;
	void setPosition(sf::Vector2f const & position);
	void setRadius(float radius);
	void setBiome(ABiome & biome);
	inline float getRadius(void) const { return m_radius; }
	void appear(void);
	bool isLock(void);
	inline void disappear(void) { m_state = State::Disappear; }
	inline bool isActivated(void) const { return m_isActive; }
	inline bool isOpening(void) { return m_sprite.getCurrentEvent() == Opening; }
	inline Level getDestination(void) const { return m_destination; }

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	PortalParticle			m_particles;
	Level					m_destination;
	sf::Vector2f			m_position;
	sf::Shader				m_shader;
	std::size_t				m_shaderIndex;
	std::size_t				m_maxParticle;
	State					m_state;
	float					m_radius;
	float					m_timer;
	float					m_timerMax;
	CircleShape *			m_box;
	bool					m_isActive;

	octo::CharacterAnimation	m_animationOpened;
	octo::CharacterAnimation	m_animationOpening;
	octo::CharacterAnimation	m_animationClosed;
	octo::CharacterSprite		m_sprite;
};

#endif
