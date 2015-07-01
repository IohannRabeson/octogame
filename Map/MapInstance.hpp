#ifndef MAPINSTANCE_HPP
# define MAPINSTANCE_HPP

# include "Map.hpp"
# include "StaticTileObject.hpp"

class MapInstance : public Map
{
public:
	MapInstance(void);
	virtual ~MapInstance(void);

	virtual void swapDepth(void);
	virtual void registerDepth(void);
	virtual void nextStep(void);
	virtual void previousStep(void);
	virtual void computeMapRange(int startX, int endX, int startY, int endY);

protected:
	virtual void initBiome(void);
	virtual float firstCurve(float * vec);
	virtual float secondCurve(float * vec);
	virtual void setColor(Tile & tile);

private:
	StaticTileObject *		m_instance;

};

#endif
