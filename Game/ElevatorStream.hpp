#ifndef ELEVATORBEAM_HPP
# define ELEVATORBEAM_HPP
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <AnimatedSprite.hpp>
# include <VertexBuilder.hpp>
# include <ParticleSystem.hpp>
# include <Math.hpp>

# include <array>
# include <cmath>
# include <iostream>

# include "AGameObject.hpp"
# include "IPlaceable.hpp"

class ABiome;
class RectangleShape;

class ElevatorStream : public AGameObject<GameObjectType::Elevator>, public IPlaceable
{
	class BeamParticle;
public:
	ElevatorStream();

	void		setupSprite(void);
	void		setHeight(float height);
	void		setTopY(float topY);
	void		setWidth(float width);
	void		setRotationFactor(float factor);
	void		setBiome(ABiome & biome);
	void		setPosition(sf::Vector2f const & position);

	float		getHeight(void) const;
	float		getWidth(void) const;
	float		getPosY(void) const;
	float		getTopY(void) const;
	sf::Vector2f const & getPosition(void) const;

	void		createRay(void);

	void		update(sf::Time frameTime);
	void		draw(sf::RenderTarget& render, sf::RenderStates states) const;
	void		drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	std::shared_ptr<BeamParticle>	m_particles;
	sf::Shader						m_shader;
	octo::VertexBuilder				m_builder;
	sf::Time						m_waveCycle;
	sf::Time						m_waveCycleDuration;
	RectangleShape*					m_box;
	float							m_topY;
	std::size_t						m_rayCountVertex;
	std::unique_ptr<sf::Vertex[]>	m_ray;
	sf::Color						m_borderColor;
	sf::Color						m_centerColor;
	sf::Color						m_upColor;
	sf::Vector2f					m_position;

	octo::SpriteAnimation			m_animation;
	octo::AnimatedSprite			m_spriteBottomFront;
	octo::AnimatedSprite			m_spriteBottomBack;
	octo::AnimatedSprite			m_spriteTopFront;
	octo::AnimatedSprite			m_spriteTopBack;
};

#endif
