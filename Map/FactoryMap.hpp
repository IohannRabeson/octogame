#ifndef FACTORY_MAP
# define FACTORY_MAP

# include "Map.hpp"

class FactoryMap
{
public:
	FactoryMap(void);
	virtual ~FactoryMap(void);

	void init(unsigned int p_width, unsigned int p_height);
	Map * getMap(Map::EMapType p_mapType) const;

private:
	int	mn_width;
	int	mn_height;

};

#endif
