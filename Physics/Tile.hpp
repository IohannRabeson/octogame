#ifndef TILE_HPP
# define TILE_HPP

# include <SFML/Graphics.hpp>

class ConvexShape;

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

	//TODO: accessors
	float			mf_noiseValue;
	bool			mb_isEmpty;
	ETransitionType		me_transition;
	//TODO: use another variable to store the position for the decors
	sf::Vertex *		mp_upLeft;
	sf::Vector2f		m_startTransition[4];
	sf::Color		m_startColor;
	ConvexShape *		m_shape;

	Tile(void);
	virtual ~Tile(void) = default;

	inline bool isEmpty(void) { return mb_isEmpty; }
	inline void setShape(ConvexShape * shape) { m_shape = shape; }
	void setPolygonVertices(sf::Vertex * vertex);

};

#endif
