#ifndef MONOLITHSTEP_HPP
# define MONOLITHSTEP_HPP

# include "ResourceDefinitions.hpp"
# include <AnimatedSprite.hpp>
# include <VertexBuilder.hpp>
# include <memory>

class MonolithStep
{
public:
	MonolithStep(ResourceKey const & key, sf::Vector2f const & position, sf::Vector2f const & scale);
	virtual ~MonolithStep(void) = default;

	void addMapOffset(float x, float y);
	void setPosition(sf::Vector2f const & position);
	void activate(void);

	void update(sf::Time frametime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	octo::AnimatedSprite			m_sprite;
	octo::SpriteAnimation			m_animation;
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	octo::VertexBuilder				m_builder;
	sf::Vector2f					m_size;
	sf::Time						m_timer;
	sf::Time						m_timerMax;
	std::size_t						m_used;
	bool							m_isActivated;

	void createLosange(sf::Vector2f const & size,
											sf::Vector2f const & origin,
											sf::Color const & color,
											octo::VertexBuilder& builder);

	void createEffect(sf::Vector2f const & size,
											sf::Vector2f const & origin,
											float glowingCoef,
											sf::Color color,
											octo::VertexBuilder& builder);


};

#endif
