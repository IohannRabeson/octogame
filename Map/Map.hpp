#ifndef MAP_HPP
# define MAP_HPP

# include <map>
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
	unsigned int		mn_height;
	unsigned int		mn_width;
	int					mn_temperature;
	unsigned int		mn_nbDecor;
	// between 0 and mn_width, it will be use to set the player at first appartition
	unsigned int		mn_startOffsetX;
	float				mf_transitionTimerMax;
	// Pattern to put the decor on the map
	DecorValues			m_tree;
	DecorValues			m_crystal;
	DecorValues			m_rock;
	DecorValues			m_star;
};

class Map
{
private:
	typedef std::map<int, Tile*> Decors;

public:
	enum EMapType
	{
		e_map_classic,
		e_map_high,
		e_map_instance
	};

	typedef octo::Array2D<Tile*> TileMap;

	Map(unsigned int pn_width, unsigned int pn_height);
	virtual ~Map(void);

	sf::Vertex * getHeight(int x);
	int getOffsetXDecor(int p_decorOffsetX) const;

	inline unsigned int getColumns(void) const { return m_tiles.columns(); }
	inline unsigned int getRows(void) const { return m_tiles.rows(); }
	inline int getOffsetX(void) const { return mn_offsetX; }
	inline int getOffsetY(void) const { return mn_offsetY; }
	inline Tile & get(unsigned int column, unsigned int row) { return *m_tiles(column, row); }
	inline Tile const & get(unsigned int column, unsigned int row) const { return *m_tiles(column, row); }
	inline Decors & getDecors(void) { return m_decors; }

	void init(Biome * p_biome);
	void computeMap(void);
	void computeDecor(void);
	void addOffsetX(int p_offsetX);
	void addOffsetY(int p_offsetY);

	virtual void swapDepth(void);
	virtual void registerDepth(void);
	virtual void nextStep(void) = 0;
	virtual void previousStep(void) = 0;
	virtual void computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY);

protected:
	float			mf_depth;
	float			mf_oldDepth;
	Biome *			m_biome;
	TileMap			m_tiles;
	unsigned int		mn_totalWidth;
	int			mn_offsetX;
	int			mn_offsetY;

	virtual void initBiome(void) = 0;
	// first and second curve must return a value between -1 and 1
	virtual float firstCurve(float * vec) = 0;
	virtual float secondCurve(float * vec) = 0;
	virtual void setColor(Tile & p_tile) = 0;

private:
	Map(void) = delete;
	void initQuad(int x, int y);

	static constexpr unsigned int MaxDecor = 200u;

	Tile						m_reserveTile[MaxDecor];
	Decors						m_decors;
	unsigned int					mn_decorTileCount;
	std::unique_ptr<sf::Vertex[]>			m_vertices;

};

#endif
