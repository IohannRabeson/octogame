#ifndef FISHREDNPC_HPP
# define FISHREDNPC_HPP

# include "AFishNpc.hpp"

class FishRedNpc : public AFishNpc
{
public:
	FishRedNpc(ABiome & biome);
	virtual ~FishRedNpc(void) = default;

	virtual void setup(void);

};

#endif
