#ifndef PEPETTENPC_HPP
# define PEPETTENPC_HPP

# include "ASwimNpc.hpp"

class PepetteNpc : public ASwimNpc
{
public:
	PepetteNpc(ABiome & biome);
	virtual ~PepetteNpc(void) = default;

	virtual void setup(void);
};

#endif
