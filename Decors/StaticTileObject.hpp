#ifndef STATICTILEOBJECT_HPP
# define STATICTILEOBJECT_HPP

# include "GameObject.hpp"
# include "Map.hpp"

class StaticTileObject : public GameObject
{
public:
	StaticTileObject(unsigned int p_width, unsigned int p_height, unsigned int p_depth);
	virtual ~StaticTileObject(void);

	// Load the tilemap from resources
	virtual void load(void);

	void swapDepth(void);
	void registerDepth(void);
	void nextStep(void);
	void previousStep(void);
	Tile const & get(unsigned int x, unsigned int y) const;
	unsigned int getWidth(void) const;
	unsigned int getHeight(void) const;

private:
	StaticTileObject(void) = delete;

	Map::TileMap *			m_tiles;
	unsigned int			mn_maxDepth;
	int				mn_depth;
	int				mn_oldDepth;
	unsigned int			mn_width;
	unsigned int			mn_height;

};

#endif
