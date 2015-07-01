#ifndef TILE_HPP
# define TILE_HPP

# include <SFML/Graphics.hpp>

class Tile
{
public:
	Tile(void);
	~Tile(void) = default;

	enum ETransitionType
	{
		e_transition_appear,
		e_transition_disappear,
		e_transition_already,
		e_transition_none
	};

	static constexpr float	TileSize = 16.f;
	static constexpr float	DoubleTileSize = Tile::TileSize * 2.f;
	static constexpr float	TripleTileSize = Tile::TileSize * 3.f;
	static constexpr float	HalfTileSize = Tile::TileSize / 2.f;

	inline void setTransitionType(ETransitionType transitionType) { m_transitionType = transitionType; }
	inline ETransitionType getTransitionType(void) const { return m_transitionType; }
	inline bool isTransitionType(ETransitionType transitionType) { return (m_transitionType == transitionType); }

	inline void setStartColor(sf::Color && startColor) { m_startColor = startColor; }
	inline void setStartColor(sf::Color const & startColor) { m_startColor = startColor; }
	inline sf::Color & getStartColor(void) { return m_startColor; }

	inline void setUpLeft(sf::Vertex * upLeft) { m_upLeft = upLeft; }
	inline sf::Vertex * getUpLeft(void) const { return m_upLeft; }

	inline void setIsEmpty(bool isEmpty) { m_isEmpty = isEmpty; }
	inline bool isEmpty(void) const { return m_isEmpty; }

	inline void setStartTransition(std::size_t index, sf::Vector2f && startTransition) { setStartTransition(index, startTransition.x, startTransition.y); }
	inline void setStartTransition(std::size_t index, float x, float y) { m_startTransition[index].x = x; m_startTransition[index].y = y; }
	inline void setStartTransitionY(std::size_t index, float y) { m_startTransition[index].y = y; }
	inline sf::Vector2f const & getStartTransition(std::size_t index) const { return m_startTransition[index]; }

private:
	sf::Vector2f		m_startTransition[4];
	ETransitionType		m_transitionType;
	sf::Color			m_startColor;
	sf::Vertex *		m_upLeft;
	bool				m_isEmpty;

};

#endif
