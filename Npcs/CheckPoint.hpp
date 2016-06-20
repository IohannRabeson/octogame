#ifndef CHECKPOINT_HPP
# define CHECKPOINT_HPP

# include <VertexBuilder.hpp>
# include "ANpc.hpp"

class CharacterOcto;

class CheckPoint : public ANpc, public AGameObject<GameObjectType::CheckPoint>
{
public:
	CheckPoint(void);
	virtual ~CheckPoint(void) = default;

	virtual void setup(void);
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void update(sf::Time frametime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
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
