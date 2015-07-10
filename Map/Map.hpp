#ifndef MAP_HPP
# define MAP_HPP

# include <functional>
# include <Array2D.hpp>
# include "Tile.hpp"
# include "IMapTransformable.hpp"
# include "Noise.hpp"

class ABiome;
class MapInstance;

class Map : public IMapTransformable
{
public:
	typedef std::vector<std::pair<int, sf::Vector2f>>			Decors;
	typedef Decors::iterator									Iterator;
	typedef octo::Array2D<Tile*>								TileMap;
	typedef std::function<float(float x, float y)>				MapSurfaceCallback;
	typedef std::function<sf::Color(float x, float y, float z)>	TileColorCallback;

	Map(void);
	virtual ~Map(void);

	inline std::size_t getColumns(void) const { return m_tiles.columns(); }
	inline std::size_t getRows(void) const { return m_tiles.rows(); }
	inline Tile & get(std::size_t column, std::size_t row) { return *m_tiles(column, row); }
	inline Tile const & get(std::size_t column, std::size_t row) const { return *m_tiles(column, row); }
	inline void setMapSurface(MapSurfaceCallback mapSurface) { m_mapSurface = mapSurface; }
	inline void setTileColorCallback(TileColorCallback tileColor) { m_tileColor = tileColor; }

	inline void computeMap(void) { computeMapRange(0, m_tiles.columns(), 0, m_tiles.rows()); }
	inline void computeMapRangeX(int startX, int endX) { computeMapRange(startX, endX, 0, m_tiles.rows()); }
	inline void computeMapRangeY(int startY, int endY) { computeMapRange(0, m_tiles.columns(), startY, endY); }
	inline void setCameraView(sf::Vector2f const * offset) { m_offset = offset; }
	inline void registerOffset(void) { m_curOffset = *m_offset; }
	inline Iterator begin(void) { return m_decorPositions.begin(); }
	inline Iterator end(void) { return m_decorPositions.end(); }

	void registerDecor(int x);

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
	float										m_depth;
	float										m_oldDepth;
	float										m_mapJoinWidth;
	float										m_mapJoinHalfWidth;
	TileMap										m_tiles;
	std::size_t									m_width;
	std::size_t									m_height;
	sf::Vector2f const *						m_offset;
	sf::Vector2f								m_curOffset;
	std::vector<std::unique_ptr<MapInstance>>	m_instances;
	Decors										m_decorPositions;
	sf::Vector2u								m_mapSize;
	Noise										m_noise;
	MapSurfaceCallback							m_mapSurface;
	TileColorCallback							m_tileColor;

};

#endif
