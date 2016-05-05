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
	static constexpr float OffsetTileX = 10.f;
	static constexpr float OffsetTileY = 3.f;
	static constexpr float OffsetX = Tile::TileSize * OffsetTileX;
	static constexpr float OffsetY = Tile::TileSize * OffsetTileY;

	typedef std::vector<std::pair<int, sf::Vector2f>>			Decors;
	typedef std::vector<std::pair<bool, sf::Vector2f>>			WideDecors;
	typedef octo::Array2D<Tile*>								TileMap;

	typedef std::function<float(Noise & noise, float x, float y)>				MapSurfaceGenerator;
	typedef std::function<sf::Color(Noise & noise, float x, float y, float z)>	TileColorGenerator;

	Map(void);
	virtual ~Map(void);

	inline std::size_t	getColumns(void) const { return m_tiles.columns(); }
	inline std::size_t	getRows(void) const { return m_tiles.rows(); }
	inline Tile & get(std::size_t column, std::size_t row) { return *m_tiles(column, row); }
	inline Tile const & get(std::size_t column, std::size_t row) const { return *m_tiles(column, row); }
	void setMapSurfaceGenerator(MapSurfaceGenerator mapSurface);
	void setTileColorGenerator(TileColorGenerator tileColor);

	inline void computeMap(void) { computeMapRange(0, m_tiles.columns(), 0, m_tiles.rows()); }
	inline void computeMapRangeX(int startX, int endX) { computeMapRange(startX, endX, 0, m_tiles.rows()); }
	inline void computeMapRangeY(int startY, int endY) { computeMapRange(0, m_tiles.columns(), startY, endY); }
	inline void setCameraView(sf::Vector2f const * offset) { m_offset = offset; }
	inline void registerOffset(void) { m_curOffset = *m_offset; }
	inline Decors const & getDecorsPosition(void) const { return m_decorPositions; }
	inline WideDecors const & getWideDecorsPosition(void) const { return m_wideDecorPositions; }


	void addOffsetX(int offsetX);
	void addOffsetY(int offsetY);

	void init(ABiome & biome);

	void registerDecor(int x);
	void registerWideDecor(std::size_t x);
	void computeMapRange(int startX, int endX, int startY, int endY);
	void computeDecor(void);
	void computeWideDecor(void);

	virtual void swapDepth(void);
	virtual void registerDepth(void);
	virtual void nextStep(void);
	virtual void previousStep(void);
	void registerOctoPos(sf::Vector2f const & octoPos);
	bool isOctoOnInstance(sf::IntRect const & instanceRect, sf::Vector2i const & octoPos);

private:
	typedef std::function<float(float x, float y)>				MapSurfaceGeneratorBind;
	typedef std::function<sf::Color(float x, float y, float z)>	TileColorGeneratorBind;

	float										m_depth;
	float										m_oldDepth;
	float										m_mapJoinWidth;
	float										m_mapJoinHalfWidth;
	float										m_transitionStep;
	TileMap										m_tiles;
	std::size_t									m_width;
	std::size_t									m_height;
	sf::Vector2f const *						m_offset;
	sf::Vector2f								m_curOffset;
	std::vector<std::unique_ptr<MapInstance>>	m_instances;
	std::vector<sf::IntRect>					m_instancesRect;
	Decors										m_decorPositions;
	WideDecors									m_wideDecorPositions;
	sf::Vector2u								m_mapSize;
	Noise										m_noise;
	MapSurfaceGeneratorBind						m_mapSurface;
	TileColorGeneratorBind						m_tileColor;
	sf::Vector2i								m_octoPos;
	std::size_t									m_instanceIndex;
	bool										m_isOctoOnInstance;
	sf::Color									m_moveColor;
};

#endif
