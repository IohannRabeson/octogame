#ifndef MONOLITHSTEP_HPP
# define MONOLITHSTEP_HPP

# include "ResourceDefinitions.hpp"
# include "RandomGenerator.hpp"
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
	void setEndPosition(sf::Vector2f const & position);
	void addPosition(sf::Vector2f const & position);
	void setStartEndTransition(float transition);
	void activate(void);
	void firstActivate(void);

	void update(sf::Time frametime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	enum State
	{
		None,
		FirstActivate,
		Activate,
	};
	RandomGenerator					m_generator;
	octo::AnimatedSprite			m_sprite;
	octo::SpriteAnimation			m_animation;
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	octo::VertexBuilder				m_builder;
	sf::Vector2f					m_size;
	sf::Vector2f					m_position;
	sf::Vector2f					m_endPosition;
	sf::Time						m_timer;
	sf::Time						m_timerMax;
	State							m_state;
	std::size_t						m_used;
	float							m_offset;
	float							m_startEndTransition;

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
