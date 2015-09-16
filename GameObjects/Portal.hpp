#ifndef PORTAL_HPP
# define PORTAL_HPP

# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <AnimatedSprite.hpp>
# include <ParticleSystem.hpp>
# include <Math.hpp>
# include <Interpolations.hpp>

# include "AGameObject.hpp"
# include "ABiome.hpp"
# include "IPlaceable.hpp"

# include <random>
# include <ctime>

class CircleShape;

class Portal : public AGameObject<GameObjectType::Portal>, public IPlaceable
{
public:
	enum State
	{
		Appear,
		Activated,
		Disappear
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

		//TODO: getColor particle from next biome
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
	Portal(void);
	virtual ~Portal(void);

	void setPosition(sf::Vector2f const & position);
	void setRadius(float radius);
	void setBiome(ABiome & biome);
	inline float getRadius(void) const { return m_radius; }
	void appear(void);
	inline void disappear(void) { m_state = State::Disappear; }
	inline bool isActivated(void) const { return (m_state == Activated); }

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	// TODO: info du biome vers lequel on va se téléporter
	PortalParticle			m_particles;
	sf::Vector2f			m_position;
	sf::Shader				m_shader;
	std::size_t				m_shaderIndex;
	std::size_t				m_maxParticle;
	State					m_state;
	float					m_radius;
	float					m_timer;
	float					m_timerMax;
	CircleShape *			m_box;

	octo::SpriteAnimation	m_animation;
	octo::AnimatedSprite	m_sprite;
};

#endif
