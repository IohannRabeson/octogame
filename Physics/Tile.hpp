#ifndef TILE_HPP
# define TILE_HPP

# include "Polygon.hpp"

class Tile
{
public:
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

	float			mf_noiseValue;
	bool			mb_isEmpty;
	ETransitionType		me_transition;
	sf::Vertex *		mp_upLeft;
	sf::Vector2f		m_startTransition[4];
	sf::Color		m_startColor;

	Tile(void);
	virtual ~Tile(void);

	inline bool isEmpty(void) { return mb_isEmpty; }
	inline void setUpLeft(sf::Vertex * vertex) { /*m_recompute = true;*/ mp_upLeft = vertex; }

	virtual sf::Vector2f const & getVertex(std::size_t index) const;

};

#endif
