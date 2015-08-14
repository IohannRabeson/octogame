#ifndef PORTAL_HPP
# define PORTAL_HPP

# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <ParticleSystem.hpp>
# include <Math.hpp>
# include <Interpolations.hpp>

# include "AGameObject.hpp"

# include <random>
# include <ctime>

class Portal : public AGameObject<GameObjectType::Portal>
{
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

		PortalParticle() :
			m_maxParticle(20u),
			m_engine(std::time(0)),
			m_lifeTimeDistri(1.f, 2.f),
			m_directionDistri(0.f, octo::Pi2),
			m_distanceDistri(96.f, 128.f)
		{}

		void setEmitter(sf::Vector2f const & emitter) { m_emitter = emitter; }
		void setMaxParticle(std::size_t maxParticle) { m_maxParticle = maxParticle; }

		void update(sf::Time frameTime)
		{
			ParticleSystem::update(frameTime);
			if (ParticleSystem::getCount() < m_maxParticle)
			{
				float direction = m_directionDistri(m_engine);
				float distance = m_distanceDistri(m_engine);
				emplace(sf::Color(84, 42, 94), m_emitter, sf::Vector2f(1.f, 1.f), m_directionDistri(m_engine) * 180.f,
						sf::Time::Zero,
						sf::seconds(m_lifeTimeDistri(m_engine)),
						m_emitter + sf::Vector2f(std::sin(direction) * distance, std::cos(direction) * distance), m_emitter);
			}
		}

	private:
		virtual void updateParticle(sf::Time frameTime, Particle& particle)
		{
			static float const			AngularVelocity = 30.f;

			std::get<Component::Position>(particle) = octo::linearInterpolation(std::get<MyComponent::Start>(particle), std::get<MyComponent::End>(particle), std::get<MyComponent::Time>(particle) / std::get<MyComponent::Life>(particle));
			std::get<Component::Rotation>(particle) += AngularVelocity * frameTime.asSeconds();
			std::get<MyComponent::Time>(particle) += frameTime;
			std::get<Component::Color>(particle).a = 255 - 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
		}

		virtual bool isDeadParticle(Particle const& particle)
		{
			return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
		}

		typedef std::uniform_real_distribution<float>	Dist;

		sf::Vector2f	m_emitter;
		std::size_t		m_maxParticle;
		std::mt19937	m_engine;
		Dist			m_lifeTimeDistri;
		Dist			m_directionDistri;
		Dist			m_distanceDistri;
	};

public:
	Portal(void);
	virtual ~Portal(void) = default;

	inline void setPosition(sf::Vector2f const & position) { m_position = position; }
	inline sf::Vector2f const & getPosition(void) const { return m_position; }

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget& render)const;

private:
	PortalParticle		m_particles;
	sf::Vector2f		m_position;
	sf::Shader			m_shader;
	std::size_t			m_shaderIndex;
	float				m_radius;
	float				m_timer;
	float				m_timerMax;
	bool				m_side;
	//TODO trigger physic box
	//TODO add spawnPortal, and disappearPortal

};

#endif
