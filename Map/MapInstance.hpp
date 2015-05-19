#ifndef MAPINSTANCE_HPP
# define MAPINSTANCE_HPP

# include "Map.hpp"
# include "StaticTileObject.hpp"

class MapInstance : public Map
{
public:
	MapInstance(unsigned int pn_width, unsigned int pn_height);
	virtual ~MapInstance(void);

	virtual void swapDepth(void);
	virtual void registerDepth(void);
	virtual void nextStep(void);
	virtual void previousStep(void);
	virtual void computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY);

protected:
	virtual void initBiome(void);
	virtual float firstCurve(float * vec);
	virtual float secondCurve(float * vec);
	virtual void setColor(Tile & p_tile);

private:
	MapInstance(void) = delete;
	StaticTileObject *		m_instance;

};

#endif
