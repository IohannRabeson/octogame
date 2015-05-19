#ifndef MAPHIGH_HPP
# define MAPHIGH_HPP

# include "Map.hpp"

class MapHigh : public Map
{
public:
	MapHigh(unsigned int pn_width, unsigned int pn_height);
	virtual ~MapHigh(void);

	virtual void nextStep(void);
	virtual void previousStep(void);

protected:
	virtual void initBiome(void);
	virtual float firstCurve(float * vec);
	virtual float secondCurve(float * vec);
	virtual void setColor(Tile & p_tile);

private:
	MapHigh(void) = delete;

};

#endif
