#include "FactoryMap.hpp"
#include "MapHigh.hpp"
#include "MapClassic.hpp"
#include "MapInstance.hpp"
#include <assert.h>

FactoryMap::FactoryMap(void) :
	mn_width(0),
	mn_height(0)
{ }

FactoryMap::~FactoryMap(void) { }

void FactoryMap::init(unsigned int p_width, unsigned int p_height)
{
	mn_width = p_width;
	mn_height = p_height;
}

Map * FactoryMap::getMap(Map::EMapType p_mapType) const
{
	assert(mn_width != 0 && mn_height != 0);
	switch (p_mapType)
	{
		case Map::e_map_instance:
			return new MapInstance(mn_width, mn_height);
		case Map::e_map_high:
			return new MapHigh(mn_width, mn_height);
		case Map::e_map_classic:
		default:
			return new MapClassic(mn_width, mn_height);
	}
}
