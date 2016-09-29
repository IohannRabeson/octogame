#ifndef BEACHBOYSUBNPC_HPP
# define BEACHBOYSUBNPC_HPP

# include "ASwimNpc.hpp"

class BeachBoySubNpc : public ASwimNpc
{
public:
	BeachBoySubNpc(ABiome & biome);
	virtual ~BeachBoySubNpc(void) = default;

	virtual void setup(void);

};

#endif
