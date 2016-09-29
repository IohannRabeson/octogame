#ifndef SYLVIENPC_HPP
# define SYLVIENPC_HPP

# include "ASwimNpc.hpp"

class SylvieNpc : public ASwimNpc
{
public:
	SylvieNpc(ABiome & biome);
	virtual ~SylvieNpc(void) = default;

	virtual void setup(void);
};

#endif
