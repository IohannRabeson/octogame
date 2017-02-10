#ifndef TILE_HPP
# define TILE_HPP

# include <SFML/Graphics.hpp>
# include <utility>
# include <LevelMap.hpp>
# include "AGameObject.hpp"

enum class ETransitionType : char
{
	e_transition_appear,
	e_transition_disappear,
	e_transition_already,
	e_transition_none
};

class Tile : public AGameObject<GameObjectType::Tile>
{
public:
	Tile(void);
	~Tile(void) = default;

	//TEST ENUM CHAR
	static constexpr float	TileSize = 16.f;
	static constexpr float	DoubleTileSize = Tile::TileSize * 2.f;
	static constexpr float	TripleTileSize = Tile::TileSize * 3.f;
	static constexpr float	HalfTileSize = Tile::TileSize / 2.f;

	inline void setTransitionType(ETransitionType const & transitionType) { m_transitionType = transitionType; }
	inline ETransitionType const & getTransitionType(void) const { return m_transitionType; }
	inline bool isTransitionType(ETransitionType const & transitionType) { return (m_transitionType == transitionType); }

	inline void setStartColor(sf::Color && startColor) { m_startColor = std::forward<sf::Color>(startColor); }
	inline void setStartColor(sf::Color const & startColor) { m_startColor = startColor; }
	inline sf::Color & getStartColor(void) { return m_startColor; }

	inline void setIsEmpty(bool isEmpty) { m_isEmpty = isEmpty; }
	inline bool isEmpty(void) const { return m_isEmpty; }

	inline void setTileType(octo::LevelMap::TileType tileType) { m_tileType = tileType; }
	inline octo::LevelMap::TileType getTileType(void) const { return m_tileType; }

	inline void setStartTransition(std::size_t index, sf::Vector2f && startTransition) { setStartTransition(index, startTransition.x, startTransition.y); }
	inline void setStartTransition(std::size_t index, float x, float y) { m_startTransition[index].x = x; m_startTransition[index].y = y; }
	inline void setStartTransitionY(std::size_t index, float y) { m_startTransition[index].y = y; }
	inline sf::Vector2f const & getStartTransition(std::size_t index) const { return m_startTransition[index]; }

private:
	sf::Vector2f				m_startTransition[4];
	ETransitionType				m_transitionType;
	sf::Color					m_startColor;
	bool						m_isEmpty;
	octo::LevelMap::TileType	m_tileType;

};

#endif
