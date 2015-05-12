#ifndef TILE_HPP
# define TILE_HPP

# include "Polygon.hpp"

class Tile : public Polygon
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
	static constexpr float	HalfTileSize = Tile::TileSize / 2.f;

	float			mf_noiseValue;
	bool			mb_isEmpty;
	ETransitionType		me_transition;
	sf::Vertex *		mp_upLeft;
	sf::Vector2f		m_startTransition[4];
	sf::Color		m_startColor;

	Tile(void);
	~Tile(void);

	bool isEmpty(void);
	void copy(Tile const & p_tile, float p_offsetX, float p_offsetY);
	void lightCopy(Tile const & p_tile);
	void setUpLeft(sf::Vertex * vertex);

	virtual sf::Vector2f const & getVertex(std::size_t index) const;

protected:
	virtual void computeCenter(void);
	virtual void computeEdges(void);

};

#endif
