#ifndef FISHBLUENPC_HPP
# define FISHBLUENPC_HPP

# include "AFishNpc.hpp"

class FishBlueNpc : public AFishNpc
{
public:
	FishBlueNpc(ABiome & biome);
	virtual ~FishBlueNpc(void) = default;

	virtual void setup(void);

};

#endif
