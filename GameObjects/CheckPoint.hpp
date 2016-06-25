#ifndef CHECKPOINT_HPP
# define CHECKPOINT_HPP

# include <VertexBuilder.hpp>
# include "InstanceDecor.hpp"
# include "AGameObject.hpp"
# include <memory>

class CharacterOcto;
class RectangleShape;

class CheckPoint : public InstanceDecor, public AGameObject<GameObjectType::CheckPoint>
{
public:
	CheckPoint(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~CheckPoint(void) = default;

	void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void update(sf::Time frametime);

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

private:
	RectangleShape *				m_box;
	std::size_t						m_count;
	std::size_t						m_used;
	sf::Time						m_timer;
	sf::Time						m_timerMax;
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	octo::VertexBuilder				m_builder;
	sf::Vector2f					m_size;
	sf::Color						m_color;

	sf::Vector2f					m_startPosition;
	bool							m_firstFrame;
	bool							m_isValidated;
	octo::SpriteAnimation			m_animationValidated;

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
