#ifndef MAP_HPP
# define MAP_HPP

//TODO:remove
# include <map>
# include <utility>
# include <Array2D.hpp>
# include "Tile.hpp"

struct DecorValues
{
	int					mn_nb;
	int					mn_minSizeX;
	int					mn_minSizeY;
	int					mn_maxSizeX;
	int					mn_maxSizeY;
	int					mn_minAngle = 0;
	int					mn_maxAngle = 0;
	int					mn_minElement = 0;
	int					mn_maxElement = 0;
	int					mn_minLive = 0;
	int					mn_maxLive = 0;
};

struct Biome
{
	std::size_t			mn_height;
	std::size_t			mn_width;
	std::size_t			m_totalWidth;
	int					mn_temperature;
	int					mn_wind;
	std::size_t			mn_nbDecor;
	// between 0 and mn_width, it will be use to set the player at first appartition
	std::size_t			mn_startOffsetX;
	float				mf_transitionTimerMax;
	// Pattern to put the decor on the map
	DecorValues			m_tree;
	DecorValues			m_crystal;
	DecorValues			m_rock;
	DecorValues			m_star;
	DecorValues			m_sun;
	DecorValues			m_cloud;
	DecorValues			m_mushroom;
};

class Map
{
public:
	typedef std::vector<std::pair<int, sf::Vector2f>>	Decors;
	typedef Decors::iterator							Iterator;

	enum EMapType
	{
		e_map_classic,
		e_map_high,
		e_map_instance
	};

	typedef octo::Array2D<Tile*> TileMap;

	Map(void);
	virtual ~Map(void);

	inline std::size_t getColumns(void) const { return m_tiles.columns(); }
	inline std::size_t getRows(void) const { return m_tiles.rows(); }
	inline Tile & get(std::size_t column, std::size_t row) { return *m_tiles(column, row); }
	inline Tile const & get(std::size_t column, std::size_t row) const { return *m_tiles(column, row); }
	inline void computeMap(void) { computeMapRange(0, m_tiles.columns(), 0, m_tiles.rows()); }
	inline void computeMapRangeX(int startX, int endX) { computeMapRange(startX, endX, 0, m_tiles.rows()); }
	inline void computeMapRangeY(int startY, int endY) { computeMapRange(0, m_tiles.columns(), startY, endY); }
	inline void setCameraView(sf::Vector2f const * offset) { m_offset = offset; }
	inline void registerOffset(void) { m_curOffset = *m_offset; }
	inline Iterator begin(void) { return m_decorPositions.begin(); }
	inline Iterator end(void) { return m_decorPositions.end(); }

	void registerDecor(int x);
	void addOffsetX(int p_offsetX);
	void addOffsetY(int p_offsetY);

	void init(Biome * p_biome);

	//TODO: nor more virtual, pointer to function instead
	virtual void swapDepth(void);
	virtual void registerDepth(void);
	virtual void nextStep(void);
	virtual void previousStep(void);
	virtual void computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY);
	virtual void computeDecor(void);

protected:
	float					m_depth;
	float					m_oldDepth;
	Biome *					m_biome;
	TileMap					m_tiles;
	std::size_t				m_width;
	std::size_t				m_height;
	sf::Vector2f const *	m_offset;
	sf::Vector2f			m_curOffset;
	std::size_t				m_totalWidth;

	// Containes base value to avoid redundant calcul
	octo::Array2D<sf::Vector2f>		m_baseValue;

	virtual void initBiome(void);
	// first and second curve must return a value between -1 and 1
	virtual float firstCurve(float * vec);
	virtual float secondCurve(float * vec);
	virtual void setColor(Tile & p_tile);

private:
	std::vector<std::pair<int, sf::Vector2f>>	m_decorPositions;

};

#endif
