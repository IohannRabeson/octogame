#ifndef FISHPINKNPC_HPP
# define FISHPINKNPC_HPP

# include "AFishNpc.hpp"

class FishPinkNpc : public AFishNpc
{
public:
	FishPinkNpc(ABiome & biome);
	virtual ~FishPinkNpc(void) = default;

	virtual void setup(void);

};

#endif
