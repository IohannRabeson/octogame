#ifndef MAP_HPP
# define MAP_HPP

# include <map>
# include "Matrix.hpp"
# include "Tile.hpp"

struct DecorValues
{
	int					mn_nbDecor;
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

	typedef Matrix<Tile*> TileMap;

	Map(unsigned int pn_width, unsigned int pn_height);
	virtual ~Map(void);

	sf::Vertex * getHeight(int x);
	unsigned int getColumns(void) const;
	unsigned int getRows(void) const;
	int getOffsetX(void) const;
	int getOffsetXDecor(int p_decorOffsetX) const;
	int getOffsetY(void) const;
	Tile & get(unsigned int column, unsigned int row);
	Tile const & get(unsigned int column, unsigned int row) const;
	Decors & getDecors(void);

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
