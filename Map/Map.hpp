#ifndef NEWMAP_HPP
# define NEWMAP_HPP

# include <map>
# include <Array2D.hpp>
# include "Tile.hpp"
# include "IMapTransformable.hpp"

class ABiome;
class MapInstance;

class Map : public IMapTransformable
{
private:
	typedef std::map<int, Tile*> Decors;

public:
	typedef octo::Array2D<Tile*> TileMap;

	Map(void);
	virtual ~Map(void);

	sf::Vertex * getHeight(int x);

	inline std::size_t getColumns(void) const { return m_tiles.columns(); }
	inline std::size_t getRows(void) const { return m_tiles.rows(); }
	inline Tile & get(std::size_t column, std::size_t row) { return *m_tiles(column, row); }
	inline Tile const & get(std::size_t column, std::size_t row) const { return *m_tiles(column, row); }
	inline Decors & getDecors(void) { return m_decors; }

	inline void computeMap(void) { computeMapRange(0, m_tiles.columns(), 0, m_tiles.rows()); }
	inline void computeMapRangeX(int startX, int endX) { computeMapRange(startX, endX, 0, m_tiles.rows()); }
	inline void computeMapRangeY(int startY, int endY) { computeMapRange(0, m_tiles.columns(), startY, endY); }
	inline void setCameraView(sf::Vector2f const * offset) { m_offset = offset; }
	inline void registerOffset(void) { m_curOffset = *m_offset; }

	void addOffsetX(int offsetX);
	void addOffsetY(int offsetY);

	void init(ABiome & biome);

	virtual void swapDepth(void);
	virtual void registerDepth(void);
	virtual void nextStep(void);
	virtual void previousStep(void);
	virtual void computeMapRange(int startX, int endX, int startY, int endY);
	virtual void computeDecor(void);

private:
	float								m_depth;
	float								m_oldDepth;
	TileMap								m_tiles;
	std::size_t							m_width;
	std::size_t							m_height;
	sf::Vector2f const *				m_offset;
	sf::Vector2f						m_curOffset;
	std::vector<std::unique_ptr<MapInstance>>	m_instances;

	// first and second curve must return a value between -1 and 1
	//TODO: nor more virtual, pointer to function instead
	virtual float firstCurve(float * vec);
	virtual float secondCurve(float * vec);
	virtual void setColor(Tile & tile);

	static constexpr std::size_t MaxDecor = 200u;

	Tile							m_reserveTile[MaxDecor];
	Decors							m_decors;
	std::size_t						m_decorTileCount;
	std::unique_ptr<sf::Vertex[]>	m_vertices;

	sf::Vector2u					m_mapSize;

};

#endif
