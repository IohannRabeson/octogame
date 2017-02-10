#ifndef FISHBLACKNPC_HPP
# define FISHBLACKNPC_HPP

# include "AFishNpc.hpp"

class FishBlackNpc : public AFishNpc
{
public:
	FishBlackNpc(ABiome & biome);
	virtual ~FishBlackNpc(void) = default;

	virtual void setup(void);

};

#endif
