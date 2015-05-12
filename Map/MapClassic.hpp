#ifndef MAPCLASSIC_HPP
# define MAPCLASSIC_HPP

# include "Map.hpp"

class MapClassic : public Map
{
public:
	MapClassic(unsigned int pn_width, unsigned int pn_height);
	virtual ~MapClassic(void);

	virtual void nextStep(void);
	virtual void previousStep(void);

protected:
	virtual void initBiome(void);
	virtual float firstCurve(float * vec);
	virtual float secondCurve(float * vec);
	virtual void setColor(Tile & p_tile);

private:
	MapClassic(void) = delete;

};

#endif
